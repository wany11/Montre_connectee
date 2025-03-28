/**
 * @file bluetooth.h
 * @brief Bluetooth functionality for Montre_connectee
 *
 * This header file provides the interface for the Bluetooth functionality
 * of the connected watch application.
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize and launch Bluetooth functionality
 * 
 * This function initializes the Bluetooth stack, starts advertising,
 * and enters the main Bluetooth processing loop.
 *
 * @return 0 on success, negative error code otherwise
 */
int launchBLE(void);

#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_H_ */