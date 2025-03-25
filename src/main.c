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

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);
/* Thread stacks */
#define UI_THREAD_STACK_SIZE 2048
#define SENSOR_THREAD_STACK_SIZE 4096

/* Thread priorities */
#define UI_THREAD_PRIORITY 5
#define SENSOR_THREAD_PRIORITY 5

#define MAX_SENSOR_MSGS 40
K_MSGQ_DEFINE(msgq, sizeof(sensor_msg_t), MAX_SENSOR_MSGS, 4);

/* Define the semaphore for UI readiness */
K_SEM_DEFINE(ui_ready_sem, 0, 1);

/* Define threads at file scope, not inside a function */
K_THREAD_DEFINE(ui_thread_id, UI_THREAD_STACK_SIZE, ui_thread_entry, NULL, NULL, NULL,
                UI_THREAD_PRIORITY, 0, 0);
                
K_THREAD_DEFINE(sensor_thread_id, SENSOR_THREAD_STACK_SIZE, sensors_run, NULL, NULL, NULL,
                SENSOR_THREAD_PRIORITY, 0, 500);

struct k_msgq* get_msgq(void)
{
    return &msgq;
}

int main(void)
{
    printk("Main function started\n");
    const struct device *display_dev;

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Display device not ready, aborting test");
        return 0;
    }
    printk("Display device is ready\n");

    /* Initialize UI */
    ui_init();
    printk("UI initialized\n");
    
    display_blanking_off(display_dev);

#ifdef CONFIG_LV_Z_MEM_POOL_SYS_HEAP
    lvgl_print_heap_info(false);
#else
    printf("lvgl in malloc mode\n");
#endif

    printk("UI thread started\n");
    
    /* Wait a moment to ensure UI thread has started */
    k_sleep(K_MSEC(500));

    printk("Sensor thread scheduled to start\n");

    /* Signal that UI is ready for sensors to start */
    k_sem_give(&ui_ready_sem);
    printk("UI ready semaphore given\n");

    /* Start the timer */
    start_timer();
    printk("Timer started\n");

    /* Main thread handles display updates */
    while (1) {
        uint32_t sleep_ms = lv_timer_handler();
        k_msleep(MIN(sleep_ms, 20)); /* Limit to max 50 FPS */
    }

    return 0;
}