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
#include "../../inc/chronometer.h"
#include <lvgl.h>

LOG_MODULE_REGISTER(touch_screen, CONFIG_LOG_DEFAULT_LEVEL);

#define REFRESH_RATE 100

// Définition des délais d'anti-rebond (en ms)
#define TOUCH_DEBOUNCE_DELAY_MS 400

static const struct device *const touch_dev = DEVICE_DT_GET(DT_NODELABEL(tsc2007_adafruit_2_8_tft_touch_v2));
 
static struct k_sem touch_sem;
 
static struct {
    size_t x;
    size_t y;
    bool pressed;
    bool new_data;
} touch_point;

// Stockage du dernier temps d'interaction pour l'anti-rebond global
static uint32_t last_touch_time = 0;
 
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
    last_touch_time = 0;

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

    // Vérifie si nous avons des événements tactiles
    if (touch_point.pressed) {
        uint32_t current_time = k_uptime_get_32();
        
        // Anti-rebond global - ignorer les appuis trop rapprochés
        if (current_time - last_touch_time > TOUCH_DEBOUNCE_DELAY_MS) {
            last_touch_time = current_time;
            
            if (touch_point.y > 250) {
                if (lv_scr_act() == ui_Screen2 || lv_scr_act() == NULL) {
                    _ui_screen_change(&ui_Screen8, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen8_screen_init);
                } else if (lv_scr_act() == ui_Screen3) {
                    _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen2_screen_init);
                } else if (lv_scr_act() == ui_Screen4) {
                    _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen3_screen_init);
                } else if (lv_scr_act() == ui_Screen5) {
                    _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen4_screen_init);
                } else if (lv_scr_act() == ui_Screen6) {
                    _ui_screen_change(&ui_Screen5, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen5_screen_init);
                } else if (lv_scr_act() == ui_Screen7) {
                    _ui_screen_change(&ui_Screen6, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen6_screen_init);
                } else if (lv_scr_act() == ui_Screen8) {
                    _ui_screen_change(&ui_Screen7, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen7_screen_init);
                }
            }
            else if (touch_point.y < 50) {
                if (lv_scr_act() == ui_Screen2 || lv_scr_act() == NULL) {
                    _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen3_screen_init);
                } else if (lv_scr_act() == ui_Screen3) {
                    _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen4_screen_init);
                } else if (lv_scr_act() == ui_Screen4) {
                    _ui_screen_change(&ui_Screen5, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen5_screen_init);
                } else if (lv_scr_act() == ui_Screen5) {
                    _ui_screen_change(&ui_Screen6, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen6_screen_init);
                } else if (lv_scr_act() == ui_Screen6) {
                    _ui_screen_change(&ui_Screen7, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen7_screen_init);
                } else if (lv_scr_act() == ui_Screen7) {
                    _ui_screen_change(&ui_Screen8, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen8_screen_init);
                }else if (lv_scr_act() == ui_Screen8) {
                    _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0, &ui_Screen2_screen_init);
                }
            }
        } else {
            LOG_DBG("Touch ignored (anti-rebond): %d ms since last touch", 
                    current_time - last_touch_time);
        }
    }

    // Handle LVGL timers and return sleep time
    return lv_timer_handler();
}