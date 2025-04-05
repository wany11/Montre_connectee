#ifndef RTC_H_
#define RTC_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/rtc.h>

/**
 * @brief Initialise la RTC RV8263
 * 
 * @return int 0 si succès, code d'erreur sinon
 */
int rtc_init(void);

/**
 * @brief Défini l'heure actuelle
 * 
 * @param year Année (2000-2099)
 * @param month Mois (1-12)
 * @param day Jour (1-31)
 * @param hour Heure (0-23)
 * @param minute Minute (0-59)
 * @param second Seconde (0-59)
 * @return int 0 si succès, code d'erreur sinon
 */
int rtc_set_datetime(uint16_t year, uint8_t month, uint8_t day,
                     uint8_t hour, uint8_t minute, uint8_t second);

/**
 * @brief Récupère l'heure actuelle
 * 
 * @param year Pointeur pour stocker l'année
 * @param month Pointeur pour stocker le mois
 * @param day Pointeur pour stocker le jour
 * @param hour Pointeur pour stocker l'heure
 * @param minute Pointeur pour stocker les minutes
 * @param second Pointeur pour stocker les secondes
 * @return int 0 si succès, code d'erreur sinon
 */
int rtc_get_datetime(uint16_t *year, uint8_t *month, uint8_t *day,
                     uint8_t *hour, uint8_t *minute, uint8_t *second);

#endif /* RTC_H_ */