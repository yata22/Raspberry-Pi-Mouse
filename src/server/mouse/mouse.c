//
// Created by yata on 13.02.16.
//

#include <stdio.h>
#include <string.h>
#include <linux/input.h>
#include "mouse.h"
#include "math.h"
#include "mouse_device.h"

int mouseHandler(char *buffer, void *device) {
    // Here we change the sound volume on key right and left
    // But we can also imagine do add a variable that contains the mouse speed
    // And to increase/decrease the mouse speed instead of the volume


    int *fd = (int *) device;

    if (strncmp(buffer, "event", 4) == 0) {
        int code = 0, value = 0;

        sscanf(buffer, "event:%3d:%3d", &code, &value);

        // printf("Event : [code] = %d [value] = %d\n", code, value);

        switch (code) {
            case KEY_UP:
                clickMouse(*fd, BTN_RIGHT, value);      // Right click
                break;
            case KEY_DOWN:
                clickMouse(*fd, BTN_LEFT, value);       // Left click
                break;
            case KEY_RIGHT:
                clickMouse(*fd, KEY_VOLUMEDOWN, value); // Sound volume down
                break;
            case KEY_LEFT:
                if (value == 1) {
                    clickMouse(*fd, KEY_VOLUMEUP, value); // Sound volume up
                    // Set immediately the volume key to 0
                    // To avoid that the key stays pressed and up the sound to 100% ;)
                    clickMouse(*fd, KEY_VOLUMEUP, 0);
                }
                break;

            default:
                break;
        }
    }
    else if (strncmp(buffer, "move", 3) == 0) {
        float x = 0, y = 0;

        sscanf(buffer, "move:%f:%f", &x, &y);

        moveMouse(*fd, (int) round(X_RATIO * x), (int) round(Y_RATIO * y));
    }

    return 0;
}
