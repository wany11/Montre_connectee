// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: TempSensor

#include "ui.h"
#include "ui_helpers.h"
#include "../../inc/queue.h"
#include "../../inc/sensors.h"
#include <stdio.h>
#include <zephyr/kernel.h>
#include "../../inc/timSec.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Panel3;
lv_obj_t * ui_accel_x;
lv_obj_t * ui_accel_y;
lv_obj_t * ui_accel_z;
lv_obj_t * ui_gyro_x;
lv_obj_t * ui_gyro_y;
lv_obj_t * ui_gyro_z;
lv_obj_t * ui_mag_x;
lv_obj_t * ui_mag_y;
lv_obj_t * ui_mag_z;
// CUSTOM VARIABLES

// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
lv_obj_t * ui_Screen2;
lv_obj_t * ui_Clock_Group;
lv_obj_t * ui_hour;
lv_obj_t * ui_min;
lv_obj_t * ui_sec;
lv_obj_t * ui_ThreeH;
lv_obj_t * ui_SixH;
lv_obj_t * ui_NineH;
lv_obj_t * ui_OneH;
lv_obj_t * ui_TwoH;
lv_obj_t * ui_FourH;
lv_obj_t * ui_FiveH;
lv_obj_t * ui_SevenH;
lv_obj_t * ui_EightH;
lv_obj_t * ui_TenH;
lv_obj_t * ui_ElevH;
lv_obj_t * ui_TwelveH;
// CUSTOM VARIABLES
lv_obj_t * uic_Clock_Group;

// SCREEN: ui_Screen3
void ui_Screen3_screen_init(void);
lv_obj_t * ui_Screen3;
lv_obj_t * ui_Clock_Group2;
lv_obj_t * ui_HourLabel;
lv_obj_t * ui_LabelHumi;
lv_obj_t * ui_LabelTemp;
// CUSTOM VARIABLES

// EVENTS
void ui_event____initial_actions0(lv_event_t * e);
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 32
    #error "LV_COLOR_DEPTH should be 32bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////
void sec_Animation(lv_obj_t * TargetObject, int angle)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 60000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_angle);
    lv_anim_set_values(&PropertyAnimation_0, angle, angle+3600);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, true);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_image_angle);
    lv_anim_start(&PropertyAnimation_0);

}
void min_Animation(lv_obj_t * TargetObject, int angle)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 3600000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_angle);
    lv_anim_set_values(&PropertyAnimation_0, angle, angle+3600);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, true);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_image_angle);
    lv_anim_start(&PropertyAnimation_0);

}
void hour_Animation(lv_obj_t * TargetObject, int angle)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 216000000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_angle);
    lv_anim_set_values(&PropertyAnimation_0, angle, angle+3600);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, true);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_image_angle);
    lv_anim_start(&PropertyAnimation_0);

}

// Add this function after the animation functions
void init_clock_positions(void)
{
    uint32_t temps = get_temps();
    uint32_t hours = (temps / 3600) % 12;  // 12-hour format
    uint32_t minutes = (temps / 60) % 60;
    uint32_t seconds = temps % 60;

    // Calculate angles in tenths of degrees
    uint32_t hour_angle = (hours * 3600) / 12 + (minutes * 3600) / (12 * 60);
    uint32_t min_angle = (minutes * 3600) / 60;
    uint32_t sec_angle = (seconds * 3600) / 60;

    // Start animations with correct initial angles
    if (ui_sec != NULL) {
        sec_Animation(ui_sec, sec_angle);
    }
    if (ui_min != NULL) {
        min_Animation(ui_min, min_angle);
    }
    if (ui_hour != NULL) {
        hour_Animation(ui_hour, hour_angle);
    }
}

///////////////////// FUNCTIONS ////////////////////
void ui_event____initial_actions0(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        init_clock_positions();
    }
}

///////////////////// SCREENS ////////////////////

/* Reference the semaphore declared in main.c */
extern struct k_sem ui_ready_sem;

/**
 * @brief Process all messages in temperature queue and update UI
 * 
 * This function retrieves all available messages from the temperature queue
 * and updates the corresponding UI elements.
 */
void process_queue(void)
{
    sensor_msg_t msg;
    static char buffer[32];
    struct k_msgq *temp_msgq = get_msgq();
    
    if (temp_msgq == NULL) {
        return;
    }
    
    /* Process all messages in the queue */
    while (k_msgq_get(temp_msgq, &msg, K_NO_WAIT) == 0) {
        lv_obj_t *target_label = NULL;
        const char *format = NULL;

        switch(msg.type) {
        case MSG_TYPE_TEMPERATURE:
            target_label = ui_LabelTemp;
            format = " Temp\n%.1f °C";
            break;
            
        case MSG_TYPE_HUMIDITY:
            target_label = ui_LabelHumi;
            format = " Hum\n%.1f%%";
            break;

        case MSG_TYPE_ACCEL_X:
            target_label = ui_accel_x;
            format = "Accel X: %.2f";
            break;

        case MSG_TYPE_ACCEL_Y:
            target_label = ui_accel_y;
            format = "Accel Y: %.2f";
            break;

        case MSG_TYPE_ACCEL_Z:
            target_label = ui_accel_z;
            format = "Accel Z: %.2f";
            break;

        case MSG_TYPE_GYRO_X:
            target_label = ui_gyro_x;
            format = "Gyro X: %.2f";
            break;

        case MSG_TYPE_GYRO_Y:
            target_label = ui_gyro_y;
            format = "Gyro Y: %.2f";
            break;

        case MSG_TYPE_GYRO_Z:
            target_label = ui_gyro_z;
            format = "Gyro Z: %.2f";
            break;

        case MSG_TYPE_MAG_X:
            target_label = ui_mag_x;
            format = "Mag X: %.2f";
            break;

        case MSG_TYPE_MAG_Y:
            target_label = ui_mag_y;
            format = "Mag Y: %.2f";
            break;

        case MSG_TYPE_MAG_Z:
            target_label = ui_mag_z;
            format = "Mag Z: %.2f";
            break;
        }
        
        if (target_label != NULL && format != NULL) {
            lv_snprintf(buffer, sizeof(buffer), format, msg.value);
            lv_label_set_text(target_label, buffer);
        }
    }
}

void ui_init(void)
{
    printk("UI initialization started\n");

    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    ui_Screen3_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_obj_add_event_cb(ui____initial_actions0, ui_event____initial_actions0, LV_EVENT_ALL, NULL);

    lv_disp_load_scr(ui____initial_actions0);
    lv_disp_load_scr(ui_Screen1);
    
    printk("UI initialization completed\n");
}

void ui_thread_entry(void *p1, void *p2, void *p3)
{
    printk("UI thread started\n");
    
    /* Initialize default labels if necessary */
    if (ui_LabelTemp != NULL) {
        lv_label_set_text(ui_LabelTemp, "Temperature: --.- °C");
    }
    
    if (ui_LabelHumi != NULL) {
        lv_label_set_text(ui_LabelHumi, "Humidity: --.-%");
    }
    
    if (ui_HourLabel != NULL) {
        lv_label_set_text(ui_HourLabel, "00:00:00");
    }

    while (1) {
        /* Check which screen is active */
        lv_obj_t *current_screen = lv_scr_act();
        
        if (current_screen == ui_Screen1) {
            static uint32_t last_update = 0;
            uint32_t current_time = k_uptime_get_32();
            
            // Mise à jour des capteurs toutes les 500ms
            if (current_time - last_update >= 500) {
                process_queue();
                last_update = current_time;
            }
            
            // Mise à jour de l'heure chaque seconde
            static uint32_t last_time_update = 0;
            if (current_time - last_time_update >= 1000) {
                if (ui_HourLabel != NULL) {
                    char time_str[32];
                    format_time(get_temps(), time_str, sizeof(time_str));
                    lv_label_set_text(ui_HourLabel, time_str);
                }
                last_time_update = current_time;
            }
        }
        else if (current_screen == ui_Screen3) {
            static uint32_t last_update = 0;
            uint32_t current_time = k_uptime_get_32();
            
            // Mise à jour des capteurs toutes les 500ms
            if (current_time - last_update >= 500) {
                process_queue();
                last_update = current_time;
            }
            
            // Mise à jour de l'heure chaque seconde
            static uint32_t last_time_update = 0;
            if (current_time - last_time_update >= 1000) {
                if (ui_HourLabel != NULL) {
                    char time_str[32];
                    format_time(get_temps(), time_str, sizeof(time_str));
                    lv_label_set_text(ui_HourLabel, time_str);
                }
                last_time_update = current_time;
            }
        }

        k_sleep(K_MSEC(100)); // Réduit les cycles CPU inutiles
    }
}