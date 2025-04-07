#ifndef TOUCH_SCREEN_H
#define TOUCH_SCREEN_H

#include <zephyr/device.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Initialize the touchscreen
 * 
 * @return 0 on success, negative error code otherwise
 */
int touch_screen_init(void);

/**
 * @brief Get current touch point information
 * 
 * @param x Pointer to store the X coordinate
 * @param y Pointer to store the Y coordinate
 * @param pressed Pointer to store the pressed state
 * @return 0 if new data available, negative error code otherwise
 */
int touch_screen_get_point(size_t *x, size_t *y, bool *pressed);

/**
 * @brief Process touch events
 * 
 * @return Sleep time in ms
 */
uint32_t touch_screen_process(void);

#endif /* TOUCH_SCREEN_H */