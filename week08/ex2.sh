#!/bin/bash
gcc pager.c -o pager
gcc mmu.c -o mmu
sleep 1
./pager 4 2 &
pid=$!
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pid &
sleep 10
jobs
