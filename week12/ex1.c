#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define EVENT_SIZE sizeof(struct input_event)

int main() {

    const char *device_file = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";

    // Open the device file with read-only permissions, containing info about input events
    int fd = open(device_file, O_RDONLY);
    if (fd == -1) {
        perror("Error opening device file");
        return EXIT_FAILURE;
    }
    FILE *file = fopen("ex1.txt", "w");
  // printing avaliable shortcuts
    printf("Avaliable shortcuts:\nP+E -> I passed the Exam!\n"
           "C+A+P -> Get some cappuccino!\nE+X -> program termination\n"
           "M+E -> Hi! My name is Sofia :)\n");
    fprintf(file, "Avaliable shortcuts:\nP+E -> I passed the Exam!\n"
                  "C+A+P -> Get some cappuccino!\nE+X -> program termination\n"
                  "M+E -> Hi! My name is Sofia :)\n");
    struct input_event ev;
    int event_type;
    int event_code;

    printf("Listening for keyboard events...\n");
    fprintf(file, "Listening for keyboard events...\n");
    // array that will contain codes of keyboard events
    int press[5] = {0};
    while (1) {
      //read event from file
        if (read(fd, &ev, EVENT_SIZE) != EVENT_SIZE) {
            perror("Error reading");
            return EXIT_FAILURE;
        }

        // Handle only PRESSED(ev.value = 0), REPEATED(ev.value = 2), and RELEASED(ev.value = 1) events
        if (ev.type == EV_KEY && (ev.value == 0 || ev.value == 1 || ev.value == 2)) {
            event_type = ev.value;
            event_code = ev.code;
          //update array with new keyboard event
            press[0] = press[1];
            press[1] = press[2];
            press[2] = press[3];
            press[3] = press[4];
            press[4] = event_code;
          //print info in needed format
            printf("%s 0x%04x (%d)\n",
                   (event_type == 0 ? "RELEASED" : (event_type == 1 ? "PRESSED" : "REPEATED")),
                   event_code, event_code);
            fprintf(file, "%s 0x%04x (%d)\n",
                    (event_type == 0 ? "RELEASED" : (event_type == 1 ? "PRESSED" : "REPEATED")),
                    event_code, event_code);
          //if last two events are pressing e and x then terminate 
          // event_type == 1 are here to be sure that shortcat executed when all buttons pressed
            if ((press[3] == KEY_E && press[4] == KEY_X) || (press[4] == KEY_E && press[3] == KEY_X) && event_type == 1) {
                printf("Terminating program...\n");
                fprintf(file, "Terminating program...\n");
                close(fd);
                return EXIT_SUCCESS;
            }
          // the same approach as for the E + X
            if (press[4] == KEY_E && press[3] == KEY_P && event_type == 1) {
                printf("\n\nI passed the Exam!\n\n");
                fprintf(file,"\n\nI passed the Exam!\n\n");
            }
          // the same approach as for the E + X
            if (press[2] == KEY_C && press[3] == KEY_A && press[4] == KEY_P && event_type == 1) {
                printf("\n\nGet some cappuccino!\n\n");
                fprintf(file,"\n\nGet some cappuccino!\n\n");
            }
          // the same approach as for the E + X
            if (press[3] == KEY_M && press[4] == KEY_E && event_type == 1) {
                printf("\n\nHi! My name is Sofia :) \n\n");
                fprintf(file, "\n\nHi! My name is Sofia :) \n\n");
            }
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}
