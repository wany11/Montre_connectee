/**
 * @file lsm6dso.h
 * @brief Header file for LSM6DSO accelerometer and gyroscope sensor
 *
 * This file contains the declarations for initializing and sampling
 * from the LSM6DSO 6-axis IMU sensor.
 */

#ifndef LSM6DSO_H
#define LSM6DSO_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize the LSM6DSO accelerometer and gyroscope
 *
 * This function attempts to initialize the LSM6DSO sensor and
 * set the appropriate sampling frequency.
 *
 * @return true if initialization is successful, false otherwise
 */
bool my_lsm6dso_init(void);

/**
 * @brief Sample data from the LSM6DSO sensor
 *
 * This function fetches the latest accelerometer and gyroscope data
 * from the LSM6DSO sensor and processes it. If the sensor is not
 * available, it will use simulated values.
 */
void lsm6dso_sample(void);

/**
 * @brief Fetch current step count
 * 
 * @param steps Pointer to store step count value
 * @return int 0 on success, negative error code otherwise
 */
int lsm6dso_get_step_count(uint32_t *steps);

/**
 * @brief Reset step counter to zero
 * 
 * @return int 0 on success, negative error code otherwise
 */
int lsm6dso_reset_step_count(void);

#endif /* LSM6DSO_H */