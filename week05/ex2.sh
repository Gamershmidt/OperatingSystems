#!/bin/bash

gcc -pthread ex2.c -o ex2

./ex2 $1

rm -f ex2  # Remove the executable after running

