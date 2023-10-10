#!/bin/bash

gcc -pthread ex3.c -o ex3

n=10000000
m_values=(1 2 4 10 100)

rm -f ex3_res.txt

for m in "${m_values[@]}"; do
    echo "Running with n=$n and m=$m"
    (time ./ex3 $n $m) 2>&1 | grep real | awk '{print $2}' >> ex3_res.txt
    
done

(./ex3 $n 100) >> ex3_res.txt

rm -f ex3  

