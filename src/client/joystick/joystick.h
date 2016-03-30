//
// Created by yata on 19.02.16.
//

#ifndef SERVER_JOYSTICK_H
#define SERVER_JOYSTICK_H

#include <dirent.h>

#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

/**
 * Returns a file descriptor corresponding to the Raspberry Pi Joystick
 *
 * Returns -1 on fails
 */
int joystickInit();

/**
 * Handle the events
 *
 * evfd         : Joystick event file descriptor
 * out_fd       : Events found are write in this fd
 * buffer_size  : out_fd buffer size
 * *quit        : If there is an exit request (in an event), change the quit value to 1
 *
 * Returns 1 if an event is handled, 0 if not
 */
int handle_events(int evfd, int out_fd, size_t buffer_size, int *quit);

/**
 * Close the joystick file descriptor
 *
 * On error, returns -1
 */
int joystickQuit(int fd);

/**
 * Returns 1 if the directory is an event device
 */
static int is_event_device(const struct dirent *dir);

/**
 * Try to find the requested event
 *
 * dev_name : The event name
 *
 * Returns a file descriptor if the event is found
 *
 * Else, returns -1
 */
static int open_evdev(const char *dev_name);

#endif //SERVER_JOYSTICK_H
