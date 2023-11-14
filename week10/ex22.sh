#!/bin/bash
ls -i ex1.c
#Output inode: 40905281
stat ex1.c
#This command will provide detailed information about the file, including the number of blocks, block size, total size, and permissions.
cp ex1.c ex2.c
ls -i ex2.c #inodes are not the same because inodes are unique
stat -c "%h - %n" /etc/* | grep "^3"
#The number of links represents the number of hard links to the inode.
#In a Unix-like file system, each file has at least one hard link (the directory entry)
