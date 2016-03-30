//
// Created by yata on 24.12.15.
//

/**
 * Thanks to "bk138" for the "Multicast-Client-Server-Example" on github.com
 */

#ifndef SERVER_SOCKET_MULTICAST_SERVER_H
#define SERVER_SOCKET_MULTICAST_SERVER_H

#define MULTICAST_PORT "1903"
#define MULTICAST_IP "ff05::2"
#define MULTICAST_BUFFER_LENGTH 64
#define MULTICAST_TTL 1

#include <netdb.h>

typedef struct quit_var_with_mutex {
    int quit;
    pthread_mutex_t mutex;
} quit_var_with_mutex;

/**
 * Start the multicast server
 *
 * This function need to be called in a new thread
 *
 * This function will send multicast data to the local network.
 * The clients will be able to get the server local IP
 *
 * The parameter is a pointer to an integer
 * The server will quit when the pointed value
 * isn't equal to zero
 *
 * Of fail, exit.
 */
void *startMulticastServer(void *quit);

/**
   Creates a socket suitable for sending multicast datagrams via sendto().

   On success, a socket is returned and multicastAddr set with the right info for sendto() calls.
   On error, -1 is returned.
*/
int mcast_send_socket(char *multicastIP, char *multicastPort, int multicastTTL, struct addrinfo **multicastAddr);

#endif //SERVER_SOCKET_MULTICAST_SERVER_H
