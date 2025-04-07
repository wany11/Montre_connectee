#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <lvgl_mem.h>
#include <stdio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <zephyr/sys/util.h>
#include "../ui/ui.h"
#include "../../inc/touch_screen.h"

LOG_MODULE_REGISTER(touch_screen, CONFIG_LOG_DEFAULT_LEVEL);

#define REFRESH_RATE 100

static const struct device *const touch_dev = DEVICE_DT_GET(DT_NODELABEL(tsc2007_adafruit_2_8_tft_touch_v2));
 
static struct k_sem touch_sem;
 
static struct {
    size_t x;
    size_t y;
    bool pressed;
    bool new_data;
} touch_point;
 
static void touch_event_callback(struct input_event *evt, void *user_data)
{
    if (evt->code == INPUT_ABS_X) {
        touch_point.x = evt->value;
    }
    if (evt->code == INPUT_ABS_Y) {
        touch_point.y = evt->value;
    }
    if (evt->code == INPUT_BTN_TOUCH) {
        touch_point.pressed = evt->value;
    }
    if (evt->sync) {
        touch_point.new_data = true;
        k_sem_give(&touch_sem);
    }
}
 
INPUT_CALLBACK_DEFINE(touch_dev, touch_event_callback, NULL);

int touch_screen_init(void)
{
    LOG_INF("Initializing touchscreen: %s", touch_dev->name);
 
    if (!device_is_ready(touch_dev)) {
        LOG_ERR("Device %s not found.", touch_dev->name);
        return -ENODEV;
    }
 
    k_sem_init(&touch_sem, 0, 1);
    touch_point.new_data = false;

    return 0;
}

int touch_screen_get_point(size_t *x, size_t *y, bool *pressed)
{
    if (!touch_point.new_data) {
        return -EAGAIN;
    }

    *x = touch_point.x;
    *y = touch_point.y;
    *pressed = touch_point.pressed;
    touch_point.new_data = false;

    return 0;
}

uint32_t touch_screen_process(void)
{
    // Wait for touch event with timeout
    if (k_sem_take(&touch_sem, K_MSEC(50)) == 0) {
        LOG_INF("TOUCH %s X, Y: (%d, %d)", 
                touch_point.pressed ? "PRESS" : "RELEASE",
                touch_point.x, touch_point.y);
    }

    // Handle LVGL timers and return sleep time
    return lv_timer_handler();
}