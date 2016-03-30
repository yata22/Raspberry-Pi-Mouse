//
// Created by yata on 24.12.15.
//

/**
 * Thanks to "bk138" for the "Multicast-Client-Server-Example" on github.com
 */

#ifndef CLIENT_BROADCAST_H
#define CLIENT_BROADCAST_H

#include <netinet/in.h>

#define MULTICAST_PORT "1903"
#define MULTICAST_IP "ff05::2"
#define MULTICAST_BUFFER_LENGTH 64
#define MULTICAST_SO_RCVBUF 300000


/**
 * Start the multicast client
 *
 * This function will listen the local network
 * And will return the server IP if data are received
 *
 * The IP got will be store in the ip parameter
 *
 * On fail, returns -1.
 */
int startMulticastClient(char *ip);

/**
   Creates a socket suitable for receiving multicast datagrams via recvfrom(). Also sets socket recv buffer.

   On success, returns socket.
   On fail, returns -1.
 */
int mcast_recv_socket(char *multicastIP, char *multicastPort, int multicastRecvBufSize);

#endif //CLIENT_BROADCAST_H
