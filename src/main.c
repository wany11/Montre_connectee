/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */


/**********************************************************/
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>



/**************************Drivers********************************/
/* LEDs definitions */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

static const struct gpio_dt_spec user_led_0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec user_led_1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec user_led_2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec user_led_3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

/* Buttons definitions */
#define SW0_NODE DT_ALIAS(sw0)
#define SW1_NODE DT_ALIAS(sw1)
#define SW2_NODE DT_ALIAS(sw2)
#define SW3_NODE DT_ALIAS(sw3)

static const struct gpio_dt_spec user_button_0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct gpio_dt_spec user_button_1 = GPIO_DT_SPEC_GET(SW1_NODE, gpios);
static const struct gpio_dt_spec user_button_2 = GPIO_DT_SPEC_GET(SW2_NODE, gpios);
static const struct gpio_dt_spec user_button_3 = GPIO_DT_SPEC_GET(SW3_NODE, gpios);

/************************ Tasks requirements **********************************/
#define STACK_SIZE 512
K_THREAD_STACK_DEFINE(thread_stack_area_0, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_stack_area_1, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_stack_area_ME, STACK_SIZE);

struct k_thread thread_data_0, thread_data_1, thread_data_ME;

// Remplacer les trois sémaphores par un seul avec 2 jetons
K_SEM_DEFINE(tasks_sem, 0, 2);  // initial 0, max 2 jetons

/************************* TASK 0 *********************************/
void task_0()
{
    char carac ='a';
    printf("Task 0: %c\n", carac);
    carac = (carac == 'z') ? 'a' : carac + 1;
    k_busy_wait(15000);
    
    // Rendre un jeton et s'arrêter
    k_sem_give(&tasks_sem);
    k_thread_abort(k_current_get());
}

/************************* TASK 1*********************************/
void task_1()
{
    uint8_t nb = 0;
    printf("Task 1: %d\n", nb);
    nb = (nb == 90) ? 0 : nb + 1;
    k_busy_wait(15000);
    
    // Rendre un jeton et s'arrêter
    k_sem_give(&tasks_sem);
    k_thread_abort(k_current_get());
}

/*************************TASK ME*********************************/
void ME()
{
    uint8_t i = 0;
    
    while (1) {
        // Créer et démarrer les tâches
        k_thread_create(&thread_data_0, thread_stack_area_0, 
            K_THREAD_STACK_SIZEOF(thread_stack_area_0), 
            task_0, NULL, NULL, NULL, 
            5, 0, K_NO_WAIT);

        k_thread_create(&thread_data_1, thread_stack_area_1,
            K_THREAD_STACK_SIZEOF(thread_stack_area_1),
            task_1, NULL, NULL, NULL,
            5, 0, K_NO_WAIT);

        // Attendre que les deux tâches aient terminé (2 jetons)
        k_sem_take(&tasks_sem, K_FOREVER);
        k_sem_take(&tasks_sem, K_FOREVER);
        
        // state ME.2 :
        i++;
        gpio_pin_toggle_dt(&user_led_0);
        gpio_pin_toggle_dt(&user_led_3);
        k_msleep(250);
    }
}


int main(void)
{
    // GPIO configuration for all LEDs and buttons
    if (!device_is_ready(user_led_0.port) || 
        !device_is_ready(user_led_1.port) ||
        !device_is_ready(user_led_2.port) ||
        !device_is_ready(user_led_3.port) ||
        !device_is_ready(user_button_0.port) ||
        !device_is_ready(user_button_1.port) ||
        !device_is_ready(user_button_2.port) ||
        !device_is_ready(user_button_3.port)) 
    {
        printk("GPIO ports not ready\n");
        return 0;
    }

    // Configure all LEDs and buttons
    if ((gpio_pin_configure_dt(&user_led_0, GPIO_OUTPUT_ACTIVE) < 0) ||
        (gpio_pin_configure_dt(&user_led_1, GPIO_OUTPUT_ACTIVE) < 0) ||
        (gpio_pin_configure_dt(&user_led_2, GPIO_OUTPUT_ACTIVE) < 0) ||
        (gpio_pin_configure_dt(&user_led_3, GPIO_OUTPUT_ACTIVE) < 0) ||
        (gpio_pin_configure_dt(&user_button_0, GPIO_INPUT) < 0) ||
        (gpio_pin_configure_dt(&user_button_1, GPIO_INPUT) < 0) ||
        (gpio_pin_configure_dt(&user_button_2, GPIO_INPUT) < 0) ||
        (gpio_pin_configure_dt(&user_button_3, GPIO_INPUT) < 0)) 
    {
        printk("Error configuring GPIO pins\n");
        return 0;
    }

    // Task creation
    k_thread_create(&thread_data_0, thread_stack_area_0, 
        K_THREAD_STACK_SIZEOF(thread_stack_area_0), 
        task_0, 
        NULL, NULL, NULL, 
        5, 0, K_FOREVER);

    k_thread_create(&thread_data_1, thread_stack_area_1,
        K_THREAD_STACK_SIZEOF(thread_stack_area_1),
        task_1,
        NULL, NULL, NULL,
        5, 0, K_FOREVER);
	

    k_thread_create(&thread_data_ME, thread_stack_area_ME,
        K_THREAD_STACK_SIZEOF(thread_stack_area_ME),
        ME,
        NULL, NULL, NULL,
        1, 0, K_FOREVER);

    // end of the main : free the tasks
    k_thread_start(&thread_data_ME);

    return 0;
}
