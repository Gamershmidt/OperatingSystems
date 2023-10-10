#!/bin/bash

gcc -pthread ex4.c -o ex4

n=10000000
m_values=(1 2 4 10 100)

rm -f ex4_res.txt

for m in "${m_values[@]}"; do
    echo "Running with n=$n and m=$m"
    (time ./ex4 $n $m) 2>&1 | grep real | awk '{print $2}' >> ex4_res.txt
done

rm -f ex4  

