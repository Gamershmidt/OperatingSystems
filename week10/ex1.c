#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
char *path;

void find_all_hlinks(const char* source) {
    struct stat source_stat;
    if (lstat(source, &source_stat) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    int kolvo = 0;
    while ((entry = readdir(dir)) != NULL) {
        char file_path[PATH_MAX];
        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

        struct stat file_stat;
        if (lstat(file_path, &file_stat) == -1) {
            perror("lstat");
            continue;
        }

        if (S_ISREG(file_stat.st_mode) && file_stat.st_ino == source_stat.st_ino) {
            char abs_path[PATH_MAX];
            if (realpath(file_path, abs_path) == NULL) {
                perror("realpath");
                continue;
            }
            kolvo++;
            printf("Inode: %ld, Path: %s, content: \n", file_stat.st_ino, abs_path);
            FILE* file = fopen(entry->d_name, "r");
            while (!feof(file)) {
                char c = fgetc(file);
                printf("%c", c);
            }
            fclose(file);
        }
    }
    printf("Number of hard links: %d\n\n", kolvo);
    closedir(dir);
}
void unlink_all(const char* source) {
    printf("FFFFF\n\n");
    struct stat source_stat;
    if (lstat(source, &source_stat) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    ino_t last_inode = 0;
    char last_hard_link_path[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        char file_path[PATH_MAX];
        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

        struct stat file_stat;
        if (lstat(file_path, &file_stat) == -1) {
            perror("lstat");
            continue;
        }

        if (S_ISREG(file_stat.st_mode) && file_stat.st_ino == source_stat.st_ino) {
            if (last_inode != 0 && last_inode != file_stat.st_ino) {
                if (unlink(last_hard_link_path) == -1) {
                    perror("unlink");
                    closedir(dir);
                    exit(EXIT_FAILURE);
                }
            }

            last_inode = file_stat.st_ino;
            realpath(file_path, last_hard_link_path);
        }
    }

    closedir(dir);

    if (last_inode != 0) {
        printf("Last hard link: %s\n", last_hard_link_path);
    } else {
        printf("No hard links found for %s\n", source);
    }

}
void create_sym_link(const char* source, const char* link) {
    char link_path[2*PATH_MAX];
    snprintf(link_path, sizeof(link_path), "%s/%s", path, link);

    if (symlink(source, link_path) == -1) {
        perror("symlink");
        exit(EXIT_FAILURE);
    }

    printf("Symbolic link created: %s -> %s\n", link_path, source);
}
int main(int argc, char *argv[]) {
    path = malloc(PATH_MAX*sizeof(char ));
    strcpy(path, argv[1]);
    FILE *file = fopen("myfile1.txt", "w");
    if (file) {
        fprintf(file, "Hello world.\n");
        fclose(file);
        link("myfile1.txt", "myfile11.txt");
        link("myfile1.txt", "myfile12.txt");
    } else {
        perror("fopen");
        return EXIT_FAILURE;
    }
    find_all_hlinks("myfile1.txt");

    if (rename("myfile1.txt", "/tmp/myfile1.txt") != 0) {
        perror("rename");
        return EXIT_FAILURE;
    }

    file = fopen("myfile11.txt", "w");
    if (file) {
        fprintf(file, "Modified content.\n");
        fclose(file);
    } else {
        perror("fopen");
        return EXIT_FAILURE;
    }
    //printf("FFFFF\n\n");
    create_sym_link("/tmp/myfile1.txt", "myfile13.txt");

    file = fopen("/tmp/myfile1.txt", "w");
    if (file) {
        fprintf(file, "Modified content in /tmp.\n");
        fclose(file);
    } else {
        perror("fopen");
    }

    unlink_all("/tmp/myfile1.txt");
    printf("Stat info\n");
    find_all_hlinks("/tmp/myfile1.txt");
    return EXIT_SUCCESS;

}