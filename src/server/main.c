#include <stdlib.h>
#include "socket/server.h"
#include "mouse/mouse_device.h"
#include "mouse/mouse.h"

int main() {

    // Create the mouse device
    int fd = createDevice();
    if (fd < 0) {
        return EXIT_FAILURE;
    }

    // Start the server. The received data will be sent to the mouseHandler()
    // callback function
    if (0 != startServer(mouseHandler, (void *) &fd)) {
        return EXIT_FAILURE;
    }

    // Destroy the mouse device
    destroyDevice(fd);

    return EXIT_SUCCESS;
}