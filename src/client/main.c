#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include "socket/multicastClient.h"
#include "socket/client.h"
#include "fifo/fifo.h"
#include "mouse/mouseClient.h"

int main() {
    char ip[INET6_ADDRSTRLEN];

    printf("Looking for the server IP...\n");
    if (0 != startMulticastClient(ip)) {
        return EXIT_FAILURE;
    }

    printf("IP got : [ip] = %s\n", ip);
    printf("Starting the client application...\n");

    // Create the communication fifo between the two process
    if (createFifos(2, "data", "error") != 0) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    pid_t pid_child;
    pid_child = fork();
    if (pid_child == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid_child == 0) {
        if (0 != startMouseClient()) {
            return EXIT_FAILURE;
        }

    } else {
        if (0 != startClient(ip)) {
            return EXIT_FAILURE;
        }

        // The child process must be ended
        // Just wait a few seconds if it takes longer than it should
        sleep(2);
    }

    return EXIT_SUCCESS;
}