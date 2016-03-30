//
// Created by yata on 12.02.16.
//

#ifndef CLIENT_MOUSECLIENT_H
#define CLIENT_MOUSECLIENT_H

/**
 * The mouse client will read the gyroscope and the joystick data
 * And write them in the "data" fifo
 *
 * Returns 0 when the joystick enter key is pressed
 *
 * On error, returns -1
 */
int startMouseClient();


#endif //CLIENT_MOUSECLIENT_H
