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
#include <math.h>

///////////////////// VARIABLES ////////////////////

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
lv_obj_t * ui_nbPas;
lv_obj_t * ui_Image1;
lv_obj_t * ui_Image2;
lv_obj_t * ui_Image3;
lv_obj_t * ui_Image4;
lv_obj_t * ui_Image5;
// CUSTOM VARIABLES

// SCREEN: ui_Screen4
void ui_Screen4_screen_init(void);
lv_obj_t * ui_Screen4;
lv_obj_t * ui_Clock_Group1;
lv_obj_t * ui_Accelerometre;
lv_obj_t * ui_accel_x;
lv_obj_t * ui_accel_y;
lv_obj_t * ui_accel_z;
lv_obj_t * ui_Gyroscope;
lv_obj_t * ui_gyro_x;
lv_obj_t * ui_gyro_y;
lv_obj_t * ui_gyro_z;
// CUSTOM VARIABLES

// SCREEN: ui_Screen5
void ui_Screen5_screen_init(void);
lv_obj_t * ui_Screen5;
lv_obj_t * ui_Clock_Group3;
lv_obj_t * ui_Magnetisme;
lv_obj_t * ui_mag_x;
lv_obj_t * ui_mag_y;
lv_obj_t * ui_mag_z;
lv_obj_t * ui_compass;
// CUSTOM VARIABLES

// SCREEN: ui_Screen6
void ui_Screen6_screen_init(void);
lv_obj_t * ui_Screen6;
lv_obj_t * ui_Clock_Group4;
lv_obj_t * ui_Chronometre;
lv_obj_t * ui_chronometreActi;
lv_obj_t * ui_distance;
lv_obj_t * ui_test;
// CUSTOM VARIABLES

// SCREEN: ui_Screen7
void ui_Screen7_screen_init(void);
lv_obj_t * ui_Screen7;
lv_obj_t * ui_Clock_Group5;
lv_obj_t * ui_month;
lv_obj_t * ui_lundi;
lv_obj_t * ui_mardi;
lv_obj_t * ui_mercredi;
lv_obj_t * ui_jeudi;
lv_obj_t * ui_vendredi;
lv_obj_t * ui_samedi;
lv_obj_t * ui_dimanche;
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

// Ajouter ces variables globales au début du fichier
static float current_temperature = 0.0f;
static float current_humidity = 0.0f;

// Ajouter au début du fichier avec les autres variables globales
typedef enum {
    COMPASS_N,
    COMPASS_NE,
    COMPASS_E,
    COMPASS_SE,
    COMPASS_S,
    COMPASS_SW,
    COMPASS_W,
    COMPASS_NW
} compass_direction_t;

static float mag_x_value = 0.0f;
static float mag_y_value = 0.0f;
static compass_direction_t compass_pos = COMPASS_N;

// Ajouter cette fonction après les déclarations des variables
static void update_weather_image(void) {
    // Cache toutes les images par défaut
    if (ui_Image1) lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_HIDDEN);  // sun_cloud
    if (ui_Image2) lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_HIDDEN);  // sun
    if (ui_Image3) lv_obj_add_flag(ui_Image3, LV_OBJ_FLAG_HIDDEN);  // cloud_fog
    if (ui_Image4) lv_obj_add_flag(ui_Image4, LV_OBJ_FLAG_HIDDEN);  // cloud

    // Sélectionne l'image appropriée en fonction des conditions
    lv_obj_t *selected_image = NULL;

    if (current_humidity >= 80 && current_temperature < 20) {
        // Humide et frais -> brumeux
        selected_image = ui_Image3;  // cloud_fog
    } else if (current_temperature >= 25 && current_humidity < 60) {
        // Chaud et sec -> très ensoleillé
        selected_image = ui_Image2;  // sun
    } else if (current_temperature >= 20 && current_humidity < 70) {
        // Température modérée -> partiellement nuageux
        selected_image = ui_Image1;  // sun_cloud
    } else {
        // Autres conditions -> nuageux
        selected_image = ui_Image4;  // cloud
    }

    // Affiche l'image sélectionnée
    if (selected_image) {
        lv_obj_clear_flag(selected_image, LV_OBJ_FLAG_HIDDEN);
    }
}

// Ajouter cette fonction après les autres fonctions
static const char* get_compass_direction_str(compass_direction_t dir) {
    switch (dir) {
        case COMPASS_N:  return "N";
        case COMPASS_NE: return "NE";
        case COMPASS_E:  return "E";
        case COMPASS_SE: return "SE";
        case COMPASS_S:  return "S";
        case COMPASS_SW: return "SW";
        case COMPASS_W:  return "W";
        case COMPASS_NW: return "NW";
        default:         return "?";
    }
}

static void update_compass_direction(void) {
    // Calculer l'angle entre -180 et 180 degrés
    float angle = atan2f(mag_y_value, mag_x_value) * 180.0f / 3.14159f;
    
    // Normaliser l'angle entre 0 et 360 degrés
    if (angle < 0) {
        angle += 360.0f;
    }

    // Déterminer la direction en fonction de l'angle
    if ((angle >= 340.5f) || (angle < 10.5f)) {
        compass_pos = COMPASS_W;
    } else if ((angle >= 10.5f) && (angle < 67.5f)) {
        compass_pos = COMPASS_SW;
    } else if ((angle >= 67.5f) && (angle < 170.5f)) {
        compass_pos = COMPASS_S;
    } else if ((angle >= 170.5f) && (angle < 190.5f)) {
        compass_pos = COMPASS_SE; //
    } else if ((angle >= 190.5f) && (angle < 220.5f)) {
        compass_pos = COMPASS_E;
    } else if ((angle >= 220.5f) && (angle < 240.5f)) {
        compass_pos = COMPASS_NE; //NE
    } else if ((angle >= 250.5f) && (angle < 292.5f)) {
        compass_pos = COMPASS_N;    //NORD
    } else {
        compass_pos = COMPASS_NW;
    }

    lv_obj_t *current_screen = lv_scr_act();
    if (current_screen == ui_Screen5 && ui_compass != NULL) {
        lv_label_set_text(ui_compass, get_compass_direction_str(compass_pos));
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
            current_temperature = msg.value;
            break;
            
        case MSG_TYPE_HUMIDITY:
            target_label = ui_LabelHumi;
            format = " Hum\n%.1f%%";
            current_humidity = msg.value;
            break;

        case MSG_TYPE_ACCEL_X:
            target_label = ui_accel_x;
            format = "X: %.2f";
            break;

        case MSG_TYPE_ACCEL_Y:
            target_label = ui_accel_y;
            format = "Y: %.2f";
            break;

        case MSG_TYPE_ACCEL_Z:
            target_label = ui_accel_z;
            format = "Z: %.2f";
            break;

        case MSG_TYPE_GYRO_X:
            target_label = ui_gyro_x;
            format = "X: %.2f";
            break;

        case MSG_TYPE_GYRO_Y:
            target_label = ui_gyro_y;
            format = "Y: %.2f";
            break;

        case MSG_TYPE_GYRO_Z:
            target_label = ui_gyro_z;
            format = "Z: %.2f";
            break;

        case MSG_TYPE_MAG_X:
            target_label = ui_mag_x;
            format = "X: %.2f";
            mag_x_value = msg.value;
            update_compass_direction();  // Mettre à jour la direction après chaque nouvelle valeur X
            break;

        case MSG_TYPE_MAG_Y:
            target_label = ui_mag_y;
            format = "Y: %.2f";
            mag_y_value = msg.value;
            update_compass_direction();  // Mettre à jour la direction après chaque nouvelle valeur Y
            break;

        case MSG_TYPE_MAG_Z:
            target_label = ui_mag_z;
            format = "Z: %.2f";
            break;

        case MSG_TYPE_STEP_COUNT:
            if (ui_nbPas != NULL) {
                lv_snprintf(buffer, sizeof(buffer), "%d", (int)msg.value);
                lv_label_set_text(ui_nbPas, buffer);
            }
            // Ne pas utiliser target_label car on gère directement les deux labels
            target_label = NULL;
            format = NULL;
            break;
        }
        
        if (target_label != NULL && format != NULL) {
            lv_snprintf(buffer, sizeof(buffer), format, msg.value);
            lv_label_set_text(target_label, buffer);
        }

        // Mettre à jour l'image météo si on est sur l'écran 3
        lv_obj_t *current_screen = lv_scr_act();
        if (current_screen == ui_Screen3) {
            update_weather_image();
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
    ui_Screen2_screen_init();
    ui_Screen3_screen_init();
    ui_Screen4_screen_init();
    ui_Screen5_screen_init();
    ui_Screen6_screen_init();
    ui_Screen7_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_obj_add_event_cb(ui____initial_actions0, ui_event____initial_actions0, LV_EVENT_ALL, NULL);

    lv_disp_load_scr(ui____initial_actions0);
    lv_disp_load_scr(ui_Screen2);
    
    printk("UI initialization completed\n");
}

void ui_thread_entry(void *p1, void *p2, void *p3)
{
    printk("UI thread started\n");
    
    /* Initialize default labels if necessary */
    if (ui_LabelTemp != NULL) {
        lv_label_set_text(ui_LabelTemp, " Temp\n--.- °C");  // Format identique à MSG_TYPE_TEMPERATURE
    }
    
    if (ui_LabelHumi != NULL) {
        lv_label_set_text(ui_LabelHumi, " Hum\n--.-%");    // Format identique à MSG_TYPE_HUMIDITY
    }
    
    if (ui_HourLabel != NULL) {
        lv_label_set_text(ui_HourLabel, "00:00:00");
    }
    
    if (ui_distance != NULL) {
        lv_label_set_text(ui_distance, "0 steps");
    }
    if (ui_nbPas != NULL) {
        lv_label_set_text(ui_nbPas, "0");
    }
    uint32_t last_sensor_update = 0;
    uint32_t last_time_update = 0;

    while (1) {
        lv_obj_t *current_screen = lv_scr_act();
        uint32_t current_time = k_uptime_get_32();
        
        // Mise à jour des capteurs toutes les 100ms
        if (current_time - last_sensor_update >= 100) {
            if (current_screen == ui_Screen3 ||
                current_screen == ui_Screen4 ||
                current_screen == ui_Screen5) {
                process_queue();
            }
            last_sensor_update = current_time;
        }
        
        // Mise à jour de l'heure chaque seconde (uniquement pour Screen3)
        if (current_screen == ui_Screen3) {
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