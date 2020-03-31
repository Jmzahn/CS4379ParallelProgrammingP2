#!/bin/bash
echo -e "\nTest with N: 1000 procs: 1\n"
./guass 1000 1 > guass_1000_1_out
echo -e "\nTest with N: 1000 procs: 2\n"
./guass 1000 2 > guass_1000_2_out
echo -e "\nTest with N: 1000 procs: 4\n"
./guass 1000 4 > guass_1000_4_out
echo -e "\nTest with N: 1000 procs: 8\n"
./guass 1000 8 > guass_1000_8_out
echo -e "\nTest with N: 1000 procs: 12\n"
./guass 1000 12 > guass_1000_12_out
echo -e "\nTest with N: 2000 procs: 12\n"
./guass 2000 12 > guass_2000_12_out