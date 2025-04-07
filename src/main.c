/*
 * Copyright (c) 2024 LVGL <felipe@lvgl.io>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <lvgl_mem.h>
#include "ui/ui.h"
#include "../inc/sensors.h"
#include <stdio.h>
#include "../inc/queue.h"
#include "../inc/timSec.h"
#include "../inc/debug.h"
#include "../inc/button.h"
#include "../inc/bluetooth.h"
#include "../inc/rtc.h"
#include "../inc/touch_screen.h"

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);


/* Thread stacks */
#define UI_THREAD_STACK_SIZE 2048
#define SENSOR_THREAD_STACK_SIZE 4096
#define BLE_THREAD_STACK_SIZE 4096


/* Thread priorities */
#define UI_THREAD_PRIORITY 5
#define SENSOR_THREAD_PRIORITY 5
#define BLE_THREAD_PRIORITY 5

#define MAX_SENSOR_MSGS 40
K_MSGQ_DEFINE(msgq, sizeof(sensor_msg_t), MAX_SENSOR_MSGS, 4);

/* Define the semaphore for UI readiness */
K_SEM_DEFINE(ui_ready_sem, 0, 1);

/* Define threads at file scope, not inside a function */
K_THREAD_DEFINE(ui_thread_id, UI_THREAD_STACK_SIZE, ui_thread_entry, NULL, NULL, NULL,
                UI_THREAD_PRIORITY, 0, 0);
                
K_THREAD_DEFINE(sensor_thread_id, SENSOR_THREAD_STACK_SIZE, sensors_run, NULL, NULL, NULL,
                SENSOR_THREAD_PRIORITY, 0, 500);

K_THREAD_DEFINE(bluetooth_thread_id, BLE_THREAD_STACK_SIZE, bluetooth_run, NULL, NULL, NULL,
                BLE_THREAD_PRIORITY, 0, 1000);

struct k_msgq* get_msgq(void)
{
    return &msgq;
}

int main(void)
{
    MAIN_INFO("Main function started\n");
    const struct device *display_dev;

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        MAIN_ERROR("Display device not ready, aborting test\n");
        return 0;
    }
    MAIN_INFO("Display device is ready\n");

    /* Initialize button handler */
    if (button_init(display_dev) != 0) {
        MAIN_ERROR("Button initialization failed\n");
        /* Continue anyway, as this is not critical */
    }

    /* Initialize touchscreen */
    if (touch_screen_init() != 0) {
        MAIN_ERROR("Touchscreen initialization failed\n");
        /* Continue anyway, as this might not be critical */
    } else {
        MAIN_INFO("Touchscreen initialized\n");
    }

    /* Initialize UI */
    ui_init();
    MAIN_INFO("UI initialized\n");
    
    display_blanking_off(display_dev);

#ifdef CONFIG_LV_Z_MEM_POOL_SYS_HEAP
    lvgl_print_heap_info(false);
#else
    MAIN_INFO("LVGL in malloc mode\n");
#endif

    MAIN_INFO("UI thread started\n");
    
    /* Wait a moment to ensure UI thread has started */
    k_sleep(K_MSEC(500));

    MAIN_INFO("Sensor thread scheduled to start\n");

    /* Signal that UI is ready for sensors to start */
    k_sem_give(&ui_ready_sem);
    MAIN_INFO("UI ready semaphore given\n");

    /* Start the timer */
    start_timer();
    printk("Timer started\n");

    /* RTC initialization */
    if (rtc_init() == 0) {

        
        /* Variables to store the read time */
        uint16_t year;
        uint8_t month, day, hour, minute, second;
        
        /* Read the current time */
        if (rtc_get_datetime(&year, &month, &day, &hour, &minute, &second) == 0) {
            printk("Date/time: %04d-%02d-%02d %02d:%02d:%02d\n",
                   year, month, day, hour, minute, second);
        }
    }

    /* Main loop */
    while (1) {
        /* Process touchscreen events */
        uint32_t sleep_ms = touch_screen_process();
        
        /* Limit to max 50 FPS */
        k_msleep(MIN(sleep_ms, 20));
    }

    return 0;
}