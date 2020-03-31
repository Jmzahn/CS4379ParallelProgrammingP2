#!/bin/bash
echo -e "\nTest with N: 1000 procs: 1\n"
./guass 1000 1 > ./testout/guass_1000_1_out
echo -e "\nTest with N: 1000 procs: 2\n"
./guass 1000 2 > ./testout/guass_1000_2_out
echo -e "\nTest with N: 1000 procs: 4\n"
./guass 1000 4 > ./testout/guass_1000_4_out
echo -e "\nTest with N: 1000 procs: 8\n"
./guass 1000 8 > ./testout/guass_1000_8_out
echo -e "\nTest with N: 1000 procs: 12\n"
./guass 1000 12 > ./testout/guass_1000_12_out
echo -e "\nTest with N: 2000 procs: 1\n"
./guass 2000 1 > ./testout/guass_2000_1_out
echo -e "\nTest with N: 2000 procs: 2\n"
./guass 2000 2 > ./testout/guass_2000_2_out
echo -e "\nTest with N: 2000 procs: 4\n"
./guass 2000 4 > ./testout/guass_2000_4_out
echo -e "\nTest with N: 2000 procs: 8\n"
./guass 2000 8 > ./testout/guass_2000_8_out