#include <zephyr/kernel.h>
#include "../../inc/sensors.h"
#include "../../inc/debug.h"
#include "../../inc/sensorTimers.h"

/* Flags to control task execution */
static volatile bool sensors_running = false;

/* Task thread IDs */
static k_tid_t hts221_task_tid;
static k_tid_t lis2mdl_task_tid;
static k_tid_t lsm6dso_task_tid;

/* Task thread data */
static struct k_thread hts221_task_data;
static struct k_thread lis2mdl_task_data;
static struct k_thread lsm6dso_task_data;

/* Task thread stacks */
#define TASK_STACK_SIZE 1024
K_THREAD_STACK_DEFINE(hts221_task_stack, TASK_STACK_SIZE);
K_THREAD_STACK_DEFINE(lis2mdl_task_stack, TASK_STACK_SIZE);
K_THREAD_STACK_DEFINE(lsm6dso_task_stack, TASK_STACK_SIZE);

/* HTS221 task function - 30 second interval */
static void hts221_task_func(void *p1, void *p2, void *p3)
{
    SENSORS_INFO("HTS221 task thread started\n");
    
    while (sensors_running) {
        struct k_sem *sem = get_hts221_sem();
        if (sem != NULL) {
            k_sem_give(sem);
            SENSORS_VERBOSE("HTS221 task triggered temperature/humidity reading\n");
        } else {
            SENSORS_ERROR("HTS221 semaphore not available\n");
        }
        
        /* Sleep for 30 seconds before next trigger */
        k_sleep(K_SECONDS(30));
    }
    
    SENSORS_INFO("HTS221 task thread exited\n");
}

/* LIS2MDL task function - 1 second interval */
static void lis2mdl_task_func(void *p1, void *p2, void *p3)
{
    SENSORS_INFO("LIS2MDL task thread started\n");
    
    while (sensors_running) {
        struct k_sem *sem = get_lis2mdl_sem();
        if (sem != NULL) {
            k_sem_give(sem);
            SENSORS_VERBOSE("LIS2MDL task triggered magnetometer reading\n");
        } else {
            SENSORS_ERROR("LIS2MDL semaphore not available\n");
        }
        
        /* Sleep for 1 second before next trigger */
        k_sleep(K_SECONDS(1));
    }
    
    SENSORS_INFO("LIS2MDL task thread exited\n");
}

/* LSM6DSO task function - 250ms interval */
static void lsm6dso_task_func(void *p1, void *p2, void *p3)
{
    SENSORS_INFO("LSM6DSO task thread started\n");
    
    while (sensors_running) {
        struct k_sem *sem = get_lsm6dso_sem();
        if (sem != NULL) {
            k_sem_give(sem);
            SENSORS_VERBOSE("LSM6DSO task triggered accelerometer/gyroscope reading\n");
        } else {
            SENSORS_ERROR("LSM6DSO semaphore not available\n");
        }
        
        /* Sleep for 250ms before next trigger */
        k_sleep(K_MSEC(250));
    }
    
    SENSORS_INFO("LSM6DSO task thread exited\n");
}

void init_sensor_timers(void)
{
    /* No initialization needed for tasks until start_sensor_timers is called */
    SENSORS_INFO("Sensor tasks ready for initialization\n");
}

void start_sensor_timers(void)
{
    /* Set flag to indicate tasks should run */
    sensors_running = true;
    
    /* Create and start HTS221 task with initial 1 second delay */
    hts221_task_tid = k_thread_create(&hts221_task_data, 
                                     hts221_task_stack,
                                     K_THREAD_STACK_SIZEOF(hts221_task_stack),
                                     hts221_task_func,
                                     NULL, NULL, NULL,
                                     5, 0, K_SECONDS(1));
    k_thread_name_set(hts221_task_tid, "hts221_task");
    SENSORS_INFO("HTS221 task started with 30-second intervals\n");
    
    /* Create and start LIS2MDL task with initial 2 second delay */
    lis2mdl_task_tid = k_thread_create(&lis2mdl_task_data, 
                                      lis2mdl_task_stack,
                                      K_THREAD_STACK_SIZEOF(lis2mdl_task_stack),
                                      lis2mdl_task_func,
                                      NULL, NULL, NULL,
                                      5, 0, K_SECONDS(2));
    k_thread_name_set(lis2mdl_task_tid, "lis2mdl_task");
    SENSORS_INFO("LIS2MDL task started with 1-second intervals\n");
    
    /* Create and start LSM6DSO task with initial 3 second delay */
    lsm6dso_task_tid = k_thread_create(&lsm6dso_task_data, 
                                      lsm6dso_task_stack,
                                      K_THREAD_STACK_SIZEOF(lsm6dso_task_stack),
                                      lsm6dso_task_func,
                                      NULL, NULL, NULL,
                                      5, 0, K_SECONDS(3));
    k_thread_name_set(lsm6dso_task_tid, "lsm6dso_task");
    SENSORS_INFO("LSM6DSO task started with 250ms intervals\n");
}

void stop_sensor_timers(void)
{
    /* Signal tasks to stop */
    sensors_running = false;
    
    /* Give threads time to exit their loops */
    k_sleep(K_MSEC(500));
    
    /* Abort task threads */
    k_thread_abort(hts221_task_tid);
    k_thread_abort(lis2mdl_task_tid);
    k_thread_abort(lsm6dso_task_tid);
    
    SENSORS_INFO("All sensor tasks stopped\n");
}