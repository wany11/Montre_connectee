#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include <zephyr/sys/util.h>
#include "../../inc/sensors.h"
#include "../../inc/queue.h"
#include "../../inc/hts221.h"
#include "../../inc/lsm6dso.h"
#include "../../inc/lis2mdl.h"

/* Global sensor data structure */
static sensor_data_t g_sensor_data = {
    .count = 0,
    .max_count = MAX_TEMP_READINGS,
    .overflow = false
};

/* Store the latest temperature reading */
static double latest_temperature = 0.0;

/* Store the latest humidity reading */
static double latest_humidity = 0.0;

/* Semaphore for UI readiness - declared in main.c */
extern struct k_sem ui_ready_sem;

void store_sensor_reading(sensor_msg_type_t type, double value)
{
    /* Handle reading based on sensor type */
    switch (type) {
    case MSG_TYPE_TEMPERATURE:
        /* Store the latest temperature */
        latest_temperature = value;
        
        /* Store temperature in data structure */
        if (g_sensor_data.count < g_sensor_data.max_count) {
            g_sensor_data.readings[g_sensor_data.count].temperature = value;
            g_sensor_data.readings[g_sensor_data.count].timestamp = k_uptime_get_32();
            g_sensor_data.count++;
        } else {
            /* Shift data to make room for new reading */
            for (uint32_t i = 0; i < g_sensor_data.max_count - 1; i++) {
                g_sensor_data.readings[i] = g_sensor_data.readings[i + 1];
            }
            g_sensor_data.readings[g_sensor_data.max_count - 1].temperature = value;
            g_sensor_data.readings[g_sensor_data.max_count - 1].timestamp = k_uptime_get_32();
            g_sensor_data.overflow = true;
        }
        break;
        
    case MSG_TYPE_HUMIDITY:
        /* Store the latest humidity */
        latest_humidity = value;
        break;
        
    default:
        /* Unknown sensor type */
        return;
    }
    
    /* Send to message queue */
    struct k_msgq *msgq = get_msgq();
    if (msgq != NULL) {
        sensor_msg_t msg = {
            .type = type,
            .value = value
        };
        
        /* Non-blocking send - if queue is full, we'll just skip this update */
        k_msgq_put(msgq, &msg, K_NO_WAIT);
    }
}

/* Wrapper functions for backward compatibility */
void store_reading(double temperature)
{
    store_sensor_reading(MSG_TYPE_TEMPERATURE, temperature);
}

void set_latest_humidity(double humidity)
{
    store_sensor_reading(MSG_TYPE_HUMIDITY, humidity);
}

bool get_latest_temperature(double *temperature)
{
    if (temperature == NULL || g_sensor_data.count == 0) {
        return false;
    }
    
    *temperature = latest_temperature;
    return true;
}

bool get_latest_humidity(double *humidity)
{
    if (humidity == NULL) {
        return false;
    }
    
    *humidity = latest_humidity;
    return true;
}

const sensor_data_t* get_sensor_data(void)
{
    return &g_sensor_data;
}

void clear_sensor_data(void)
{
    g_sensor_data.count = 0;
    g_sensor_data.overflow = false;
}

void sensors_init(void)
{    
    printk("Initializing sensors\n");
    
    /* Initialize HTS221 temperature and humidity sensor */
    if (!my_hts221_init()) {
        printk("Failed to initialize HTS221 temperature and humidity sensor\n");
    }
    if (!my_lsm6dso_init()) {
        printk("Failed to initialize lsm6ds0 accelerometer and gyroscope sensor\n");
    }
    if (!my_lis2mdl_init()) {
        printk("Failed to initialize lsm6ds0 accelerometer and gyroscope sensor\n");
    }
}

void sensors_run(void *p1, void *p2, void *p3)
{
    printk("Starting sensors thread\n");
    
    /* Wait for UI to be ready before starting sensors */
    printk("Waiting for UI ready semaphore...\n");
    if (k_sem_take(&ui_ready_sem, K_SECONDS(5)) != 0) {
        printk("Timed out waiting for UI ready semaphore, proceeding anyway\n");
    } else {
        printk("UI ready semaphore taken, initializing sensors\n");
    }
    
    /* Initialize sensors */
    sensors_init();
    
    printk("Sensors initialized successfully\n");
    
    /* Run sensor loop */
    while (1) {
        /* Poll both sensors */
        hts221_sample();
        lis2mdl_sample();
        lsm6dso_sample();
        /* Poll once per second */
        k_sleep(K_SECONDS(1));
    }
}