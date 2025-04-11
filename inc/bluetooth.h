/**
 * @file bluetooth.h
 * @brief Bluetooth functionality for sensor data service
 *
 * This file contains declarations for Bluetooth functionality 
 * that provides a sensor data service for the connected device.
 *
 * Copyright (c) 2023 Nordic Semiconductor ASA
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * @brief Initialize and run Bluetooth functionality
 *
 * This function initializes the Bluetooth stack, starts advertising,
 * and handles connections and sensor data notifications. It includes
 * a non-returning loop that continuously processes Bluetooth events.
 */
void bluetooth_run(void);

/**
 * @brief Check if Bluetooth is currently active
 *
 * @return true if Bluetooth is active, false otherwise
 */
bool is_ble_active(void);

/**
 * @brief Restart Bluetooth advertising
 * 
 * Wakes up the Bluetooth thread and restarts advertising
 */
void bluetooth_restart(void);

/**
 * @brief Stop Bluetooth activity
 *
 * Stops advertising and disconnects any active connections
 */
void bluetooth_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_H_ */