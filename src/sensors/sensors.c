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
#include "../../inc/sensorTimers.h"

/* Global sensor data structure */
sensor_data_t g_sensor_data = {
    .accel_x = 0.0,
    .accel_y = 0.0,
    .accel_z = 0.0,
    .gyro_x = 0.0,
    .gyro_y = 0.0,
    .gyro_z = 0.0,
    .mag_x = 0.0,
    .mag_y = 0.0,
    .mag_z = 0.0,
    .temperature = 0.0,
    .humidity = 0.0
};

/* Semaphore for UI readiness - declared in main.c */
extern struct k_sem ui_ready_sem;

static struct k_sem hts221_sem;
static struct k_sem lis2mdl_sem;
static struct k_sem lsm6dso_sem;

/* Define thread stacks */
#define STACK_SIZE 1024
K_THREAD_STACK_DEFINE(hts221_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(lis2mdl_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(lsm6dso_stack, STACK_SIZE);

/* Thread data structures */
static struct k_thread hts221_thread_data;
static struct k_thread lis2mdl_thread_data;
static struct k_thread lsm6dso_thread_data;

/* Thread IDs */
static k_tid_t hts221_tid;
static k_tid_t lis2mdl_tid;
static k_tid_t lsm6dso_tid;

/* Thread functions */
static void hts221_thread_func(void *p1, void *p2, void *p3)
{
    while (1) {
        /* Wait for timer signal via semaphore */
        k_sem_take(&hts221_sem, K_FOREVER);
        
        SENSORS_VERBOSE("Starting HTS221 sampling\n");
        hts221_sample();
        
        SENSORS_VERBOSE("HTS221 sampling complete\n");
    }
}

static void lis2mdl_thread_func(void *p1, void *p2, void *p3)
{
    while (1) {
        /* Wait for timer signal via semaphore */
        k_sem_take(&lis2mdl_sem, K_FOREVER);
        
        SENSORS_VERBOSE("Starting LIS2MDL sampling\n");
        lis2mdl_sample();
        
        SENSORS_VERBOSE("LIS2MDL sampling complete\n");
    }
}

static void lsm6dso_thread_func(void *p1, void *p2, void *p3)
{
    while (1) {
        /* Wait for timer signal via semaphore */
        k_sem_take(&lsm6dso_sem, K_FOREVER);
        
        SENSORS_VERBOSE("Starting LSM6DSO sampling\n");
        lsm6dso_sample();
        
        SENSORS_VERBOSE("LSM6DSO sampling complete\n");
    }
}

void store_sensor_reading(sensor_msg_type_t type, double value)
{
    /* Handle reading based on sensor type */
    switch (type) {
    case MSG_TYPE_TEMPERATURE:
        /* Store the latest temperature */
        g_sensor_data.temperature = value;
        SENSORS_VERBOSE("Storing temperature reading: %.2f°C\n", value);
        break;
        
    case MSG_TYPE_HUMIDITY:
        /* Store the latest humidity */
        g_sensor_data.humidity = value;
        SENSORS_VERBOSE("Storing humidity reading: %.2f%%\n", value);
        break;
        
    case MSG_TYPE_ACCEL_X: g_sensor_data.accel_x = value;
    case MSG_TYPE_ACCEL_Y: g_sensor_data.accel_y = value;
    case MSG_TYPE_ACCEL_Z: g_sensor_data.accel_z = value;
        /* Store accelerometer data */
        SENSORS_VERBOSE("Storing accelerometer reading (axis %d): %.2f\n", 
                      type - MSG_TYPE_ACCEL_X, value);
        break;
        
    case MSG_TYPE_GYRO_X: g_sensor_data.gyro_x = value;
    case MSG_TYPE_GYRO_Y: g_sensor_data.gyro_y = value;
    case MSG_TYPE_GYRO_Z: g_sensor_data.gyro_z = value;
        /* Store gyroscope data */
        SENSORS_VERBOSE("Storing gyroscope reading (axis %d): %.2f\n", 
                      type - MSG_TYPE_GYRO_X, value);
        break;
        
    case MSG_TYPE_MAG_X: g_sensor_data.mag_x = value;
    case MSG_TYPE_MAG_Y: g_sensor_data.mag_y = value;
    case MSG_TYPE_MAG_Z: g_sensor_data.mag_z = value;
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
        if (ret != 0) {
            SENSORS_VERBOSE("Queue full, purging oldest message\n");
            k_msgq_purge(msgq); // Clear the queue
            ret = k_msgq_put(msgq, &msg, K_NO_WAIT); // Try again
            if (ret != 0) {
                SENSORS_ERROR("Failed to send message to queue even after purge: %d\n", ret);
            }
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

void sensors_init(void)
{    
    SENSORS_INFO("Initializing sensors\n");
    
    /* Initialize semaphores for sensor synchronization */
    k_sem_init(&hts221_sem, 0, 1);
    k_sem_init(&lis2mdl_sem, 0, 1);
    k_sem_init(&lsm6dso_sem, 0, 1);
    
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
    
    /* Initialize sensor timers */
    init_sensor_timers();
    
    /* Create sensor threads */
    hts221_tid = k_thread_create(&hts221_thread_data, hts221_stack, STACK_SIZE,
                                hts221_thread_func, NULL, NULL, NULL,
                                5, 0, K_NO_WAIT);
    
    lis2mdl_tid = k_thread_create(&lis2mdl_thread_data, lis2mdl_stack, STACK_SIZE,
                                 lis2mdl_thread_func, NULL, NULL, NULL,
                                 5, 0, K_NO_WAIT);
                                 
    lsm6dso_tid = k_thread_create(&lsm6dso_thread_data, lsm6dso_stack, STACK_SIZE,
                                 lsm6dso_thread_func, NULL, NULL, NULL,
                                 5, 0, K_NO_WAIT);
                                 
    /* Start sensor timers */
    start_sensor_timers();
}

/* Modified sensors_run to monitor threads instead of executing them */
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
    
    /* Initialize sensors and start threads */
    sensors_init();
    
    SENSORS_INFO("Sensors initialized successfully\n");
    
    /* Main sensors thread just sleeps - timers control everything */
    while (1) {
        /* Sleep for a while */
        k_sleep(K_SECONDS(5));
        SENSORS_VERBOSE("Sensors thread monitoring - all sensors running\n");
    }
}

// Add function to get semaphore
struct k_sem *get_hts221_sem(void)
{
    return &hts221_sem;
}

// Add function to get LIS2MDL semaphore
struct k_sem *get_lis2mdl_sem(void)
{
    return &lis2mdl_sem;
}

// Add function to get LSM6DSO semaphore
struct k_sem *get_lsm6dso_sem(void)
{
    return &lsm6dso_sem;
}