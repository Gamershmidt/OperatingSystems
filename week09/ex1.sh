#!/bin/bash
gcc pager.c -o pager
gcc mmu.c -o mmu
sleep 1
./pager 5 2 aging &
pid=$!
./mmu 5 R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 W1 R2 R0 W4 R2 R0 W2 R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4 R2 R3 R1 R0 R2 W4 R0 $pid &
sleep 20
