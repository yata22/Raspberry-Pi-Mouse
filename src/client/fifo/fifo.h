//
// Created by yata on 25.03.16.
//

#ifndef CLIENT_FIFO_H
#define CLIENT_FIFO_H

/**
 * Create several fifo
 * name : this is the fifo name
 *
 * On error, returns -1
 */
int createFifo(char *name);

/**
 * Create few fifo
 *
 * num : this is the number of fifo that the user want to create
 *
 * additional arguments : there correspond to each
 * fifo name, must be char* format
 *
 * On any error, returns -1
 */
int createFifos(int num, ...);

/**
 * Returns a read only fifo
 *
 * name : this is the fifo name
 */

/**
 * Open a fifo
 *
 * name : fifo name
 * flags : open flags
 *
 * returns the fifo or -1 on failure
 */

int openFifo(char *name, int flags);

/**
 * Get the fifo dir
 *
 * name : fifo name
 * dir  : the dir will be store in this parameter
 * length : dir length
 *
 * On error, returns -1
 */
static int getFifoDir(char *name, char *dir, size_t length);

#endif //CLIENT_FIFO_H
