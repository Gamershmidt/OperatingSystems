#!/bin/bash
rm -rf ./tmp  # Delete ./tmp folder if it exists
ln -s ./tmp tmp1  # Create a symbolic link tmp1 for ./tmp
ls -li
mkdir ./tmp
ls -li
# The difference that before was only link tmp1
# After adding tmp there are two lines: info about tmp and link
bash gen.sh 10 ./tmp/ex1.txt
ls -l ./tmp1
ln -s $(realpath ./tmp) ./tmp1/tmp2
bash gen.sh 10 ./tmp1/tmp2/ex1.txt
ls -l ./tmp1
ls -l ./tmp1/tmp2
ls ./tmp1/tmp2/tmp2/tmp2/
#infinitely many folders  and subfolders that are the same, and have the same structure subfolder + file
rm -rf ./tmp
ls -l tmp1
ls -l tmp1/tmp2
rm tmp1  # Remove symbolic link tmp1
rm tmp1/tmp2  # Remove symbolic link tmp1/tmp2
