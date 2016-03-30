//
// Created by yata on 12.02.16.
//

/*
 * Thanks to :
 * input.h documentation :
 * https://www.kernel.org/doc/Documentation/input/event-codes.txt
 * Getting started with uinput :
 * http://thiemonge.org/getting-started-with-uinput
 */

#ifndef SOCKET_MOUSE_DEVICE_H
#define SOCKET_MOUSE_DEVICE_H

#include <asm-generic/int-ll64.h>

/**
 * Create a mouse device
 *
 * Returns the file descriptor corresponding to the created device
 *
 * On error, returns -1
 */
int createDevice();

/**
 * Move the mouse
 *
 * It's a relative movement (in pixels) on the x and y axe
 *
 * The fd parameter is the file descriptor corresponding to the device
 * created by the CreateDevice() function
 *
 * On error, returns -1
 */
int moveMouse(int fd, int x, int y);

/**
 * Execute the requested click
 *
 * The key clicked is given via the keyCode parameter
 * the key codes are defined in input.h
 *
 * The value is the key state (e.g. 0 = key release, 1 = key press, ...)
 *
 * The fd parameter is the file descriptor corresponding to the device
 * created by the CreateDevice() function
 *
 * On error, returns -1
 */
int clickMouse(int fd, __u16 keyCode, __s32 value);

/**
 * Destroy the device given in parameter
 * It should be a file descriptor created by the CreateDevice() function
 */
int destroyDevice(int fd);

#endif //SOCKET_MOUSE_DEVICE_H
