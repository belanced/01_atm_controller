# ATM Controller
This is a project for ATM Controller. You may reproduce the my result via following commands below.

## Prerequisite
This project is written on cpp standard 17. Please make sure you have installed g++>=9, cmake>=3.16 and googletest==1.15.2. 

``` bash
# install basic dependencies in debian/ubuntu
sudo apt install -y cmake build-essential git
```
``` bash
# install googletest
git clone https://github.com/google/googletest/tree/main/googletest -b v1.15.2
cd googletest 
mkdir build && cd build
cmake ..
make -j
sudo make install
```

## Build
Follow the commands below to build the project. Cmake should find googletest package.

``` bash
cd $PROJ_DIR # where PROJ_DIR=/path/to/01_atm_controller
mkdir build && cd build
cmake .. && make -j
```

## Test
Follow the commands to run the unit tests.
``` bash
cd $PROJ_DIR/build # where RPOJ_DIR=/path/to/01_atm_controller

ctest -V # 1. Just test
ctest -V | tee test.log  # 2. Test and log as file
```

You may find the exemplar log in `test.log`.

## Remarks
* This project was tested on Ubuntu 20.04. But I believe it should work on any machine with g++>=9. 