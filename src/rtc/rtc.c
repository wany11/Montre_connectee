#include "../../inc/rtc.h"
#include "../../inc/debug.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/util.h>  // Inclure explicitement pour bin2bcd et bcd2bin

static const struct device *rtc_dev;
static const struct device *i2c_dev;

#define RV8263_I2C_ADDR 0x51  // Adresse I2C du RV-8263
// Adresses des registres corrects du RV-8263-C8
#define RV8263_REG_SECONDS    0x04
#define RV8263_REG_MINUTES    0x05
#define RV8263_REG_HOURS      0x06
#define RV8263_REG_DAYS       0x07
#define RV8263_REG_WEEKDAYS   0x08
#define RV8263_REG_MONTHS     0x09
#define RV8263_REG_YEARS      0x0A
#define RV8263_REG_CONTROL1   0x0F
#define RV8263_REG_CONTROL2   0x10
#define RV8263_REG_STATUS     0x11
#define RV8263_REG_CONTROL3   0x12

/* Définition des variables globales */
uint16_t g_current_year = 2023;
uint8_t g_current_month = 1;
uint8_t g_current_day = 1;
uint8_t g_current_hour = 0;
uint8_t g_current_minute = 0;
uint8_t g_current_second = 0;

/* Met à jour les variables globales en appelant rtc_get_datetime */
int rtc_update_global_time(void)
{
    return rtc_get_datetime(&g_current_year, &g_current_month, &g_current_day, 
                           &g_current_hour, &g_current_minute, &g_current_second);
}

/* Fonction alias pour résoudre l'erreur de build */
int rtc_get_datetimes(uint16_t *year, uint8_t *month, uint8_t *day,
                      uint8_t *hour, uint8_t *minute, uint8_t *second)
{
    return rtc_get_datetime(year, month, day, hour, minute, second);
}

int rtc_init(void)
{
    /* Récupérer l'appareil RTC via le nœud DT */
    rtc_dev = DEVICE_DT_GET(DT_NODELABEL(rv_8263));
    if (!device_is_ready(rtc_dev)) {
        RTC_ERROR("RV8263 n'est pas prêt\n");
        return -ENODEV;
    }
    
    /* Récupérer le bus I2C associé au RTC */
    i2c_dev = DEVICE_DT_GET(DT_BUS(DT_NODELABEL(rv_8263)));
    if (!device_is_ready(i2c_dev)) {
        RTC_ERROR("Bus I2C n'est pas prêt\n");
        return -ENODEV;
    }

    RTC_INFO("RTC RV8263 initialisée avec succès\n");
    return 0;
}

int rtc_set_datetime(uint16_t year, uint8_t month, uint8_t day,
                     uint8_t hour, uint8_t minute, uint8_t second)
{
    if (i2c_dev == NULL) {
        return -ENODEV;
    }

    int ret;
    uint8_t data;

    // Conversion en BCD et écriture des registres
    data = bin2bcd(second);
    ret = i2c_reg_write_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_SECONDS, data);
    if (ret < 0) {
        RTC_ERROR("Impossible d'écrire les secondes: %d\n", ret);
        return ret;
    }

    data = bin2bcd(minute);
    ret = i2c_reg_write_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_MINUTES, data);
    if (ret < 0) {
        RTC_ERROR("Impossible d'écrire les minutes: %d\n", ret);
        return ret;
    }

    data = bin2bcd(hour);
    ret = i2c_reg_write_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_HOURS, data);
    if (ret < 0) {
        RTC_ERROR("Impossible d'écrire les heures: %d\n", ret);
        return ret;
    }

    data = bin2bcd(day);
    ret = i2c_reg_write_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_DAYS, data);
    if (ret < 0) {
        RTC_ERROR("Impossible d'écrire le jour: %d\n", ret);
        return ret;
    }

    data = bin2bcd(month);
    ret = i2c_reg_write_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_MONTHS, data);
    if (ret < 0) {
        RTC_ERROR("Impossible d'écrire le mois: %d\n", ret);
        return ret;
    }

    data = bin2bcd(year % 100);  // Uniquement les 2 derniers chiffres
    ret = i2c_reg_write_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_YEARS, data);
    if (ret < 0) {
        RTC_ERROR("Impossible d'écrire l'année: %d\n", ret);
        return ret;
    }

    RTC_INFO("Heure définie: %04d-%02d-%02d %02d:%02d:%02d\n",
            year, month, day, hour, minute, second);
    return 0;
}

int rtc_get_datetime(uint16_t *year, uint8_t *month, uint8_t *day,
                     uint8_t *hour, uint8_t *minute, uint8_t *second)
{
    if (i2c_dev == NULL) {
        return -ENODEV;
    }

    int ret;
    uint8_t data;

    // Lecture des secondes
    ret = i2c_reg_read_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_SECONDS, &data);
    if (ret < 0) {
        RTC_ERROR("Impossible de lire les secondes: %d\n", ret);
        return ret;
    }
    *second = bcd2bin(data & 0x7F);  // Masquer bit de gestion

    // Lecture des minutes
    ret = i2c_reg_read_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_MINUTES, &data);
    if (ret < 0) {
        RTC_ERROR("Impossible de lire les minutes: %d\n", ret);
        return ret;
    }
    *minute = bcd2bin(data & 0x7F);

    // Lecture des heures
    ret = i2c_reg_read_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_HOURS, &data);
    if (ret < 0) {
        RTC_ERROR("Impossible de lire les heures: %d\n", ret);
        return ret;
    }
    *hour = bcd2bin(data & 0x3F);

    // Lecture du jour
    ret = i2c_reg_read_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_DAYS, &data);
    if (ret < 0) {
        RTC_ERROR("Impossible de lire le jour: %d\n", ret);
        return ret;
    }
    *day = bcd2bin(data & 0x3F);

    // Lecture du mois
    ret = i2c_reg_read_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_MONTHS, &data);
    if (ret < 0) {
        RTC_ERROR("Impossible de lire le mois: %d\n", ret);
        return ret;
    }
    *month = bcd2bin(data & 0x1F);

    // Lecture de l'année
    ret = i2c_reg_read_byte(i2c_dev, RV8263_I2C_ADDR, RV8263_REG_YEARS, &data);
    if (ret < 0) {
        RTC_ERROR("Impossible de lire l'année: %d\n", ret);
        return ret;
    }
    // Année 20xx
    *year = 2000 + bcd2bin(data);

    // Mettre à jour les variables globales en même temps
    g_current_year = *year;
    g_current_month = *month;
    g_current_day = *day;
    g_current_hour = *hour;
    g_current_minute = *minute;
    g_current_second = *second;

    RTC_INFO("Heure actuelle: %04d-%02d-%02d %02d:%02d:%02d\n",
            *year, *month, *day, *hour, *minute, *second);
    return 0;
}


