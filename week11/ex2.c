#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define BLOCK_SIZE 128
const int SUPER_BLOCK_SIZE = 1024;
const int INODE_SIZE = 56;
const int MAX_FILES = 16;
const int MAX_FILE_BLOCKS = 8;
FILE *disk;
struct inode{
    char name[16];
    int size;
    int blockPointers[8];
    int used;
};
int create(char name[16], int size){
    // Check to see if we have sufficient free space on disk by reading in the free block list
    char freeBlockList[BLOCK_SIZE];
    fseek(disk, 0, SEEK_SET);
    fread(freeBlockList, sizeof(char), BLOCK_SIZE, disk);

    // Scan the free block list to ensure enough space for the new file
    int freeSpaceCount = 0;
    for (int i = 1; i < BLOCK_SIZE; i++) { // Start from index 1 (excluding super block)
        if (freeBlockList[i] == 0)
            freeSpaceCount++;
    }

    if (freeSpaceCount < size) {
        printf("Error: Insufficient space to create file.\n");
        return -1; // Return error code
    }

    // Look for a free inode on disk
    struct inode tempInode;
    int inodeIndex = -1;
    for (int i = 0; i < MAX_FILES; i++) {
        fseek(disk, SUPER_BLOCK_SIZE + i * INODE_SIZE, SEEK_SET);
        fread(&tempInode, sizeof(struct inode), 1, disk);
        if (tempInode.used == 0) {
            inodeIndex = i;
            break;
        }
    }

    if (inodeIndex == -1) {
        printf("Error: No free inode available.\n");
        return -1; // Return error code
    }

    // Set the found inode as used
    fseek(disk, SUPER_BLOCK_SIZE + inodeIndex * INODE_SIZE, SEEK_SET);
    strcpy(tempInode.name, name);
    tempInode.size = size;
    tempInode.used = 1;

    // Allocate data blocks to the file
    for (int i = 0; i < size; i++) {
        int blockIndex = -1;
        for (int j = 1; j < BLOCK_SIZE; j++) { // Start from index 1 (excluding super block)
            if (freeBlockList[j] == 0) {
                blockIndex = j;
                freeBlockList[j] = 1; // Mark block as used
                tempInode.blockPointers[i] = blockIndex;
                break;
            }
        }

        if (blockIndex == -1) {
            printf("Error: Failed to allocate blocks for the file.\n");
            // Rollback changes by freeing previously allocated blocks
            for (int k = 0; k < i; k++) {
                freeBlockList[tempInode.blockPointers[k]] = 0;
            }
            tempInode.used = 0; // Mark inode as free
            return -1; // Return error code
        }
    }

    // Write updated free block list and inode to disk
    fseek(disk, 0, SEEK_SET);
    fwrite(freeBlockList, sizeof(char), BLOCK_SIZE, disk);

    fseek(disk, SUPER_BLOCK_SIZE + inodeIndex * INODE_SIZE, SEEK_SET);
    fwrite(&tempInode, sizeof(struct inode), 1, disk);

    return 0; // Return success

} // End Create



int delete(char name[16]){
    struct inode tempInode;
    int found = 0;

    // Locate the inode for the specified file name
    int i;
    for (i = 0; i < MAX_FILES; i++) {
        fseek(disk, SUPER_BLOCK_SIZE + i * INODE_SIZE, SEEK_SET);
        fread(&tempInode, sizeof(struct inode), 1, disk);
        if (tempInode.used == 1 && strcmp(tempInode.name, name) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Error: File '%s' not found.\n", name);
        return -1; // Return error code
    }

    // Free blocks of the file being deleted
    char freeBlockList[BLOCK_SIZE];
    fseek(disk, 0, SEEK_SET);
    fread(freeBlockList, sizeof(char), BLOCK_SIZE, disk);

    for (int j = 0; j < tempInode.size; j++) {
        freeBlockList[tempInode.blockPointers[j]] = 0; // Free the block
    }

    // Mark inode as free
    tempInode.used = 0;

    // Write updated free block list and inode to disk
    fseek(disk, 0, SEEK_SET);
    fwrite(freeBlockList, sizeof(char), BLOCK_SIZE, disk);

    fseek(disk, SUPER_BLOCK_SIZE + i * INODE_SIZE, SEEK_SET);
    fwrite(&tempInode, sizeof(struct inode), 1, disk);

    return 0; // Return success

} // End Delete


int ls(void){
    // List names of all files on disk

    // Step 1: read in each inode and print!
    // Move file pointer to the position of the 1st inode (129th byte)
    // for(i=0;i<16;i++)
    // REad in a inode
    // If the inode is in-use
    // print the "name" and "size" fields from the inode
    // end for
    struct inode tempInode;

    printf("Files in the file system:\n");

    // Read each inode and print the names and sizes of used inodes
    for (int i = 0; i < MAX_FILES; i++) {
        fseek(disk, SUPER_BLOCK_SIZE + i * INODE_SIZE, SEEK_SET);
        fread(&tempInode, sizeof(struct inode), 1, disk);
        if (tempInode.used == 1) {
            printf("Name: %s\tSize: %d blocks\n", tempInode.name, tempInode.size);
        }
    }

    return 0; // Return success

} // End ls

int reads(char name[16], int blockNum, char buf[1024]){

    struct inode tempInode;
    int found = 0;

    // Locate the inode for the specified file name
    for (int i = 0; i < MAX_FILES; i++) {
        fseek(disk, SUPER_BLOCK_SIZE + i * INODE_SIZE, SEEK_SET);
        fread(&tempInode, sizeof(struct inode), 1, disk);
        if (tempInode.used == 1 && strcmp(tempInode.name, name) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Error: File '%s' not found.\n", name);
        return -1; // Return error code
    }

    // Check if blockNum is within the file size
    if (blockNum >= tempInode.size) {
        printf("Error: Block number out of bounds for file '%s'.\n", name);
        return -1; // Return error code
    }

    // Get the disk address of the specified block
    int blockAddr = tempInode.blockPointers[blockNum];

    // Move the file pointer to the block location
    fseek(disk, blockAddr * BLOCK_SIZE, SEEK_SET);

    // Read the block into the buffer
    fread(buf, sizeof(char), BLOCK_SIZE, disk);

    return 0; // Return success

} // End read


int writes(char name[16], int blockNum, char buf[1024]){

    struct inode tempInode;
    int found = 0;

    // Locate the inode for the specified file name
    for (int i = 0; i < MAX_FILES; i++) {
        fseek(disk, SUPER_BLOCK_SIZE + i * INODE_SIZE, SEEK_SET);
        fread(&tempInode, sizeof(struct inode), 1, disk);
        if (tempInode.used == 1 && strcmp(tempInode.name, name) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Error: File '%s' not found.\n", name);
        return -1; // Return error code
    }

    // Check if blockNum is within the file size
    if (blockNum >= tempInode.size) {
        printf("Error: Block number out of bounds for file '%s'.\n", name);
        return -1; // Return error code
    }

    // Get the disk address of the specified block
    int blockAddr = tempInode.blockPointers[blockNum];

    // Move the file pointer to the block location
    fseek(disk, blockAddr * BLOCK_SIZE, SEEK_SET);

    // Write the block from the buffer to disk
    fwrite(buf, sizeof(char), BLOCK_SIZE, disk);

    return 0; // Return success


} // end write



int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <inputFileName>\n", argv[0]);
        exit(1);
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        exit(1);
    }

    char diskFileName[256]; // Assuming max disk file name length is 255 characters
    char command[20];
    char fileName[16];
    int size, blockNum;
    char buffer[1024];

    // Read disk file name from input
    fscanf(inputFile, "%s", diskFileName);

    // Open the disk file
    disk = fopen(diskFileName, "r+");
    if (disk == NULL) {
        perror("Error opening disk file");
        exit(1);
    }

    // Process commands from the input file
    while (fscanf(inputFile, "%s", command) != EOF) {
        if (strcmp(command, "C") == 0) {
            fscanf(inputFile, "%s %d", fileName, &size);
            printf("Creating file '%s' of size %d blocks.\n", fileName, size);
            int result = create(fileName, size);
            if (result == 0) {
                printf("File '%s' created successfully.\n", fileName);
            } else {
                printf("Failed to create file '%s'.\n", fileName);
            }
        } else if (strcmp(command, "D") == 0) {
            fscanf(inputFile, "%s", fileName);
            printf("Deleting file '%s'.\n", fileName);
            int result = delete(fileName);
            if (result == 0) {
                printf("File '%s' deleted successfully.\n", fileName);
            } else {
                printf("Failed to delete file '%s'.\n", fileName);
            }
        } else if (strcmp(command, "L") == 0) {
            printf("Listing all files on disk and their sizes.\n");
            ls();
        } else if (strcmp(command, "R") == 0) {
            fscanf(inputFile, "%s %d", fileName, &blockNum);
            printf("Reading block %d from file '%s'.\n", blockNum, fileName);
            int result = reads(fileName, blockNum, buffer);
            if (result == 0) {
                printf("Block %d read successfully from file '%s'.\n", blockNum, fileName);
                // Process buffer content if needed
            } else {
                printf("Failed to read block %d from file '%s'.\n", blockNum, fileName);
            }
        } else if (strcmp(command, "W") == 0) {
            fscanf(inputFile, "%s %d", fileName, &blockNum);
            printf("Writing to block %d in file '%s'.\n", blockNum, fileName);
            // Fill buffer with data to write (if needed)
            // Example: sprintf(buffer, "Data to write to block %d in file '%s'.", blockNum, fileName);
            int result = writes(fileName, blockNum, buffer);
            if (result == 0) {
                printf("Data written successfully to block %d in file '%s'.\n", blockNum, fileName);
            } else {
                printf("Failed to write data to block %d in file '%s'.\n", blockNum, fileName);
            }
        } else {
            printf("Invalid command.\n");
        }
    }

    fclose(inputFile);
    fclose(disk); // Close the disk file pointer when finished
    return 0;


}