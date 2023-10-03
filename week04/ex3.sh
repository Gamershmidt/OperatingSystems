#!/bin/bash
gcc ex3.c -o ex3
./ex3 3 &
temp=`pgrep gnome-terminal`
sleep 14
pstree -c $temp
./ex3 5 &
temp=`pgrep gnome-terminal`
sleep 24
pstree -c $temp
