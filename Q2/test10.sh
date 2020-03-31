#!/bin/bash
echo -e "\nTest with N: 2 procs: 1\n"
./guass 2 1 > ./test10/guass_2_1_out
echo -e "\nTest with N: 2 procs: 2\n"
./guass 2 2 > ./test10/guass_2_2_out
echo -e "\nTest with N: 4 procs: 1\n"
./guass 4 1 > ./test10/guass_4_1_out
echo -e "\nTest with N: 4 procs: 2\n"
./guass 4 2 > ./test10/guass_4_2_out
echo -e "\nTest with N: 10 procs: 1\n"
./guass 10 1 > ./test10/guass_10_1_out
echo -e "\nTest with N: 10 procs: 2\n"
./guass 10 2 > ./test10/guass_10_2_out
echo -e "\nTest with N: 10 procs: 4\n"
./guass 10 4 > ./test10/guass_10_4_out
