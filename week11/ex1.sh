#!/bin/bash
dd if=/dev/zero of=lofs.img bs=1M count=50
#This dd command writes zeroes (avaliable loop device) to the file lofs.img using a block size of 1 megabyte (1M) and creates a file with a size of 50 MB by setting the count to 50.
loop_dev=$(losetup -f)
sudo losetup $loop_dev lofs.img
#This command associates the loop device with the file lofs.img, essentially mounting the file as a loop device.
#Without sudo permisson will be denied
sudo mkfs.ext4 $loop_dev
#This command creates an Ext4 file system on the loop device.
mkdir ./lofsdisk
#make the direcrory lofsdisk
sudo chown $USER ./lofsdisk
#Creates a directory named lofsdisk and adjusts its ownership. Also, changes the ownership of the loop device.
sudo chown $USER:disk $loop_dev
#Creates a directory named lofsdisk and adjusts its ownership. Also, changes the ownership of the loop device.

# Mount the loop device to the lofsdisk directory
sudo mount $loop_dev ./lofsdisk
#Create file1 and file2 and fill it with name and surname
sudo sh -c 'echo "Sofia" > ./lofsdisk/file1'
sudo sh -c 'echo "Gamershmidt" > ./lofsdisk/file2'
# Function to get paths of shared libraries used by a binary
get_libs() {
    #first argument
    binary_file=$1
    #expression to extract libraries
    libs=$(find $binary_file -type f -perm /a+x -exec ldd {} \; \
        | grep so \
        | sed -e '/^[^\t]/ d' \
        | sed -e 's/\t//' \
        | sed -e 's/.*=..//' \
        | sed -e 's/ (0.*)//' 
        )
    #return libraries
    echo "$libs"
}
# Get shared libraries for specified commands and copy them to LOFS
commands=("bash" "cat" "echo" "ls")
# go through list of commands
for cmd in "${commands[@]}"; do
    #get path to command
    cmd_path=$(which "$cmd")
    #get libraries via get_libs function
    libraries=$(get_libs "$cmd_path")
    if [ $? -eq 0 ]; then
        echo "Shared libraries for $cmd $cmd_path:"
        echo "$libraries"

        # Copy the command binary and its shared libraries to LOFS
        sudo cp --parents "$cmd_path" ./lofsdisk/
        sudo cp --parents $libraries ./lofsdisk/
    fi
done
#compile ex1.c
gcc ex1.c -o ex1
# Run ex1 within the chrooted environment and save output to ex1.txt
sudo cp ./ex1 ./lofsdisk/
sudo chroot ./lofsdisk ./ex1 > ex1.txt
# Use chroot to change the apparent root directory to lofsdisk and executes the ex1 binary, redirecting its output to ex1.txt.

