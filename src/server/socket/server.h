//
// Created by yata on 21.12.15.
//

#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H

#define PORT 41231
#define MAX_DATA_LENGTH 128

/**
 * Start the TCP/IP server
 *
 * The server will listen for any connection
 * The received data will be sent to the callback function
 * The user can send a parameter to this call function with
 * the param parameter.
 *
 * On error, returns -1
 */
int startServer(int (*callback)(char *, void *), void *param);

/**
 *  This function will receive the data sent by the client
 *  And sends will send this data to the callback function
 *
 *  param : The user can send a second parameter to the callback function
 *  with this parameter
 *
 *  Returns 0
 *  If the client send "quit", returns 1.
 *  On error, returns -1
 */
int connectionHandler(int sock, int (*callback)(char *, void *), void *param);

/**
 * Returns a socket that will listen for any connection
 *
 * On error, returns -1
 */
int createSocket();

#endif //SOCKET_SOCKET_H
