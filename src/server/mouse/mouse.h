//
// Created by yata on 13.02.16.
//

/**
 * This file contains all the functions required to handle the received data
 * to move the mouse via the "mouse_device.c functions"
 */


#ifndef SERVER_MOUSE_H
#define SERVER_MOUSE_H

#define MOUSE_SPEED 8
#define X_RATIO     3 * MOUSE_SPEED
#define Y_RATIO     2.5 * MOUSE_SPEED

/**
 * This function will handle the data received by the buffer parameter
 *
 * The device parameter is a file descriptor representing the mouse
 * (Can be create via the functions defined in mouse_device.h)
 *
 * For instance, if a left click is requested, the function will execute
 * that click (by calling functions defined in mouse_device.h)
 */
int mouseHandler(char *buffer, void *device);

#endif //SERVER_MOUSE_H
