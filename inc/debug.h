#ifndef DEBUG_H_
#define DEBUG_H_

#include <zephyr/kernel.h>

/* Debug level configuration */
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 1  /* Default debug level if not defined elsewhere */
#endif

/* Debug level definitions */
#define DEBUG_NONE    0  /* No debug output */
#define DEBUG_ERROR   1  /* Only errors */
#define DEBUG_INFO    2  /* Errors and general info */
#define DEBUG_VERBOSE 3  /* All debug information */

/* Debug print macros with module tag parameter */
#define DEBUG_ERROR_PRINT(module, fmt, ...) \
    do { if (DEBUG_LEVEL >= DEBUG_ERROR) \
        printk("[%s][ERROR] " fmt, module, ##__VA_ARGS__); } while (0)

#define DEBUG_INFO_PRINT(module, fmt, ...) \
    do { if (DEBUG_LEVEL >= DEBUG_INFO) \
        printk("[%s][INFO] " fmt, module, ##__VA_ARGS__); } while (0)

#define DEBUG_VERBOSE_PRINT(module, fmt, ...) \
    do { if (DEBUG_LEVEL >= DEBUG_VERBOSE) \
        printk("[%s][VERBOSE] " fmt, module, ##__VA_ARGS__); } while (0)

/* Debug print macros for specific modules */
#define SENSORS_ERROR(fmt, ...)   DEBUG_ERROR_PRINT("SENSORS", fmt, ##__VA_ARGS__)
#define SENSORS_INFO(fmt, ...)    DEBUG_INFO_PRINT("SENSORS", fmt, ##__VA_ARGS__)
#define SENSORS_VERBOSE(fmt, ...) DEBUG_VERBOSE_PRINT("SENSORS", fmt, ##__VA_ARGS__)

#define UI_ERROR(fmt, ...)        DEBUG_ERROR_PRINT("UI", fmt, ##__VA_ARGS__)
#define UI_INFO(fmt, ...)         DEBUG_INFO_PRINT("UI", fmt, ##__VA_ARGS__)
#define UI_VERBOSE(fmt, ...)      DEBUG_VERBOSE_PRINT("UI", fmt, ##__VA_ARGS__)

#define MAIN_ERROR(fmt, ...)      DEBUG_ERROR_PRINT("MAIN", fmt, ##__VA_ARGS__)
#define MAIN_INFO(fmt, ...)       DEBUG_INFO_PRINT("MAIN", fmt, ##__VA_ARGS__)
#define MAIN_VERBOSE(fmt, ...)    DEBUG_VERBOSE_PRINT("MAIN", fmt, ##__VA_ARGS__)

#define LIS2MDL_ERROR(fmt, ...)   DEBUG_ERROR_PRINT("LIS2MDL", fmt, ##__VA_ARGS__)
#define LIS2MDL_INFO(fmt, ...)    DEBUG_INFO_PRINT("LIS2MDL", fmt, ##__VA_ARGS__)
#define LIS2MDL_VERBOSE(fmt, ...) DEBUG_VERBOSE_PRINT("LIS2MDL", fmt, ##__VA_ARGS__)

#define HTS221_ERROR(fmt, ...)   DEBUG_ERROR_PRINT("HTS221", fmt, ##__VA_ARGS__)
#define HTS221_INFO(fmt, ...)    DEBUG_INFO_PRINT("HTS221", fmt, ##__VA_ARGS__)
#define HTS221_VERBOSE(fmt, ...) DEBUG_VERBOSE_PRINT("HTS221", fmt, ##__VA_ARGS__)

#define LSM6DSO_ERROR(fmt, ...)   DEBUG_ERROR_PRINT("LSM6DSO", fmt, ##__VA_ARGS__)
#define LSM6DSO_INFO(fmt, ...)    DEBUG_INFO_PRINT("LSM6DSO", fmt, ##__VA_ARGS__)
#define LSM6DSO_VERBOSE(fmt, ...) DEBUG_VERBOSE_PRINT("LSM6DSO", fmt, ##__VA_ARGS__)

#define MAIN_ERROR(fmt, ...)   DEBUG_ERROR_PRINT("MAIN", fmt, ##__VA_ARGS__)
#define MAIN_INFO(fmt, ...)    DEBUG_INFO_PRINT("MAIN", fmt, ##__VA_ARGS__)
#define MAIN_VERBOSE(fmt, ...) DEBUG_VERBOSE_PRINT("MAIN", fmt, ##__VA_ARGS__)

#define BUTTON_ERROR(fmt, ...)   DEBUG_ERROR_PRINT("BUTTON", fmt, ##__VA_ARGS__)
#define BUTTON_INFO(fmt, ...)    DEBUG_INFO_PRINT("BUTTON", fmt, ##__VA_ARGS__)
#define BUTTON_VERBOSE(fmt, ...) DEBUG_VERBOSE_PRINT("BUTTON", fmt, ##__VA_ARGS__)

#define RTC_ERROR(fmt, ...)   DEBUG_ERROR_PRINT("RTC", fmt, ##__VA_ARGS__)
#define RTC_INFO(fmt, ...)    DEBUG_INFO_PRINT("RTC", fmt, ##__VA_ARGS__)
#define RTC_VERBOSE(fmt, ...) DEBUG_VERBOSE_PRINT("RTC", fmt, ##__VA_ARGS__)

#endif /* DEBUG_H_ */