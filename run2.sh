#!/bin/sh
g++ -std=c++11 decode_sat.cpp -o ds
./ds $1
