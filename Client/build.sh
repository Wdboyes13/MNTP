#!/bin/sh

wclang++ -std=c++20 -nostdlib++ -nostdinc++ -O2 \
        -L/opt/homebrew/opt/boost/lib \
        -L/opt/will/lib/libcpp \
        -I/opt/homebrew/opt/boost/include \
        -I/opt/will/include/c++/v1 \
        Main.cpp \
        -o MNTP-Client \
        -lc++ -lc++abi -lboost_system \
        -Wl,-rpath,/opt/will/lib/libcpp