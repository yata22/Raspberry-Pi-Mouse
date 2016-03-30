#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "client.h"
#include "../fifo/fifo.h"
#include <sys/errno.h>
#include <string.h>
#include <signal.h>

int startClient(char *ip) {
    // Open the data fifo
    int data_fifo = openFifo("data", O_RDONLY);
    if (data_fifo < 0) {
        return -1;
    }
    // Open the error fifo
    // Some errors will be write in this fifo
    // to allow the over process to print the error
    // in a graphic environment for instance
    int error_fifo = openFifo("error", O_WRONLY);
    if (error_fifo == -1) {
        close(data_fifo);
        return -1;
    }

    // The data received by the data_fifo will be stored in this variable
    char *data = calloc(sizeof(char), BUFFER_SIZE);
    if(data == NULL){
        perror("calloc");
        close(error_fifo);
        close(data_fifo);
        return -1;
    }


    struct sockaddr_in6 server_sin;

    /* Configure the connection */
    inet_pton(AF_INET6, ip, &(server_sin.sin6_addr));
    server_sin.sin6_family = AF_INET6;
    server_sin.sin6_port = htons(PORT);

    // We temporary ignore the SIGPIPE signals, to handle the errors when we try to write in the error fifo
    // If there is a SIGPIPE, the other process must have quit, so we also close the program properly
    signal(SIGPIPE, SIG_IGN);

    int sock;
    // Connect to the server
    while (1) {
        // Create the socket
        sock = socket(AF_INET6, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket()");
            goto exit;
        }
        printf("[socket client] Connection at %s on the port %d...\n", ip, PORT);
        // Connection
        while (connect(sock, (struct sockaddr *) &server_sin, sizeof(server_sin)) < 0) {
            if (write(error_fifo, strerror(errno), BUFFER_SIZE) < 0) {
                // Can't write data, the other process must have quit
                perror("write");
                goto exit;
            }

            // Retry to connect each second
            sleep(1);
        }

        printf("[socket client] Connected\n");
        if(write(error_fifo, "Connected", BUFFER_SIZE) < 0){
            goto exit;
        }

        ssize_t t = 0;
        while (1) {
            // Read the data_fifo
            t = read(data_fifo, data, BUFFER_SIZE);

            if (t > 0) {
                if (send(sock, data, BUFFER_SIZE, MSG_NOSIGNAL | MSG_DONTWAIT) < 0) {
                    // Disconnected (?)

                    // Write the error message in the error_fifo
                    if(write(error_fifo, strerror(errno), BUFFER_SIZE) < 0) {
                        goto exit;
                    }

                    // Break and try to connect again
                    break;
                }
            } else {
                // Can't read data, the other process must have quit
                if (t == 0) {
                    printf("[socket client] No data received.\n");
                } else {
                    perror("read()");
                }

                goto exit;
            }
        }

        close(sock);
        printf("[socket client] Socket closed\n");
    }

    exit:

    // Set the SIGPIPE signal to default
    signal(SIGPIPE, SIG_DFL);
    free(data);
    close(data_fifo);
    close(error_fifo);
    close(sock);
    printf("[socket client] Socket closed\n");

    return 0;
}