#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
void handler(int signum) {
    exit(0);
}
int main() {
    FILE* pid_file = fopen("/tmp/ex1.pid", "w");
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);
    signal(SIGKILL, handler);
    int random_fd = open("/dev/random", O_RDONLY);
    //printf("pass:");
    if (random_fd == -1) {
        perror("Error opening /dev/random");
        return 1;
    }
    char password[16];
    strcpy(password, "pass:");
    for (int i = 0; i < 8; i++) {
        char c;
        ssize_t n = read(random_fd, &c, 1);
        if (isprint(c)) {
            password[i+5] = c;
        } else {
            i--;
        }
    }
    char *password_mem = (char *)mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (password_mem == MAP_FAILED) {
        perror("Error mapping memory");
        return 1;
    }
    strcpy(password_mem, password);
    int i=0;

    while (1){
    };
    return 0;
}
