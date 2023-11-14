#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAX_PATH 2048
#define BUF_LEN (10 * (sizeof(struct inotify_event) + MAX_PATH + 1))

char* path;

void print_info(char * path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char file_path[PATH_MAX];
        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

        struct stat file_stat;
        if (lstat(file_path, &file_stat) == -1) {
            perror("lstat");
            closedir(dir);
            exit(EXIT_FAILURE);
        }

        printf("Name: %s Size: %ld bytes\n", entry->d_name, file_stat.st_size);
    }

    closedir(dir);
}

void handler() {
    print_info(path);

    kill(getpid(), SIGTERM);
}

int main(int argc, char *argv[]) {
    path = malloc(19000);
    strcpy(path, argv[1]);
    print_info(path);
    signal(SIGINT, handler);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int inotify_fd, wd;
    char buf[BUF_LEN];
    ssize_t numRead;
    struct inotify_event *event;

    inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    wd = inotify_add_watch(inotify_fd, argv[1], IN_ACCESS | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN | IN_ATTRIB);
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }
    while(1){
        numRead = read(inotify_fd, buf, BUF_LEN);
        if (numRead == 0) {
            fprintf(stderr, "read() from inotify fd returned 0!");
            exit(EXIT_FAILURE);
        }

        if (numRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        for (char *p = buf; p < buf + numRead;) {

            event = (struct inotify_event *)p;
            char file_path[PATH_MAX];
            snprintf(file_path, sizeof(file_path), "%s/%s", path, event->name);

            struct stat file_stat;
            int tmp = stat(file_path, &file_stat);

            char full_path[MAX_PATH];
            bool flag = false;
            if (event->mask & IN_ACCESS) {
                printf("%s is accessed\n", event->name);
                snprintf(full_path, MAX_PATH, "%s/%s", argv[1], event->name);
                printf("path: %s\ninode: %ld\nhard-link: %lu\n\n", full_path, file_stat.st_ino, file_stat.st_nlink);
                flag = true;
            }

            if (event->mask & IN_CREATE) {
                printf("%s is created\n", event->name);
                snprintf(full_path, MAX_PATH, "%s/%s", argv[1], event->name);
                printf("path: %s\ninode: %ld\nhard-link: %lu\n\n", full_path, file_stat.st_ino, file_stat.st_nlink);
                flag = true;
            }

            if (event->mask & IN_DELETE) {
                printf("%s is deleted\n", event->name);
                snprintf(full_path, MAX_PATH, "%s/%s", argv[1], event->name);
                printf("path: %s\ninode: %ld\nhard-link: %lu\n\n", full_path, file_stat.st_ino, file_stat.st_nlink);
                flag = true;
            }

            if (event->mask & IN_MODIFY) {
                printf("%s is modified\n", event->name);
                snprintf(full_path, MAX_PATH, "%s/%s", argv[1], event->name);
                printf("path: %s\ninode: %ld\nhard-link: %lu\n\n", full_path, file_stat.st_ino, file_stat.st_nlink);
                flag = true;
            }

            if (event->mask & IN_OPEN) {
                printf("%s is opened\n", event->name);
                snprintf(full_path, MAX_PATH, "%s/%s", argv[1], event->name);
                printf("path: %s\ninode: %ld\nhard-link: %lu\n\n", full_path, file_stat.st_ino, file_stat.st_nlink);
                flag = true;
            }

            if (event->mask & IN_ATTRIB) {
                printf("%s metadata changed\n", event->name);
                snprintf(full_path, MAX_PATH, "%s/%s", argv[1], event->name);
                printf("path: %s\ninode: %ld\nhard-link: %lu\n\n", full_path, file_stat.st_ino, file_stat.st_nlink);
                flag = true;
            }



            p += sizeof(struct inotify_event) + event->len;
        }
    };
    return 0;
}
