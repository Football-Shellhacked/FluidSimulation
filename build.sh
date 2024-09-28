#!/bin/bash
cd build
./premake5 clean
./premake5 gmake2
cd ..
make