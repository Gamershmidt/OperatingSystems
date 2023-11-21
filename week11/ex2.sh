#!/bin/bash
#!/bin/bash

# Compile create_fs.c to create the file system
gcc create_fs.c -o create_fs

# Create the file system
./create_fs disk0

# Compile ex2.c to create the file system simulation
gcc ex2.c -o ex2

# Process input from input.txt
./ex2 input.txt

# Cleanup compiled files if needed
# rm create_fs ex2
