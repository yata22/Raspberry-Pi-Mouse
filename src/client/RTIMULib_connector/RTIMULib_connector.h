//
// Created by yata on 12.02.16.
//

/**
 * This short RTIMULib_connector library is used to call the c++ RTIMULib
 * library from C code
 */

#ifndef CLIENT_RTIMULIB_CONNECTOR_H
#define CLIENT_RTIMULIB_CONNECTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Contains the gyroscope data
 */
typedef struct GyroData {
    float x;
    float y;
    float z;
} GyroData;

/**
 * Init the RTIMULib_connector library
 */
int RTIMULibConnectorInit();

/**
 * Returns the actual gyroscope data
 */
GyroData getGyroData();

/**
 * See IMUGetPollInterval() from the RTIMULib library
 */
int IMUGetPollInterval();

/**
 * See IMURead() from the RTIMULib library
 */
int IMURead();

#ifdef __cplusplus
}
#endif

#endif //CLIENT_RTIMULIB_CONNECTOR_H
