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
#include "../../inc/debug.h"  /* Include the debug header */

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
        SENSORS_VERBOSE("Storing temperature reading: %.2f°C\n", value);
        
        /* Store temperature in data structure */
        if (g_sensor_data.count < g_sensor_data.max_count) {
            g_sensor_data.readings[g_sensor_data.count].temperature = value;
            g_sensor_data.readings[g_sensor_data.count].timestamp = k_uptime_get_32();
            g_sensor_data.count++;
            SENSORS_VERBOSE("Added temperature to readings array, count=%d\n", g_sensor_data.count);
        } else {
            /* Shift data to make room for new reading */
            SENSORS_VERBOSE("Readings array full, shifting data\n");
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
        SENSORS_VERBOSE("Storing humidity reading: %.2f%%\n", value);
        break;
        
    case MSG_TYPE_ACCEL_X:
    case MSG_TYPE_ACCEL_Y:
    case MSG_TYPE_ACCEL_Z:
        /* Store accelerometer data */
        SENSORS_VERBOSE("Storing accelerometer reading (axis %d): %.2f\n", 
                      type - MSG_TYPE_ACCEL_X, value);
        break;
        
    case MSG_TYPE_GYRO_X:
    case MSG_TYPE_GYRO_Y:
    case MSG_TYPE_GYRO_Z:
        /* Store gyroscope data */
        SENSORS_VERBOSE("Storing gyroscope reading (axis %d): %.2f\n", 
                      type - MSG_TYPE_GYRO_X, value);
        break;
        
    case MSG_TYPE_MAG_X:
    case MSG_TYPE_MAG_Y:
    case MSG_TYPE_MAG_Z:
        /* Store magnetometer data */
        SENSORS_VERBOSE("Storing magnetometer reading (axis %d): %.2f\n", 
                      type - MSG_TYPE_MAG_X, value);
        break;
        
    default:
        /* Unknown sensor type */
        SENSORS_ERROR("Unknown sensor message type: %d\n", type);
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
        int ret = k_msgq_put(msgq, &msg, K_NO_WAIT);
        if (ret == 0) {
            SENSORS_VERBOSE("Message sent to queue: type=%d, value=%.2f\n", type, value);
        } else {
            SENSORS_ERROR("Failed to send message to queue: %d\n", ret);
        }
    } else {
        SENSORS_ERROR("Message queue not available\n");
    }
}

/* Wrapper functions for backward compatibility */
void store_reading(double temperature)
{
    SENSORS_VERBOSE("Using legacy store_reading() function\n");
    store_sensor_reading(MSG_TYPE_TEMPERATURE, temperature);
}

void set_latest_humidity(double humidity)
{
    SENSORS_VERBOSE("Using legacy set_latest_humidity() function\n");
    store_sensor_reading(MSG_TYPE_HUMIDITY, humidity);
}

bool get_latest_temperature(double *temperature)
{
    if (temperature == NULL || g_sensor_data.count == 0) {
        SENSORS_ERROR("Invalid temperature pointer or no readings available\n");
        return false;
    }
    
    *temperature = latest_temperature;
    SENSORS_VERBOSE("Retrieved latest temperature: %.2f°C\n", *temperature);
    return true;
}

bool get_latest_humidity(double *humidity)
{
    if (humidity == NULL) {
        SENSORS_ERROR("Invalid humidity pointer\n");
        return false;
    }
    
    *humidity = latest_humidity;
    SENSORS_VERBOSE("Retrieved latest humidity: %.2f%%\n", *humidity);
    return true;
}

const sensor_data_t* get_sensor_data(void)
{
    SENSORS_VERBOSE("Returning sensor data structure, count=%d\n", g_sensor_data.count);
    return &g_sensor_data;
}

void clear_sensor_data(void)
{
    SENSORS_INFO("Clearing sensor data\n");
    g_sensor_data.count = 0;
    g_sensor_data.overflow = false;
}

void sensors_init(void)
{    
    SENSORS_INFO("Initializing sensors\n");
    
    /* Initialize HTS221 temperature and humidity sensor */
    if (!my_hts221_init()) {
        SENSORS_ERROR("Failed to initialize HTS221 temperature and humidity sensor\n");
    } else {
        SENSORS_INFO("HTS221 initialized successfully\n");
    }
    
    if (!my_lsm6dso_init()) {
        SENSORS_ERROR("Failed to initialize lsm6dso accelerometer and gyroscope sensor\n");
    } else {
        SENSORS_INFO("LSM6DSO initialized successfully\n");
    }
    
    if (!my_lis2mdl_init()) {
        SENSORS_ERROR("Failed to initialize lis2mdl magnetometer sensor\n");
    } else {
        SENSORS_INFO("LIS2MDL initialized successfully\n");
    }
}

void sensors_run(void *p1, void *p2, void *p3)
{
    SENSORS_INFO("Starting sensors thread\n");
    
    /* Wait for UI to be ready before starting sensors */
    SENSORS_VERBOSE("Waiting for UI ready semaphore...\n");
    if (k_sem_take(&ui_ready_sem, K_SECONDS(5)) != 0) {
        SENSORS_ERROR("Timed out waiting for UI ready semaphore, proceeding anyway\n");
    } else {
        SENSORS_INFO("UI ready semaphore taken, initializing sensors\n");
    }
    
    /* Initialize sensors */
    sensors_init();
    
    SENSORS_INFO("Sensors initialized successfully\n");
    
    /* Run sensor loop */
    while (1) {
        /* Poll all sensors */
        SENSORS_VERBOSE("Polling sensors\n");
        
        hts221_sample();
        lis2mdl_sample();
        lsm6dso_sample();
        
        /* Poll once per second */
        k_sleep(K_SECONDS(1));
    }
}