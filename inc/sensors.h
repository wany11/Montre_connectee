#ifndef SENSORS_H
#define SENSORS_H

#include <zephyr/kernel.h>
#include <stdbool.h>
#include "queue.h"

/* Sensor data structure */
typedef struct {
    double temperature;  /* Temperature reading */
    double humidity;     /* Humidity reading */
    double accel_x;      /* Accelerometer X-axis reading */
    double accel_y;      /* Accelerometer Y-axis reading */
    double accel_z;      /* Accelerometer Z-axis reading */
    double gyro_x;       /* Gyroscope X-axis reading */
    double gyro_y;       /* Gyroscope Y-axis reading */
    double gyro_z;       /* Gyroscope Z-axis reading */
    double mag_x;        /* Magnetometer X-axis reading */
    double mag_y;        /* Magnetometer Y-axis reading */
    double mag_z;        /* Magnetometer Z-axis reading */
    double step_count;   /* Step counter reading */
} sensor_data_t;

/**
 * @brief Store a sensor reading based on type
 * 
 * @param type Type of sensor reading (from sensor_msg_type_t)
 * @param value Value to store
 */
void store_sensor_reading(sensor_msg_type_t type, double value);

/**
 * @brief Store a temperature reading (backward compatibility)
 * 
 * @param temperature Temperature value to store
 */
void store_reading(double temperature);

/**
 * @brief Initialize all sensors
 */
void sensors_init(void);

/**
 * @brief Sensors thread entry point
 */
void sensors_run(void *p1, void *p2, void *p3);

/**
 * @brief Get HTS221 semaphore for signaling
 * 
 * @return Pointer to the HTS221 semaphore
 */
struct k_sem *get_hts221_sem(void);

/**
 * @brief Get LIS2MDL semaphore for signaling
 * 
 * @return Pointer to the LIS2MDL semaphore
 */
struct k_sem *get_lis2mdl_sem(void);

/**
 * @brief Get LSM6DSO semaphore for signaling
 * 
 * @return Pointer to the LSM6DSO semaphore
 */
struct k_sem *get_lsm6dso_sem(void);

#endif /* SENSORS_H */