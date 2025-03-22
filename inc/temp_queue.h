#ifndef TEMP_QUEUE_H
#define TEMP_QUEUE_H

#include <zephyr/kernel.h>

/* Define message types */
typedef enum {
    MSG_TYPE_TEMPERATURE,
    MSG_TYPE_HUMIDITY
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
struct k_msgq *get_temp_msgq(void);

#endif /* TEMP_QUEUE_H */