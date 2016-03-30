//
// Created by yata on 21.12.15.
//

#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#define PORT 41231
#define BUFFER_SIZE 128

/**
 * Start the client.
 * It will send to the server the data contained in the fifo named "data"
 *
 * Stop the connection when the other process is also ending
 *
 * ip : ip of the server the client will connect to
 *
 * On error, store the error message in the fifo named "error"
 *
 * On fail, returns -1
 */
int startClient(char *ip);

#endif //SOCKET_CLIENT_H
