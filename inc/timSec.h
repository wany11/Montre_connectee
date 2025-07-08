#ifndef TIMSEC_H
#define TIMSEC_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Initialize the time service
 *
 * Attempts to initialize RTC. If not available, starts a fallback software timer.
 */
void start_timer(void);

/**
 * @brief Get current time in seconds
 *
 * @return Current time in seconds since midnight (0-86399)
 */
uint32_t get_temps(void);

/**
 * @brief Format time value into a string
 *
 * @param seconds Time in seconds since midnight
 * @param buffer Output buffer for formatted time
 * @param buffer_size Size of the output buffer
 */
void format_time(uint32_t seconds, char *buffer, size_t buffer_size);

/**
 * @brief Set the current time
 *
 * @param hour Hour (0-23)
 * @param minute Minute (0-59)
 * @param second Second (0-59)
 * @return 0 on success, negative error code on failure
 */
int set_time(uint8_t hour, uint8_t minute, uint8_t second, uint16_t year, uint8_t month, uint8_t day);

#endif /* TIMSEC_H */