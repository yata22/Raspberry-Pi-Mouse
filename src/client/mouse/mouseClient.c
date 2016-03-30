//
// Created by yata on 12.02.16.
//

/**
 * Thanks to the sense hat examples locate at /usr/src/sense-hat/examples/RTIMULib/
 */

#include "mouseClient.h"
#include "../socket/client.h"
#include "../RTIMULib_connector/RTIMULib_connector.h"
#include "../joystick/joystick.h"
#include "../fifo/fifo.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/time.h>

int startMouseClient() {
    // Open the data fifo
    int data_fifo = openFifo("data", O_WRONLY);
    if (data_fifo == -1) {
        return -1;
    }

    // Open the error fifo (not implemented...)
    int error_fifo = openFifo("error", O_RDONLY | O_NONBLOCK);
    if (error_fifo == -1) {
        close(data_fifo);
        return -1;
    }

    char texte[BUFFER_SIZE];
    memset(texte, '\0', BUFFER_SIZE);

    struct pollfd evpoll = {
            .events = POLLIN,
    };

    // Initialize the joystick
    evpoll.fd = joystickInit();
    if (evpoll.fd < 0) {
        fprintf(stderr, "Event device not found.\n");
        return evpoll.fd;
    }

    // Initialize RTIMULib_connector
    RTIMULibConnectorInit();

    GyroData data;

    // This quit variable will be given to the event_handler
    // The value will be 1 on quit request event
    int quit = 0;

    // At each iteration, we will store the handled events in this variable
    // We need to know if an event has been handled, because it mean that a key has been pressed
    // And for stabilization issue, we will not look for the gyroscope data for a while
    // to don't move the mouse right after a click
    int event_handled;

    // We will store the time with some delay.
    // And will not look for the gyroscope data while this time is not outdated
    struct timeval tv;
    double next_time = 0;

    // process data
    while (!quit) {
        // Get the time
        gettimeofday(&tv, NULL);

        //  poll at the rate recommended by the IMU
        usleep((__useconds_t) (IMUGetPollInterval() * 1000));

        if ((tv.tv_sec) + (tv.tv_usec) / 1000000.0 >
            next_time) { // Look for the gyroscope data only if there is no delay
            while (IMURead()) {
                data = getGyroData();
                if (
                        (data.y > -0.025 && data.y < 0.025 && data.z > -0.015 &&
                        data.z < 0.015)                                                 //Data are too close to zero
                        || (data.y > 10 || data.y < -10 || data.z > 10 || data.y < -10) //Data are too big
                        ) {
                    continue;
                }
                // Data seems corrects
                // Write the "move data" in the fifo
                snprintf(texte, BUFFER_SIZE, "move:%f:%f", data.z, data.y);
                write(data_fifo, texte, BUFFER_SIZE);
            }
        }

        event_handled = 0;
        while (poll(&evpoll, 1, 0) > 0)
            event_handled += handle_events(evpoll.fd, data_fifo, BUFFER_SIZE, &quit);

        // Block the mouse movement for 0.2s
        if (event_handled > 0) {
            gettimeofday(&tv, NULL);
            next_time = (tv.tv_sec) + (tv.tv_usec) / 1000000.0 + 0.2;
        }
    }

    // Just wait a second to be sure that the client has send the last data store in the data_fifo
    sleep(1);
    close(error_fifo);
    close(data_fifo);

    joystickQuit(evpoll.fd);

    return 0;
}




