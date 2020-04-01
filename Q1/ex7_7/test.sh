#!/bin/bash
echo -e "\nTest with 1 proc\n"
./ex7_7 1 > ./testout/ex7_7_1_out
echo -e "\nTest with 2 proc\n"
./ex7_7 2 > ./testout/ex7_7_2_out
echo -e "\nTest with 4 proc\n"
./ex7_7 4 > ./testout/ex7_7_4_out
echo -e "\nTest with 8 proc\n"
./ex7_7 8 > ./testout/ex7_7_8_out
