#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>

/**
 * @brief Initialize button handler
 *
 * Sets up button GPIO and interrupt handling
 *
 * @param disp_dev Display device reference for toggling display
 * @return 0 on success, negative error code on failure
 */
int button_init(const struct device *disp_dev);

/**
 * @brief Check if display is currently on
 *
 * @return true if display is on, false if off
 */
bool is_display_on(void);

/**
 * @brief Manually turn display on
 */
void display_turn_on(void);

/**
 * @brief Manually turn display off
 */
void display_turn_off(void);

#endif /* BUTTON_HANDLER_H */


void bluetooth_button_handler(void);
