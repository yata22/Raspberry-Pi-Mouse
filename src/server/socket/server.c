//
// Created by yata on 21.12.15.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include "server.h"
#include "multicastServer.h"
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <bits/sigthread.h>

int startServer(int (*callback)(char *, void *), void *param) {

    int sock = createSocket();
    if (sock < 0) {
        fprintf(stderr, "[server] createSocket() failed\n");
        return -1;
    }

    // Run the multicast server in background
    pthread_t multicastThread;
    // To quit properly, the multicast thread must know when he must quit
    // This is done by setting the value of the quitMulticast.quit variable to 1 :
    quit_var_with_mutex quitMulticast;
    quitMulticast.quit = 0;
    quitMulticast.mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    if (pthread_create(&multicastThread, NULL, startMulticastServer, (void *) &quitMulticast) < 0) {
        perror("pthread_create");
        return -1;
    }

    int client_sock;

    // Waiting for any connection
    while ((client_sock = accept(sock, NULL, 0))) {
        if (client_sock == -1) {
            perror("accept");
            break;
        }

        // Launch the connection handler
        if (connectionHandler(client_sock, callback, param) != 0) {
            // There is an error, or the client want the server to disconnect
            break;
        }

        // Check if the multicast thread is still operating
        if(pthread_kill(multicastThread, 0) != 0){
            fprintf(stderr, "[server] The multicast thread isn't running any more. Closing the server...\n");
            break;
        }
    }



    // Ask to the multicast server to quit
    quitMulticast.quit = 1;
    pthread_join(multicastThread, NULL);

    close(sock);

    printf("[server] Server closed\n");

    return 0;
}


int connectionHandler(int sock, int (*callback)(char *, void *), void *param) {
    printf("[server] New connection\n");

    // Prepare the data reception
    ssize_t read_size;
    char buffer[MAX_DATA_LENGTH];

    // Receive data from the client
    while ((read_size = recv(sock, buffer, MAX_DATA_LENGTH, 0)) != 0) {
        if (read_size < 0 || read_size > MAX_DATA_LENGTH) {
            printf("Invalid data\n");
            continue;
        }
        buffer[MAX_DATA_LENGTH - 1] = '\0';

        // Call the callback function
        (*callback)(buffer, param);

        // Return 1 if the client want to close the server
        if (strncmp(buffer, "quit", 3) == 0) {
            close(sock);
            return 1;
        }
    }

    if (read_size == 0) {
        printf("[server] Client disconnected\n");
    }
    else if (read_size == -1) {
        perror("recv failed");
    }

    close(sock);

    return 0;
}


int createSocket() {
    int sock;
    struct sockaddr_in6 sin;
    int yes = 1;

    // Create the server socket
    sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // Socket accept any connection
    sin.sin6_addr = (struct in6_addr) IN6ADDR_ANY_INIT;
    sin.sin6_family = AF_INET6;
    sin.sin6_port = htons(PORT);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        return -1;
    }

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("bind");
        return -1;
    }

    printf("[server] Socket open\n");

    // Start listening
    if (listen(sock, 2) < 0) {
        perror("listen");
    }
    printf("[server] Listening port %d...\n", PORT);

    return sock;
}



