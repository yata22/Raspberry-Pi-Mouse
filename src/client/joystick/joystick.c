//
// Created by yata on 19.02.16.
//

#define _GNU_SOURCE // It's requiered to use the version_sort() function (used by the open_evdev() function)

#include "joystick.h"
#include <stdio.h>
#include <fcntl.h>
#include <stropts.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>


int joystickInit() {
    return open_evdev("Raspberry Pi Sense HAT Joystick");
}

int handle_events(int evfd, int out_fd, size_t buffer_size, int *quit) {
    struct input_event ev[64];
    int i, rd;
    char *buffer = calloc(sizeof(char), buffer_size);
    if(buffer == NULL){
        perror("buffer");
        return 0;
    }

    rd = (int) read(evfd, ev, sizeof(struct input_event) * 64);

    if (rd < (int) sizeof(struct input_event)) {
        fprintf(stderr, "expected %d bytes, got %d\n",
                (int) sizeof(struct input_event), rd);
        return 0;
    }
    for (i = 0; i < rd / sizeof(struct input_event); i++) {
        if (ev->type != EV_KEY)
            continue;
        if (ev->value == 2)
            continue;

        switch (ev->code) {
            case KEY_ENTER:
                printf("Quit event...\n");
                // I've noticed that sometime the Joystick don't send some key release event (or there is a bug...)
                // So to avoid some undefined behaviour, we send some key release request before quitting
                snprintf(buffer, buffer_size, "event:%d:%d", KEY_LEFT, 0);
                write(out_fd, buffer, buffer_size);

                snprintf(buffer, buffer_size, "event:%d:%d", KEY_RIGHT, 0);
                write(out_fd, buffer, buffer_size);

                snprintf(buffer, buffer_size, "event:%d:%d", KEY_UP, 0);
                write(out_fd, buffer, buffer_size);

                snprintf(buffer, buffer_size, "event:%d:%d", KEY_DOWN, 0);
                write(out_fd, buffer, buffer_size);

                write(out_fd, "quit", 4);

                *quit = 1;
                free(buffer);
                return 1;
            default:
                snprintf(buffer, buffer_size, "event:%d:%d", ev->code, ev->value);
                write(out_fd, buffer, buffer_size);
                free(buffer);
                return 1;
        }
    }
    free(buffer);

    return 0;
}


int joystickQuit(int fd) {
    return close(fd);
}


int static is_event_device(const struct dirent *dir) {
    return strncmp(EVENT_DEV_NAME, dir->d_name,
                   strlen(EVENT_DEV_NAME) - 1) == 0;
}

int static open_evdev(const char *dev_name) {
    struct dirent **namelist;
    int i, ndev;
    int fd = -1;

    // Scan the input event directory
    ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
    if (ndev <= 0)
        return ndev;

    for (i = 0; i < ndev; i++) // For each input event
    {
        char fname[64];
        char name[256];

        snprintf(fname, sizeof(fname), "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
        fd = open(fname, O_RDONLY);
        if (fd < 0) {
            perror("open");
            continue;
        }

        // Get the input name
        ioctl(fd, EVIOCGNAME(sizeof(name)), name);

        // Check if it corresponds to the requested input name
        if (strcmp(dev_name, name) == 0)
            break;
        close(fd);
    }

    for (i = 0; i < ndev; i++)
        free(namelist[i]);

    return fd;
}
