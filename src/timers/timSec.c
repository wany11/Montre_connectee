#include "../../inc/timSec.h"
#include "../../inc/rtc.h"
#include "../../inc/debug.h"
#include "../../inc/sensors.h"
#include <zephyr/kernel.h>

// Variables pour le compteur de secours
static bool rtc_available = false;
static uint32_t fake_time = 56730;  // Temps en secondes depuis minuit
static struct k_timer fake_timer;

// Fonction de callback pour le timer de secours
static void fake_timer_expiry(struct k_timer *timer)
{
    fake_time++;
    if (fake_time >= 24 * 3600) {
        extern sensor_data_t g_sensor_data;
        g_sensor_data.step_count = 0;  // Réinitialiser le compteur de pas
        fake_time = 0;  // Réinitialiser à minuit après 24h
    }
}

/* Function to initialize time service */
void start_timer(void)
{
    // Tenter d'initialiser le RTC
    int ret = rtc_init();
    if (ret == 0) {
        rtc_available = true;
        DEBUG_INFO_PRINT("TIMER", "RTC détectée et initialisée avec succès\n");
    } else {
        rtc_available = false;
        DEBUG_ERROR_PRINT("TIMER", "RTC non détectée, utilisation du compteur de secours\n");
        
        // Initialiser le timer de secours (1 seconde d'intervalle)
        k_timer_init(&fake_timer, fake_timer_expiry, NULL);
        k_timer_start(&fake_timer, K_SECONDS(1), K_SECONDS(1));
    }
}

/* Function to get the current time in seconds */
uint32_t get_temps(void)
{
    if (rtc_available) {
        uint16_t year;
        uint8_t month, day, hour, minute, second;
        
        // Obtenir l'heure actuelle depuis le RTC
        int ret = rtc_get_datetime(&year, &month, &day, &hour, &minute, &second);
        if (ret < 0) {
            DEBUG_ERROR_PRINT("TIMER", "Erreur lors de la lecture RTC set de l'heure système: %d\n", ret);
            return fake_time;  // Utiliser le temps de secours en cas d'erreur
        }
        
        // Convertir en secondes depuis minuit
        return hour * 3600 + minute * 60 + second;
    } else {
        // Utiliser le compteur de secours
        return fake_time;
    }
}

void format_time(uint32_t seconds, char *buffer, size_t buffer_size)
{
    uint32_t hours = (seconds / 3600); // Convertir en heures (remise à 0 après 24h)
    uint32_t minutes = (seconds / 60) % 60; // Convertir en minutes
    uint32_t secs = seconds % 60;           // Récupérer les secondes restantes

    snprintf(buffer, buffer_size, "%02u:%02u:%02u", hours, minutes, secs);
}

/* Function to set the current time */
int set_time(uint8_t hour, uint8_t minute, uint8_t second,  uint16_t year, uint8_t month, uint8_t day)
{
    if (rtc_available) {
        // Obtenir d'abord la date actuelle pour ne pas la modifier
        uint8_t old_hour, old_minute, old_second;
        
        int ret = rtc_get_datetime(&year, &month, &day, &old_hour, &old_minute, &old_second);
        if (ret < 0) {
            DEBUG_ERROR_PRINT("TIMER", "Erreur lors de la lecture RTC pour réglage: %d\n", ret);
            return ret;
        }
        
        // Définir la nouvelle heure tout en gardant la même date
        return rtc_set_datetime(year, month, day, hour, minute, second);
    } else {
        // Définir l'heure sur le compteur de secours
        g_current_day = day;
        g_current_month = month;
        g_current_year = year;
        fake_time = hour * 3600 + minute * 60 + second;
        DEBUG_INFO_PRINT("TIMER", "Heure définie sur le compteur de secours: %02d:%02d:%02d\n", 
                         hour, minute, second);
        return 0;
    }
}