/*
 * Button handler for nRF5340DK
 * Handles button interrupts and display toggling
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/display.h>
#include "../../inc/debug.h"

/* Button configuration for nRF5340DK */
#define BUTTON1_NODE DT_ALIAS(sw0)
#define BUTTON_DEBOUNCE_DELAY_MS 50

/* Button GPIO device */
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON1_NODE, gpios);
static struct gpio_callback button_cb_data;

/* Display device reference */
static const struct device *display_dev;

/* Display state tracking */
static bool display_on = true;

/* Work item for debouncing button presses */
static struct k_work_delayable button_work;

/* Button press handler with debounce */
static void button_pressed_work_handler(struct k_work *work)
{
    /* Toggle display state */
    display_on = !display_on;
    
    if (display_on) {
        BUTTON_INFO("Turning display ON\n");
        display_set_brightness(display_dev, 255);
        display_blanking_off(display_dev);
    } else {
        BUTTON_INFO("Turning display OFF\n");
        display_set_brightness(display_dev, 1);
        display_blanking_on(display_dev);
    }
}

/* Button interrupt callback */
static void button_pressed_cb(const struct device *dev, struct gpio_callback *cb,
                             uint32_t pins)
{
    BUTTON_VERBOSE("Button pressed at %u ms\n", k_uptime_get_32());
    
    /* Schedule the debounce handler */
    k_work_schedule(&button_work, K_MSEC(BUTTON_DEBOUNCE_DELAY_MS));
}

/* Initialize button */
int button_init(const struct device *disp_dev)
{
    int ret;

    /* Store display device reference for later use */
    display_dev = disp_dev;

    if (!gpio_is_ready_dt(&button)) {
        BUTTON_ERROR("Button device %s is not ready\n", button.port->name);
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret != 0) {
        BUTTON_ERROR("Failed to configure button pin: %d\n", ret);
        return ret;
    }

    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        BUTTON_ERROR("Failed to configure button interrupt: %d\n", ret);
        return ret;
    }

    /* Initialize work item for button debouncing */
    k_work_init_delayable(&button_work, button_pressed_work_handler);

    /* Setup GPIO callback */
    gpio_init_callback(&button_cb_data, button_pressed_cb, BIT(button.pin));
    ret = gpio_add_callback(button.port, &button_cb_data);
    if (ret != 0) {
        BUTTON_ERROR("Failed to add button callback: %d\n", ret);
        return ret;
    }

    BUTTON_INFO("Button initialized successfully\n");
    return 0;
}

/* Get current display state */
bool is_display_on(void)
{
    return display_on;
}

/* Manually turn display on */
void display_turn_on(void)
{
    if (!display_on) {
        display_on = true;
        display_blanking_off(display_dev);
        BUTTON_INFO("Display manually turned ON\n");
    }
}

/* Manually turn display off */
void display_turn_off(void)
{
    if (display_on) {
        display_on = false;
        display_blanking_on(display_dev);
        BUTTON_INFO("Display manually turned OFF\n");
    }
}