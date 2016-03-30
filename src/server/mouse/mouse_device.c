//
// Created by yata on 12.02.16.
//

#include "mouse_device.h"
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


int createDevice() {
    int fd;

    // Open the uinput FD
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("open /dev/uinput");
        return -1;
    }

    /* Configure the input Device */

    // Key event
    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0) {
        perror("ioctl");
        return -1;
    }
    // Volume up key
    if (ioctl(fd, UI_SET_KEYBIT, KEY_VOLUMEUP) < 0) {
        perror("ioctl");
        return -1;
    }
    // Volume down key
    if (ioctl(fd, UI_SET_KEYBIT, KEY_VOLUMEDOWN) < 0) {
        perror("ioctl");
        return -1;
    }
    // Left click
    if (ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0) {
        perror("ioctl");
        return -1;
    }
    // Right click
    if (ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT) < 0) {
        perror("ioctl");
        return -1;
    }
    // Relative mouse axis move
    if (ioctl(fd, UI_SET_EVBIT, EV_REL) < 0) {
        perror("ioctl");
        return -1;
    }
    // Relative mouse X axis move
    if (ioctl(fd, UI_SET_RELBIT, REL_X) < 0) {
        perror("ioctl");
        return -1;
    }
    // Relative mouse Y axis move
    if (ioctl(fd, UI_SET_RELBIT, REL_Y) < 0) {
        perror("ioctl");
        return -1;
    }

    // Create uinput_user_dev structure (defined in uinput.h)
    struct uinput_user_dev uidev;

    memset(&uidev, 0, sizeof(uidev));

    // Fill the structure
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "raspberry-pi-mouse");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    // Finish the configuration by writing the structure in the uinput file descriptor
    if (write(fd, &uidev, sizeof(uidev)) < 0) {
        perror("write");
        return -1;
    }

    // Create the device via the UI_DEV_CREATE ioctl request on the file descriptor
    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        perror("ioctl");
        return -1;
    }

    // Device created

    // Sleep 1 second
    // I don't really know why is this important
    // But I guess we need to wait that the OS (the kernel ?) read the data and create the device
    sleep(1);

    return fd;
}

int moveMouse(int fd, int x, int y) {
    if (fd < 0) {
        return -1;
    }

    struct input_event ev[3];

    memset(ev, 0, sizeof(ev));

    ev[0].type = EV_REL;
    ev[0].code = REL_X;
    ev[0].value = x;
    ev[1].type = EV_REL;
    ev[1].code = REL_Y;
    ev[1].value = y;
    ev[2].type = EV_SYN;
    ev[2].code = SYN_REPORT;

    if (write(fd, ev, sizeof(ev)) < 0) {
        perror("write");
        return -1;
    }

    return 0;
}


int clickMouse(int fd, __u16 keyCode, __s32 value) {
    if (fd < 0) {
        return -1;
    }

    struct input_event ev[2];

    memset(ev, 0, sizeof(ev));


    ev[0].type = EV_KEY;
    ev[0].code = keyCode;
    ev[0].value = value;
    ev[1].type = EV_SYN;
    ev[1].code = SYN_REPORT;

    if (write(fd, &ev, sizeof(ev)) < 0) {
        perror("write");
        return -1;
    }

    return 0;
}

int destroyDevice(int fd) {
    // It's better to wait a while
    // I guess that the OS need a while to read the last data in the FD
    sleep(1);

    // Destroy the input device
    if (ioctl(fd, UI_DEV_DESTROY)) {
        perror("ioctl");
        return -1;
    }

    // Close the FD
    if (close(fd) < 0) {
        perror("close");
        return -1;
    }

    return 0;
}