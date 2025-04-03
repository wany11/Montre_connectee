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

/**
 * @brief Initialize and run Bluetooth functionality
 *
 * This function initializes the Bluetooth stack, starts advertising,
 * and handles connections and sensor data notifications. It includes
 * a non-returning loop that continuously processes Bluetooth events.
 */
void bluetooth_run(void);

#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_H_ */