#!/bin/bash
echo -e "\nFor seeing values of input/output\n"
echo -e "\nTest with N: 2 procs: 1\n"
./gauss 2 1 > ./test10out/gauss_2_1_out
echo -e "\nTest with N: 2 procs: 2\n"
./gauss 2 2 > ./test10out/gauss_2_2_out
echo -e "\nTest with N: 4 procs: 1\n"
./gauss 4 1 > ./test10out/gauss_4_1_out
echo -e "\nTest with N: 4 procs: 2\n"
./gauss 4 2 > ./test10out/gauss_4_2_out
echo -e "\nTest with N: 10 procs: 1\n"
./gauss 10 1 > ./test10out/gauss_10_1_out
echo -e "\nTest with N: 10 procs: 2\n"
./gauss 10 2 > ./test10out/gauss_10_2_out