#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
  
    int id = atoi(argv[1]);
    int fd;
    char buffer[1024];
    char path[100];
    snprintf(path, 100, "/tmp/ex1/s%d", id);
   
    fd = open(path, O_RDONLY);
    if (fd == -1) {
      perror("open sub");
      exit(EXIT_FAILURE);
    }
    read(fd, buffer, sizeof(buffer));
    printf("Message received: %s", buffer);
    close(fd);

    return 0;
}

