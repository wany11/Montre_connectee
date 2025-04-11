/*
 * Button handler for nRF5340DK
 * Handles button interrupts and display toggling
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/display.h>
#include "../../inc/debug.h"
#include "../ui/ui.h"
#include "../../inc/bluetooth.h"

/* Button configuration for nRF5340DK */
#define BUTTON1_NODE DT_ALIAS(sw0)
#define BUTTON2_NODE DT_ALIAS(sw1)
#define BUTTON_DEBOUNCE_DELAY_MS 50
#define BUTTON_MIN_INTERVAL_MS 200  // Temps minimum entre deux pressions

/* Button GPIO device */
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(BUTTON1_NODE, gpios);
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET(BUTTON2_NODE, gpios);
static struct gpio_callback button_cb_data1;
static struct gpio_callback button_cb_data2;

/* Display device reference */
static const struct device *display_dev;

/* Display state tracking */
static bool display_on = true;

/* Work item for debouncing button presses */
static struct k_work_delayable button_work;

/* Current active screen */
static lv_obj_t *current_active_screen = NULL;

static uint32_t last_button1_press = 0;
static uint32_t last_button2_press = 0;

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

/* Button 1 interrupt callback - Toggle Bluetooth */
static void button1_pressed_cb(const struct device *dev, struct gpio_callback *cb,
                             uint32_t pins)
{
    uint32_t current_time = k_uptime_get_32();
    BUTTON_INFO("Button 1 pressed at %u ms (last press: %u ms)\n", 
              current_time, last_button1_press);
              
    if (current_time - last_button1_press < BUTTON_MIN_INTERVAL_MS) {
        BUTTON_INFO("Button 1 press ignored due to debounce\n");
        return;
    }
    last_button1_press = current_time;
    
    /* Call the Bluetooth handler to toggle BLE */
    bluetooth_button_handler();
}

/* Button 2 interrupt callback - Home screen & Display toggle */
static void button2_pressed_cb(const struct device *dev, struct gpio_callback *cb,
                             uint32_t pins)
{
    uint32_t current_time = k_uptime_get_32();
    if (current_time - last_button2_press < BUTTON_MIN_INTERVAL_MS) {
        return;
    }
    last_button2_press = current_time;

    BUTTON_INFO("Button 2 pressed at %u ms\n", current_time);
    
    /* Go to home screen */
    current_active_screen = ui_Screen2;
    _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen2_screen_init);
    
    /* Toggle display if pressed again within short time */
    k_work_schedule(&button_work, K_MSEC(BUTTON_DEBOUNCE_DELAY_MS));
}

/* Initialize button */
int button_init(const struct device *disp_dev)
{
    int ret;

    /* Store display device reference for later use */
    display_dev = disp_dev;

    if (!gpio_is_ready_dt(&button1) || !gpio_is_ready_dt(&button2)) {
        BUTTON_ERROR("Error: button device %s is not ready\n", button1.port->name);
        return -ENODEV;
    }

    /* Configure buttons as inputs with pull-up */
    ret = gpio_pin_configure_dt(&button1, GPIO_INPUT | GPIO_PULL_UP);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to configure button1 pin\n", ret);
        return ret;
    }

    ret = gpio_pin_configure_dt(&button2, GPIO_INPUT | GPIO_PULL_UP);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to configure button2 pin\n", ret);
        return ret;
    }

    /* Initialize work item before configuring interrupts */
    k_work_init_delayable(&button_work, button_pressed_work_handler);

    /* Set up callbacks */
    gpio_init_callback(&button_cb_data1, button1_pressed_cb, BIT(button1.pin));
    gpio_init_callback(&button_cb_data2, button2_pressed_cb, BIT(button2.pin));

    /* Add callbacks */
    ret = gpio_add_callback(button1.port, &button_cb_data1);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to add callback for button1\n", ret);
        return ret;
    }
    
    ret = gpio_add_callback(button2.port, &button_cb_data2);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to add callback for button2\n", ret);
        return ret;
    }

    /* Configure interrupts AFTER adding callbacks */
    ret = gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to configure interrupt for button1\n", ret);
        return ret;
    }
    
    ret = gpio_pin_interrupt_configure_dt(&button2, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        BUTTON_ERROR("Error %d: failed to configure interrupt for button2\n", ret);
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

/* Bluetooth toggle handler */
void bluetooth_button_handler(void)
{
    if (!is_ble_active()) {
        BUTTON_INFO("Restarting Bluetooth advertising\n");
        bluetooth_restart();
    } else {
        BUTTON_INFO("Stopping Bluetooth\n");
        bluetooth_stop();
    }
}