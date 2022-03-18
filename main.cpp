/*
 * @Description: Read CoMPASS Bin file and convert it to CSV
 * @Author: Ming Fang
 * @Date: 2022-03-18 15:52:58
 * @LastEditors: Ming Fang
 * @LastEditTime: 2022-03-18 18:14:34
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <filesystem>

class Event
{
private:
    /* data */
public:
    Event(const char* buf, const int bufSize);
    uint16_t board;
    uint16_t channel;
    uint64_t timetag;
    uint16_t energy;
    uint16_t energyShort;
    uint32_t flag;
    std::vector<uint16_t> samples;

    friend std::ostream& operator<<(std::ostream& of, const Event& event);
};

std::ostream& operator<<(std::ostream& os, const Event& event)
{
    os << event.board << ';'
       << event.channel << ';'
       << event.timetag << ';'
       << event.energy << ';'
       << event.energyShort << ';'
       << event.flag << ';';
    for (std::size_t i = 0; i < event.samples.size()-1; i++)
    {
        os << event.samples[i] << ';';
    }
    os << event.samples[event.samples.size()-1];
    return os;
}

int getSampleNumber(const std::string path, const int v)
{
    uint32_t sampleNumber(0);
    std::ifstream fileptr;
    fileptr.open(path, std::ios::in | std::ios::binary);
    if (!fileptr.good())
    {
        throw std::invalid_argument(std::string("Cannot open file: ") + path);
        return 0;
    }
    int headerSizeBeforeSampleNumber = 20;
    if(v == 2)
    {
        fileptr.ignore(2); // ignore the first 2 bytes of the file
        headerSizeBeforeSampleNumber += 1; // one additional byte for waveform code
    }
    while (!fileptr.eof())
    {
        //read header
        fileptr.ignore(headerSizeBeforeSampleNumber);
        fileptr.read((char *)&(sampleNumber), 4);
        break;
    }
    fileptr.close();
    return sampleNumber;
}

Event::Event(const char* buf, const int bufSize)
{
    std::memcpy(&board, buf, sizeof(decltype(board)));
    std::memcpy(&channel, &buf[2], sizeof(decltype(channel)));
    std::memcpy(&timetag, &buf[4], sizeof(decltype(timetag)));
    std::memcpy(&energy, &buf[12], sizeof(decltype(energy)));
    std::memcpy(&energyShort, &buf[14], sizeof(decltype(energyShort)));
    std::memcpy(&flag,  &buf[16], sizeof(decltype(flag)));
    // skip waveform code, 1 byte

    uint32_t nSample(0);
    std::memcpy(&nSample, &buf[21], sizeof(uint32_t));
    if (bufSize - 25 != nSample*2)
    {
        throw std::invalid_argument(std::string("Expected ") + std::to_string(nSample) + " samples, but got" + std::to_string(bufSize - 25));
    }
    samples = std::vector<uint16_t>(nSample, 0);
    std::memcpy(samples.data(), &buf[25], 2*nSample);
}

int main(int argc, char** argv)
{
    const int CoMPASSVersion = 2;
    // get binary file name from cmd
    const std::string binFilePath(argv[1]);
    // get max num of events from cms
    const uint64_t targetNumEvents(std::stoi(argv[2]));
    // const std::string binFilePath("/media/ming/SeagateDrive/dcr_FJ30035_A3818/DAQ/run_27p5V/RAW/DataR_CH0@DT5730S_2263_run_27p5V.BIN");
    std::cout << "Binary input file: " << binFilePath << std::endl;
    // replace bin/BIN extension with CSV
    const std::string csvFilePath = binFilePath.substr(0, binFilePath.find_last_of('.')) + ".CSV";
    std::cout << "CSV output file: " << csvFilePath << std::endl;
    // get bin file size
    std::filesystem::path p{binFilePath};
    uint64_t binFileSize = std::filesystem::file_size(p);
    if (CoMPASSVersion == 2)
    {
        binFileSize -= 2;
    }
    // get number of samples in a waveform
    const int sampleNumber = getSampleNumber(binFilePath, CoMPASSVersion);
    int headerSize = 24;
    int eventSize = sampleNumber *2;
    if (CoMPASSVersion == 2)
    {
        headerSize += 1;
    }
    eventSize += headerSize;
    const uint64_t totalNumberEvents = binFileSize / eventSize;
    
    // initialize buffer, size ~ 64MB
    uint64_t bufSize = 1024*1024*512 / eventSize;
    bufSize = bufSize * eventSize;
    char * buffer = new char[bufSize];
    std::vector<Event> events;

    std::ifstream binFile(binFilePath, std::ios::in | std::ios::binary);
    if (!binFile.good())
    {
        std::cerr << "Cannot open file: " << binFilePath << std::endl;
        return 1;
    }
    
    std::ofstream csvFile(csvFilePath, std::ios::out);
    if (!csvFile.good())
    {
        std::cerr << "Cannot open file: " << csvFilePath << std::endl;
        return 1;
    }

    // read from binary
    // parse
    // write to csv
    if(CoMPASSVersion == 2)
        binFile.ignore(2); // ignore the first 2 bytes of the file

    uint64_t bufIndex = 0;
    uint64_t currentNumber = 0;
    const uint64_t maxNumEvents = std::min(targetNumEvents, totalNumberEvents);
    while (!binFile.eof() && currentNumber < maxNumEvents)
    {
        // read a new chunk of data
        binFile.read(buffer, bufSize);
        // bufSize = binFile.gcount();
        // bufSize = bufSize / eventSize;
        // bufSize = bufSize * eventSize;
        bufIndex = 0;
        events.clear();

        // extract pulses
        while (bufIndex < bufSize && currentNumber < maxNumEvents)
        {
            Event newPulse(&buffer[bufIndex], eventSize);
            events.push_back(newPulse);
            currentNumber++;
            bufIndex += eventSize;
        }
        for (std::size_t i = 0; i < events.size(); i++)
        {
            csvFile << events[i] << '\n';
        }
        std::cout << currentNumber  << " events processed, " << maxNumEvents - currentNumber << " events more to process." << '\n';
    }

    binFile.close();
    csvFile.close();

    return 0;
}