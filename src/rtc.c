#include "../inc/rtc.h"
#include "../inc/debug.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/rtc.h>

#define RTC_ERROR(fmt, ...)   DEBUG_ERROR_PRINT("RTC", fmt, ##__VA_ARGS__)
#define RTC_INFO(fmt, ...)    DEBUG_INFO_PRINT("RTC", fmt, ##__VA_ARGS__)
#define RTC_VERBOSE(fmt, ...) DEBUG_VERBOSE_PRINT("RTC", fmt, ##__VA_ARGS__)

static const struct device *rtc_dev;



int rtc_init(void)
{
    /* Récupérer l'appareil RTC */
    rtc_dev = DEVICE_DT_GET(DT_NODELABEL(rv_8263));
    if (!device_is_ready(rtc_dev)) {
        RTC_ERROR("RV8263 n'est pas prêt");
        return -ENODEV;
    }

    RTC_INFO("RTC RV8263 initialisée avec succès");
    return 0;
}

int rtc_set_datetime(uint16_t year, uint8_t month, uint8_t day,
                     uint8_t hour, uint8_t minute, uint8_t second)
{
    if (rtc_dev == NULL) {
        return -ENODEV;
    }

    struct rtc_time time;
    time.tm_year = year - 1900;  // RTC API attend les années depuis 1900
    time.tm_mon = month - 1;     // RTC API attend les mois 0-11
    time.tm_mday = day;
    time.tm_hour = hour;
    time.tm_min = minute;
    time.tm_sec = second;

    int ret = rtc_set_time(rtc_dev, &time);
    if (ret < 0) {
        RTC_ERROR("Impossible de définir l'heure: %d", ret);
        return ret;
    }

    RTC_INFO("Heure définie: %04d-%02d-%02d %02d:%02d:%02d",
            year, month, day, hour, minute, second);
    return 0;
}

int rtc_get_datetime(uint16_t *year, uint8_t *month, uint8_t *day,
                     uint8_t *hour, uint8_t *minute, uint8_t *second)
{
    if (rtc_dev == NULL) {
        return -ENODEV;
    }

    struct rtc_time time;
    int ret = rtc_get_time(rtc_dev, &time);
    if (ret < 0) {
        RTC_ERROR("Impossible de lire l'heure: %d", ret);
        return ret;
    }

    *year = time.tm_year + 1900;  // L'API RTC renvoie les années depuis 1900
    *month = time.tm_mon + 1;     // L'API RTC renvoie les mois 0-11
    *day = time.tm_mday;
    *hour = time.tm_hour;
    *minute = time.tm_min;
    *second = time.tm_sec;

    RTC_INFO("Heure actuelle: %04d-%02d-%02d %02d:%02d:%02d",
            *year, *month, *day, *hour, *minute, *second);
    return 0;
}