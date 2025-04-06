#ifndef SENSOR_TIMERS_H
#define SENSOR_TIMERS_H

/**
 * @brief Initialize sensor timers
 * 
 * Initializes timers for periodic sensor sampling
 */
void init_sensor_timers(void);

/**
 * @brief Start all sensor timers
 * 
 * Starts timers for periodic sensor sampling with appropriate intervals
 */
void start_sensor_timers(void);

/**
 * @brief Stop all sensor timers
 * 
 * Stops all sensor sampling timers
 */
void stop_sensor_timers(void);

#endif /* SENSOR_TIMERS_H */