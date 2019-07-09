# Virtual Prototyping CMake projects

## SystemC and SystemC-AMS
- SystemC Download: [SystemC](http://www.accellera.org/downloads/standards/systemc)
- SystemC-AMS Download: [SystemC-AMS](https://www.coseda-tech.com/systemc-ams-proof-of-concept)

    - choose a directory to extract the archives to
    - `cd` into systemc folder
    - `./configure`
    - `make`
    - `make install`
    - `cd` into systemc-ams folder
    - `./configure --with-layout=unix CXXFLAGS="-DSC_INCLUDE_EXTRA_STD_HEADERS" --with-systemc=/path/to/systemc/folder` (for some systems `./configure --with-systemc=/path/to/systemc/folder` is enough)
    - `make`
    - `make install`

## Setup environment variables for cmake

- Add to `.*rc` file of the shell you are using (e.g `.bashrc`) in your home:

    - `export SYSTEMC=/directory/of/systemc`
    - `export SYSTEMCAMS=/directory/of/systemc-ams`
    - `export LD_LIBRARY_PATH=$SYSTEMC/lib-linux64:$SYSTEMC/lib:$SYSTEMC/lib64:$SYSTEMCAMS/lib-linux64`

## New Project

- Use `./createProject.sh  projectName` to create a new project folder with all files from the blank project.


## Compile
- `cd` to your project base folder (with `CMakeLists.txt`):
- `cmake .`
    - if compilation in the next step fails dues to some weird error (like errors with `std::...gets`) ,you can try different CXXFLAGS with
        - `./cmakeVoid.sh`
        - `./cmakeNormalClang.sh`
        - `./cmakeVoidClang.sh`
- `make`
