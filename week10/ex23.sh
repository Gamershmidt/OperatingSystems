#!/bin/bash
chmod +x gen.sh
# Run the script to generate ex1.txt
./gen.sh 10 ex1.txt

# Link ex1.txt to ex11.txt and ex12.txt
ln ex1.txt ex11.txt
ln ex1.txt ex12.txt

# Compare the content of all files
diff ex1.txt ex11.txt
diff ex1.txt ex12.txt
#The content of the files is tha same because both files are hardlinks to the same object
# Check i-node numbers and save the output to output.txt
ls -i ex1.txt ex11.txt ex12.txt > output.txt
#The i-node for all files is the same because when we create hard link the inode for objects is the same
# Check the disk usage of ex1.txt
du -h ex1.txt

ln ex1.txt ex13.txt
mv ex13.txt /tmp/
#The difference between searching in the current path (.) and the root path (/) is in the scope of the search:

#Current Path (.): Searching in the current path (.) looks for links to ex1.txt within the current directory and its subdirectories.
#This search is limited to the current location and its descendant directories.

#Root Path (/): Searching in the root path (/) scans the entire filesystem starting from the root directory. 
#It traverses through all directories and files in the system, including all mounted partitions and external devices, searching for links to ex1.txt.
find /home/sofia/OS/week10 -inum $(ls -i ex1.txt | cut -d ' ' -f 1)
#To avoid permission denied better use sudo
find / -inum $(ls -i ex1.txt | cut -d ' ' -f 1)
ls -l ex1.txt
find /home/sofia/OS/week10 -inum $(ls -i ex1.txt | cut -d ' ' -f 1) -exec rm {} \;

