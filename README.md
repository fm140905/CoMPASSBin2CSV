<!--
 * @Description: 
 * @Author: Ming Fang
 * @Date: 2021-04-06 15:24:22
 * @LastEditors: Ming Fang
 * @LastEditTime: 2022-08-22 22:01:52
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
    ./main ${path-to-your-CoMPASS-binary-data} ${max-number-events-to-process} ${CoMPASS-version}
    ```
- If `CoMPASS-vesion` is not provided, it is set to 2 by default.
- Output file is saved in the same directory where the input file is, and the format is CoMPASS CSV.
