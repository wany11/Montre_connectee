#include "../../inc/timSec.h"

static uint32_t temps = 0; // Variable to store the time in seconds

/* Timer callback function */
static void timer_handler(struct k_timer *dummy)
{
    temps++;
}

/* Zephyr timer definition */
K_TIMER_DEFINE(sec_timer, timer_handler, NULL);

/* Function to start the timer */
void start_timer(void)
{
    k_timer_start(&sec_timer, K_SECONDS(1), K_SECONDS(1));
}

/* Function to get the current value of temps */
uint32_t get_temps(void)
{
    return temps;
}

void format_time(uint32_t seconds, char *buffer, size_t buffer_size)
{
    uint32_t hours = (seconds / 3600) % 24; // Convertir en heures (remise à 0 après 24h)
    uint32_t minutes = (seconds / 60) % 60; // Convertir en minutes
    uint32_t secs = seconds % 60;           // Récupérer les secondes restantes

    snprintf(buffer, buffer_size, "%02u:%02u:%02u", hours, minutes, secs);
}