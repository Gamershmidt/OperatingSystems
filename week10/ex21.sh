#!/bin/bash
#Filetypes in etc and dev directories
file /dev/*
file /etc/*
#number of directories in the folder /etc
ls -l /etc | grep ^d | wc -l
file ex1.c
gcc ex1.c -o ex1
file ex1.c
#Change english output to russian
file ex1.c
gcc ex1.c -o ex1
file ex1.c
# Output for ex1.c before: ex1.c: C source, ASCII text
# after changes: ex1.c: C source, Unicode text, UTF-8 text
# Difference is in the encoding, previously symbols belonged to ASCII, but russian symbols belong to UTF-8