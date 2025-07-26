#!/bin/sh

wclang++ -std=c++20 -O2 \
        -L/opt/homebrew/opt/boost/lib \
        -L../tzdb-2025b \
        -I/opt/homebrew/opt/boost/include \
        -I../date/include \
        -I/opt/will/include/c++/v1 \
        Main.cpp ../date/src/tz.cpp \
        -o MNTP-Server \
        -ltz -lboost_system -lcurl \
        -wno-deprecated-literal-operator