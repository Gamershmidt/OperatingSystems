#!/bin/bash
gcc monitor.c -o monitor
./monitor /home/sofia/OS/week10 &
sleep 1
gcc ex1.c -o ex1
./ex1 /home/sofia/OS/week10
bash ex1_test.sh

