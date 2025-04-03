/*
 * Button handler for nRF5340DK
 * Handles button interrupts and display toggling
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/display.h>
#include "../../inc/debug.h"
#include "../ui/ui.h"

/* Button configuration for nRF5340DK */
#define BUTTON1_NODE DT_ALIAS(sw0)
#define BUTTON2_NODE DT_ALIAS(sw1)
#define BUTTON3_NODE DT_ALIAS(sw2)
#define BUTTON4_NODE DT_ALIAS(sw3)
#define BUTTON_DEBOUNCE_DELAY_MS 50

/* Button GPIO device */
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(BUTTON1_NODE, gpios);
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET(BUTTON2_NODE, gpios);
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET(BUTTON3_NODE, gpios);
static const struct gpio_dt_spec button4 = GPIO_DT_SPEC_GET(BUTTON4_NODE, gpios);
static struct gpio_callback button_cb_data1;
static struct gpio_callback button_cb_data2;
static struct gpio_callback button_cb_data3;
static struct gpio_callback button_cb_data4;

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

/* Button 3 interrupt callback */
static void button3_pressed_cb(const struct device *dev, struct gpio_callback *cb,
                             uint32_t pins)
{
    BUTTON_VERBOSE("Button 3 pressed at %u ms\n", k_uptime_get_32());
    
    _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_Screen1_screen_init);
}

/* Button 4 interrupt callback */
static void button4_pressed_cb(const struct device *dev, struct gpio_callback *cb,
                             uint32_t pins)
{
    BUTTON_VERBOSE("Button 4 pressed at %u ms\n", k_uptime_get_32());
    _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen2_screen_init);
    
}

/* Initialize button */
int button_init(const struct device *disp_dev)
{
    int ret;

    /* Store display device reference for later use */
    display_dev = disp_dev;

    if (!gpio_is_ready_dt(&button1) || !gpio_is_ready_dt(&button2) ||
        !gpio_is_ready_dt(&button3) || !gpio_is_ready_dt(&button4)) {
        BUTTON_ERROR("Error: button device %s is not ready\n", button1.port->name);
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&button1, GPIO_INPUT | GPIO_PULL_UP);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to configure %s pin %d\n",
                ret, button1.port->name, button1.pin);
        return ret;
    }

    ret = gpio_pin_configure_dt(&button2, GPIO_INPUT | GPIO_PULL_UP);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to configure %s pin %d\n",
                ret, button2.port->name, button2.pin);
        return ret;
    }

    ret = gpio_pin_configure_dt(&button3, GPIO_INPUT | GPIO_PULL_UP);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to configure %s pin %d\n",
                ret, button3.port->name, button3.pin);
        return ret;
    }

    ret = gpio_pin_configure_dt(&button4, GPIO_INPUT | GPIO_PULL_UP);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to configure %s pin %d\n",
                ret, button4.port->name, button4.pin);
        return ret;
    }

    ret = gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure_dt(&button2, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure_dt(&button3, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure_dt(&button4, GPIO_INT_EDGE_TO_ACTIVE);

    gpio_init_callback(&button_cb_data1, button_pressed_cb, BIT(button1.pin));
    gpio_init_callback(&button_cb_data2, button_pressed_work_handler, BIT(button2.pin));
    gpio_init_callback(&button_cb_data3, button3_pressed_cb, BIT(button3.pin));
    gpio_init_callback(&button_cb_data4, button4_pressed_cb, BIT(button4.pin));

    gpio_add_callback(button1.port, &button_cb_data1);
    gpio_add_callback(button2.port, &button_cb_data2);
    gpio_add_callback(button3.port, &button_cb_data3);
    gpio_add_callback(button4.port, &button_cb_data4);

    k_work_init_delayable(&button_work, button_pressed_work_handler);

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