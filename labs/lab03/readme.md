# Compile and run commands

gcc -g lcp3_bm.c -o lcp.out -Wall
./lcp.out < data/test_e.txt > out/nbug_t_e_01.txt
meld out/test_e.txt out/nbug_t_e_01.txt
