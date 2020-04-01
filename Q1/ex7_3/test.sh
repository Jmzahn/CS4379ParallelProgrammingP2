#!/bin/bash
echo -e "\nTest with 1 proc\n"
./ex7_3 1 > ./testout/ex7_3_1_out
echo -e "\nTest with 2 proc\n"
./ex7_3 2 > ./testout/ex7_3_2_out
echo -e "\nTest with 4 proc\n"
./ex7_3 4 > ./testout/ex7_3_4_out
echo -e "\nTest with 8 proc\n"
./ex7_3 8 > ./testout/ex7_3_8_out
