#include <zephyr/kernel.h>
#include "../../inc/sensors.h"
#include "../../inc/debug.h"
#include "../../inc/sensorTimers.h"

/* Define timers for each sensor */
static struct k_timer hts221_timer;
static struct k_timer lis2mdl_timer;
static struct k_timer lsm6dso_timer;

/* HTS221 timer expiry handler - 30 second interval */
static void hts221_timer_handler(struct k_timer *timer_id)
{
    struct k_sem *sem = get_hts221_sem();
    if (sem != NULL) {
        k_sem_give(sem);
        SENSORS_VERBOSE("HTS221 timer expired - triggering temperature/humidity reading\n");
    } else {
        SENSORS_ERROR("HTS221 semaphore not available\n");
    }
}

/* LIS2MDL timer expiry handler - 5 second interval */
static void lis2mdl_timer_handler(struct k_timer *timer_id)
{
    struct k_sem *sem = get_lis2mdl_sem();
    if (sem != NULL) {
        k_sem_give(sem);
        SENSORS_VERBOSE("LIS2MDL timer expired - triggering magnetometer reading\n");
    } else {
        SENSORS_ERROR("LIS2MDL semaphore not available\n");
    }
}

/* LSM6DSO timer expiry handler - 1 second interval */
static void lsm6dso_timer_handler(struct k_timer *timer_id)
{
    struct k_sem *sem = get_lsm6dso_sem();
    if (sem != NULL) {
        k_sem_give(sem);
        SENSORS_VERBOSE("LSM6DSO timer expired - triggering accelerometer/gyroscope reading\n");
    } else {
        SENSORS_ERROR("LSM6DSO semaphore not available\n");
    }
}

void init_sensor_timers(void)
{
    /* Initialize all sensor timers with their respective callbacks */
    k_timer_init(&hts221_timer, hts221_timer_handler, NULL);
    k_timer_init(&lis2mdl_timer, lis2mdl_timer_handler, NULL);
    k_timer_init(&lsm6dso_timer, lsm6dso_timer_handler, NULL);
    
    SENSORS_INFO("All sensor timers initialized\n");
}

void start_sensor_timers(void)
{
    /* Start HTS221 timer with 30-second intervals */
    k_timer_start(&hts221_timer, K_SECONDS(1), K_SECONDS(30));
    SENSORS_INFO("HTS221 timer started with 30-second intervals\n");
    
    /* Start LIS2MDL timer with 5-second intervals */
    k_timer_start(&lis2mdl_timer, K_SECONDS(2), K_SECONDS(5));
    SENSORS_INFO("LIS2MDL timer started with 5-second intervals\n");
    
    /* Start LSM6DSO timer with 1-second intervals */
    k_timer_start(&lsm6dso_timer, K_SECONDS(3), K_SECONDS(5));
    SENSORS_INFO("LSM6DSO timer started with 1-second intervals\n");
}

void stop_sensor_timers(void)
{
    /* Stop all timers */
    k_timer_stop(&hts221_timer);
    k_timer_stop(&lis2mdl_timer);
    k_timer_stop(&lsm6dso_timer);
    
    SENSORS_INFO("All sensor timers stopped\n");
}