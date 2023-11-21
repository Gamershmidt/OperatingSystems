#include <stdio.h>
#include <dirent.h>

int main() {
    // Open the root directory
    DIR *root = opendir("/");

    if (root == NULL) {
        perror("opendir");
        return 1;
    }

    // Read the directory entries
    struct dirent *entry;
    while ((entry = readdir(root)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(root);

    return 0;
}

