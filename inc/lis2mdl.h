/**
 * @file lis2mdl.h
 * @brief Header file for the LIS2MDL magnetometer driver
 *
 * Functions for initializing and sampling data from the LIS2MDL magnetometer
 *
 * Copyright (c) 2023
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LIS2MDL_H_
#define LIS2MDL_H_

#include <stdbool.h>

/**
 * @brief Initialize the LIS2MDL magnetometer
 *
 * @return true if initialization was successful, false otherwise
 */
bool my_lis2mdl_init(void);

/**
 * @brief Sample data from the LIS2MDL magnetometer
 *
 * This function will read the current magnetometer values and store them
 * using the store_sensor_reading function. If the device is not available,
 * simulated values will be used.
 */
void lis2mdl_sample(void);

#endif /* LIS2MDL_H_ */