#include "../../inc/timSec.h"
#include "../../inc/rtc.h"
#include "../../inc/debug.h"
#include "../../inc/sensors.h"
#include <zephyr/kernel.h>

// Variables pour le compteur de secours
static bool rtc_available = false;
static struct k_timer fake_timer;

// Suppression de fake_time et utilisation des variables globales
static void fake_timer_expiry(struct k_timer *timer)
{
    increment_time(); // Appeler increment_time chaque seconde

    if (g_current_hour == 0 && g_current_minute == 0 && g_current_second == 0) {
        extern sensor_data_t g_sensor_data;
        g_sensor_data.step_count = 0;  // Réinitialiser le compteur de pas à minuit
        
        // Réinitialiser également le chronomètre d'activité
        extern void ui_reset_activity_chrono(void);
        ui_reset_activity_chrono();
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

    }
    // Initialiser le timer de secours (1 seconde d'intervalle)
    k_timer_init(&fake_timer, fake_timer_expiry, NULL);
    k_timer_start(&fake_timer, K_SECONDS(1), K_SECONDS(1));
}

/* Function to get the current time in seconds */
uint32_t get_temps(void)
{
    // Utiliser les variables globales pour calculer le temps en secondes
    return g_current_hour * 3600 + g_current_minute * 60 + g_current_second;
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
        // Définir l'heure sur les variables globales
        g_current_day = day;
        g_current_month = month;
        g_current_year = year;
        g_current_hour = hour;
        g_current_minute = minute;
        g_current_second = second;
        DEBUG_INFO_PRINT("TIMER", "Heure définie sur les variables globales: %02d:%02d:%02d\n", hour, minute, second);
        return 0;
    }
}