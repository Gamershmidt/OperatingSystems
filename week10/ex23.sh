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

# Check i-node numbers and save the output to output.txt
ls -i ex1.txt ex11.txt ex12.txt > output.txt

# Check the disk usage of ex1.txt
du -h ex1.txt

ln ex1.txt ex13.txt
mv ex13.txt /tmp/
find /home/sofia/OS/week10 -inum $(ls -i ex1.txt | cut -d ' ' -f 1)
find / -inum $(ls -i ex1.txt | cut -d ' ' -f 1)
ls -l ex1.txt
find /home/sofia/OS/week10 -inum $(ls -i ex1.txt | cut -d ' ' -f 1) -exec rm {} \;

