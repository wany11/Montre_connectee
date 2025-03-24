#ifndef TEMP_QUEUE_H
#define TEMP_QUEUE_H

#include <zephyr/kernel.h>

/* Define message types */
typedef enum {
    MSG_TYPE_TEMPERATURE,
    MSG_TYPE_HUMIDITY,
    MSG_TYPE_ACCEL_X,
    MSG_TYPE_ACCEL_Y,
    MSG_TYPE_ACCEL_Z,
    MSG_TYPE_GYRO_X,
    MSG_TYPE_GYRO_Y,
    MSG_TYPE_GYRO_Z,
    MSG_TYPE_MAG_X,
    MSG_TYPE_MAG_Y,
    MSG_TYPE_MAG_Z,
} sensor_msg_type_t;

/* Message structure to hold both value and type */
typedef struct {
    sensor_msg_type_t type;
    double value;
} sensor_msg_t;

/**
 * @brief Get the pointer to the sensor message queue
 * 
 * @return Pointer to the sensor message queue
 */
struct k_msgq *get_msgq(void);

#endif /* TEMP_QUEUE_H */