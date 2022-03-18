<!--
 * @Description: 
 * @Author: Ming Fang
 * @Date: 2021-04-06 15:24:22
 * @LastEditors: Ming Fang
 * @LastEditTime: 2022-03-18 18:07:54
-->
## Compile
- Install`cmake` and `git`.
- Clone this repository by running in your terminal 
    ```bash
    git clone https://github.com/fm140905/CoMPASSBin2CSV.git
    ```
- `cd` into the repo that you have just cloned.
- Run 
  ```bash
  mkdir -p build
  cd build
  cmake ..
  cmake --build .
  ``` 
  to compile the project.

## Run
- Under your working directory, run
    ```bash
    ./main ${path-to-your-CoMPASS-binary-data} ${max-number-events-to-process}
    ```
- Output file is saved in the same directory where the input file is, and the format is CoMPASS CSV.

## Note

- The script assumes you used CoMPASS version to acquired the binary data. If you used verion 1, change variable `CoMPASSVersion` in `main.cpp` to 1 and recompile.