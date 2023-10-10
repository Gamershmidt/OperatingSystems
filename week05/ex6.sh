#!/bin/bash

gcc -pthread ex6.c -o ex6

n=10000000
m_values=(1 2 4 10 100)

rm -f ex6_res.txt

for m in "${m_values[@]}"; do
    echo "Running with n=$n and m=$m"
    (time ./ex6 $n $m) 2>&1 | grep real | awk '{print $2}' >> ex6_res.txt
done

rm -f ex6  # Remove the executable after running

