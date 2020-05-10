#!/bin/bash
echo -e "\nTest with N: 2000 procs: 1\n"
./gauss 2000 1 > ./testout/gauss_2000_1_out
echo -e "\nTest with N: 2000 procs: 2\n"
./gauss 2000 2 > ./testout/gauss_2000_2_out
echo -e "\nTest with N: 2000 procs: 4\n"
./gauss 2000 4 > ./testout/gauss_2000_4_out
echo -e "\nTest with N: 2000 procs: 8\n"
./gauss 2000 8 > ./testout/gauss_2000_8_out