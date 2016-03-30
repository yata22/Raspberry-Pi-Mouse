//
// Created by yata on 25.03.16.
//

#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include "fifo.h"
#include <stdarg.h>
#include <fcntl.h>

int createFifo(char *name) {
    char dir[64];

    if (getFifoDir(name, dir, 64) != 0) {
        return -1;
    }

    if (0 != mkfifo(dir, 0644) && errno != EEXIST) {
        perror("mkfifo()");
        return -1;
    }

    return 0;
}


int createFifos(int num, ...) {
    va_list arguments;
    va_start(arguments, num);

    int i;
    for (i = 0; i < num; i++) {
        if (createFifo(va_arg(arguments, char*)) != 0) {
            return -1;
        }
    }

    va_end(arguments);
    return 0;
}

int openFifo(char *name, int flags) {
    char dir[64];

    if (getFifoDir(name, dir, 64) != 0) {
        return -1;
    }

    int fifo = open(dir, flags);
    if (fifo == -1) {
        fprintf(stderr, "Error while opening the fifo \"%s\" with open()\n", name);
        return -1;
    }

    return fifo;
}

static int getFifoDir(char *name, char *dir, size_t length) {
    int n = snprintf(dir, length, "/tmp/RaspberryPiMouse_%s.fifo", name);

    if (n < 0 || n > length) {
        fprintf(stderr, "Invalid fifo name : [name] = %s\n", name);
        return -1;
    }

    return 0;
}