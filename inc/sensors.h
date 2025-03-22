#ifndef SENSORS_H
#define SENSORS_H

#include <zephyr/kernel.h>
#include "temp_queue.h"

/* Maximum number of temperature readings to store */
#define MAX_TEMP_READINGS 10

/* Temperature reading structure */
typedef struct {
    double temperature;
    uint32_t timestamp;
} temp_reading_t;

/* Sensor data structure */
typedef struct {
    temp_reading_t readings[MAX_TEMP_READINGS];
    uint32_t count;
    uint32_t max_count;
    bool overflow;
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
 * @brief Set the latest humidity reading (backward compatibility)
 * 
 * @param humidity Humidity value to store
 */
void set_latest_humidity(double humidity);

/**
 * @brief Get the latest temperature reading
 * 
 * @param temperature Pointer to store the temperature value
 * @return true if successful, false otherwise
 */
bool get_latest_temperature(double *temperature);

/**
 * @brief Get the latest humidity reading
 * 
 * @param humidity Pointer to store the humidity value
 * @return true if successful, false otherwise
 */
bool get_latest_humidity(double *humidity);

/**
 * @brief Get the sensor data structure
 * 
 * @return Pointer to the sensor data structure
 */
const sensor_data_t* get_sensor_data(void);

/**
 * @brief Clear the sensor data
 */
void clear_sensor_data(void);

/**
 * @brief Initialize the sensors
 */
void sensors_init(void);

/**
 * @brief Sensors thread entry point
 */
void sensors_run(void *p1, void *p2, void *p3);

/* Sensor interface functions */
bool my_hts221_init(void);
void hts221_sample(void);

#endif /* SENSORS_H */