#ifndef TIMSEC_H
#define TIMSEC_H

#include <zephyr/kernel.h>

/* Function to start the timer */
void start_timer(void);

/* Function to get the current value of temps */
uint32_t get_temps(void);
void format_time(uint32_t seconds, char *buffer, size_t buffer_size);

#endif // TIMSEC_H