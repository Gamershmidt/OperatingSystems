#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>

int main() {
    /*

    int random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1) {
        perror("Error opening /dev/random");
        return 1;
    }

    char c;
    for (int i = 0; i < 4; ++i) {
        int counter = 0;
        while(1){
            ssize_t n = read(random_fd, &c, 1);
            if (n == -1) {
                perror("Error reading from /dev/random");
                close(random_fd);
                return 1;
            }
            if (isprint(c)) {
                fputc(c, file);
                counter++;
            }
            if(counter == 1024) {
                fputc('\n', file);
                break;
            }
        }

    }

    close(random_fd);*/
    FILE *file = fopen("text.txt", "w");
    if (file == NULL) {
        perror("Error creating text.txt");
        return 1;
    }
    fclose(file);
    // Open the /dev/random device
    int random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1) {
        perror("Error opening /dev/random");
        return 1;
    }

    long sz = sysconf(_SC_PAGESIZE);
    int chunk_size = sz * 1024;
    int total_capital_letters = 0;

    int text_fd = open("text.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (text_fd == -1) {
        perror("Error opening text.txt");
        close(random_fd);
        return 1;
    }

    //off_t file_size = 500 * 1024 * 1024;
    off_t file_size = 4 * 1024 * 1024;
    if (lseek(text_fd, file_size - 1, SEEK_SET) == -1) {
        perror("Error extending text.txt");
        close(random_fd);
        close(text_fd);
        return 1;
    }
    write(text_fd, "", 1);

    // Map the file into memory

    for (int j = 0; j < file_size / chunk_size; j++) {
        char *file_data = mmap(NULL, chunk_size, PROT_READ | PROT_WRITE, MAP_SHARED, text_fd, 0);
        if (file_data == MAP_FAILED) {
            perror("Error mapping text.txt into memory");
            close(random_fd);
            close(text_fd);
            return 1;
        }
        for (off_t i = 0; i < chunk_size; i++) {
            char c;
            ssize_t n = read(random_fd, &c, 1);
            if (n == -1) {
                perror("Error reading from /dev/random");
                close(random_fd);
                munmap(file_data, file_size);
                close(text_fd);
                return 1;
            }

            if (isprint(c)) {
                file_data[i] = c;
            } else {
                i--;
            }

            if ((i + 1) % 1024 == 0) {
                file_data[i + 1] = '\n';
            }
        }

        for (off_t i = 0; i < file_size; i++) {
            if (isupper(file_data[i])) {
                total_capital_letters++;
                file_data[i] = tolower(file_data[i]);
            }
        }
        munmap(file_data, chunk_size);
    }


    printf("Total capital letters: %d\n", total_capital_letters);

    close(random_fd);
    close(text_fd);

    return 0;
}
