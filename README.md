# Clean Code project Args

## the git base contain two submodule for Gtest and FMT
   So need to clone with command:
   git clone --recursive xxxxx
   Or update the git project with:
   git submodule update --init --recursive

## Using CMake to build the project:
* On Linux:
```
mkdir build && cd build
cmake ..
make
```
* On Windows: Using the CMake GUI or command line cmake -G "Visual Studio 14 2015" $CMAKE_FILE_DIR