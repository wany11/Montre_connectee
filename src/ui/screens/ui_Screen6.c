// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: Proj_test

#include "../../ui/ui.h"
#include <zephyr/kernel.h>
#include "../../../inc/lsm6dso.h"  // Inclure l'en-tête du capteur LSM6DSO
#include "../../../inc/ui_Screen6.h" // Inclure le fichier d'en-tête ui_Screen6.h

// Déclaration externe pour la fonction de récupération du nombre de pas
extern int lsm6dso_get_step_count(uint32_t *steps);

// Déclaration externe pour les fonctions de détection d'activité
extern bool lsm6dso_detect_activity(void);
extern bool lsm6dso_is_activity_detected(void);

// Variables pour le chronomètre
static chrono_state_t chrono_state = CHRONO_STOPPED;
static int64_t chrono_start_time = 0;
static int64_t chrono_elapsed_time = 0;
static lv_timer_t *chrono_timer = NULL;
static uint32_t step_count = 0;

// Boutons pour le chronomètre
static lv_obj_t *start_pause_btn;
static lv_obj_t *reset_btn;
static lv_obj_t *start_pause_label;

// Variables pour le chronomètre d'activité
static int64_t activity_start_time = 0;
static int64_t activity_elapsed_time = 0;
static bool activity_was_detected = false;

// Déclaration préalable des fonctions pour éviter les erreurs
static void update_chrono_display(void);
static void update_step_display(void);
static void update_activity_chrono_display(void);

// Fonction pour mettre à jour l'affichage du chronomètre
static void update_chrono_display(void)
{
    int64_t elapsed;
    char buf[32];
    
    if (chrono_state == CHRONO_RUNNING) {
        elapsed = chrono_elapsed_time + (k_uptime_get() - chrono_start_time);
    } else {
        elapsed = chrono_elapsed_time;
    }
    
    // Convertir en minutes:secondes.centièmes
    unsigned int min = elapsed / 60000;
    unsigned int sec = (elapsed % 60000) / 1000;
    unsigned int ms = (elapsed % 1000) / 10;
    
    snprintf(buf, sizeof(buf), "%02u:%02u.%02u", min, sec, ms);
    lv_label_set_text(ui_Chronometre, buf);
    
    // Mise à jour de l'affichage du bouton selon l'état
    if (chrono_state == CHRONO_STOPPED) {
        lv_label_set_text(start_pause_label, "Start");
        // Désactiver le bouton Reset lorsque le chronomètre est arrêté
        lv_obj_add_state(reset_btn, LV_STATE_DISABLED);
    } else if (chrono_state == CHRONO_RUNNING) {
        lv_label_set_text(start_pause_label, "Pause");
        // Activer le bouton Reset
        lv_obj_clear_state(reset_btn, LV_STATE_DISABLED);
    } else if (chrono_state == CHRONO_PAUSED) {
        lv_label_set_text(start_pause_label, "Start");
        // Bouton Reset déjà actif
    }
}

// Ajoutez cette fonction après update_chrono_display()
static void update_step_display(void)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%u steps", step_count);
    lv_label_set_text(ui_distance, buf);
}

// Fonction publique pour mettre à jour le compteur de pas depuis d'autres modules
void ui_set_step_count(uint32_t steps)
{
    step_count = steps;
    update_step_display();
}

// Fonction publique pour réinitialiser le chronomètre d'activité
void ui_reset_activity_chrono(void)
{
    activity_elapsed_time = 0;
    activity_was_detected = false;
    update_activity_chrono_display();
}

// Fonction pour mettre à jour l'affichage du chronomètre d'activité
static void update_activity_chrono_display(void)
{
    char buf[32];
    
    // Convertir en heures:minutes:secondes
    unsigned int hours = activity_elapsed_time / 3600000;
    unsigned int min = (activity_elapsed_time % 3600000) / 60000;
    unsigned int sec = (activity_elapsed_time % 60000) / 1000;
    
    snprintf(buf, sizeof(buf), "%02u:%02u:%02u", hours, min, sec);
    lv_label_set_text(ui_chronometreActi, buf);
}

// Fonction de mise à jour périodique du chronomètre
static void chrono_update_timer_cb(lv_timer_t *timer)
{
    if (chrono_state == CHRONO_RUNNING) {
        update_chrono_display();
    }
}

// Fonction de mise à jour périodique du compteur de pas
static void step_update_timer_cb(lv_timer_t *timer)
{
    uint32_t current_steps = 0;
    if (lsm6dso_get_step_count(&current_steps) == 0) {
        if (current_steps != step_count) {
            step_count = current_steps;
            update_step_display();
        }
    }
}

// Fonction de mise à jour du chronomètre d'activité
static void activity_timer_cb(lv_timer_t *timer)
{
    // Utiliser la fonction lsm6dso_detect_activity() pour détecter l'activité en temps réel
    // Cette fonction fait l'analyse directe des données du capteur
    bool current_activity = lsm6dso_detect_activity();
    
    // Obtenir le temps actuel
    int64_t now = k_uptime_get();
    
    if (current_activity) {
        // Si c'est le début d'une période d'activité
        if (!activity_was_detected) {
            activity_was_detected = true;
            activity_start_time = now;
        }
        // Pas besoin de mettre à jour le temps écoulé pendant l'activité
        // Nous le ferons seulement à la fin d'une période d'activité
    } else {
        // Si c'est la fin d'une période d'activité
        if (activity_was_detected) {
            // Ajouter le temps de cette période d'activité au total
            activity_elapsed_time += (now - activity_start_time);
            activity_was_detected = false;
        }
    }
    
    // Si on est toujours en activité, calculer le temps total incluant la période actuelle
    int64_t total_elapsed = activity_elapsed_time;
    if (activity_was_detected) {
        total_elapsed += (now - activity_start_time);
    }
    
    // Convertir en heures:minutes:secondes
    unsigned int hours = total_elapsed / 3600000;
    unsigned int min = (total_elapsed % 3600000) / 60000;
    unsigned int sec = (total_elapsed % 60000) / 1000;
    
    char buf[32];
    snprintf(buf, sizeof(buf), "%02u:%02u:%02u", hours, min, sec);
    lv_label_set_text(ui_chronometreActi, buf);
}

// Gestionnaire d'événement pour le bouton Start/Pause
static void start_pause_event_cb(lv_event_t *e)
{
    switch (chrono_state) {
        case CHRONO_STOPPED:
            // Démarrer le chronomètre
            chrono_state = CHRONO_RUNNING;
            chrono_start_time = k_uptime_get();
            lv_label_set_text(start_pause_label, "Pause");
            // Activer le bouton Reset
            lv_obj_clear_state(reset_btn, LV_STATE_DISABLED);
            break;
            
        case CHRONO_RUNNING:
            // Mettre en pause
            chrono_state = CHRONO_PAUSED;
            chrono_elapsed_time += (k_uptime_get() - chrono_start_time);
            lv_label_set_text(start_pause_label, "Start");
            break;
            
        case CHRONO_PAUSED:
            // Reprendre le chronomètre
            chrono_state = CHRONO_RUNNING;
            chrono_start_time = k_uptime_get();
            lv_label_set_text(start_pause_label, "Pause");
            break;
    }
    
    update_chrono_display();
}

// Gestionnaire d'événement pour le bouton Reset
static void reset_event_cb(lv_event_t *e)
{
    if (chrono_state != CHRONO_STOPPED) {
        // Réinitialiser le chronomètre
        chrono_state = CHRONO_STOPPED;
        chrono_elapsed_time = 0;
        lv_label_set_text(start_pause_label, "Start");
        // Désactiver le bouton Reset car le chronomètre est arrêté
        lv_obj_add_state(reset_btn, LV_STATE_DISABLED);
        update_chrono_display();
    }
}

// Fonction à appeler lorsqu'on appuie sur la zone tactile - conservée pour compatibilité
void chrono_button_handler(void)
{
    // Cette fonction est maintenant remplacée par les gestionnaires d'événements des boutons
}

void ui_Screen6_screen_init(void)
{
    ui_Screen6 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Clock_Group4 = lv_obj_create(ui_Screen6);
    lv_obj_set_width(ui_Clock_Group4, 244);
    lv_obj_set_height(ui_Clock_Group4, 244);
    lv_obj_set_align(ui_Clock_Group4, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Clock_Group4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Clock_Group4, 10000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Clock_Group4, lv_color_hex(0x262F29), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Clock_Group4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chronometre = lv_label_create(ui_Screen6);
    lv_obj_set_width(ui_Chronometre, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_Chronometre, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_Chronometre, 0);
    lv_obj_set_y(ui_Chronometre, -50);
    lv_obj_set_align(ui_Chronometre, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_Chronometre, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Chronometre, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Chronometre, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_chronometreActi = lv_label_create(ui_Screen6);
    lv_obj_set_width(ui_chronometreActi, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_chronometreActi, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_chronometreActi, 0);
    lv_obj_set_y(ui_chronometreActi, 50);    
    lv_obj_set_align(ui_chronometreActi, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_chronometreActi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_chronometreActi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_chronometreActi, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_distance = lv_label_create(ui_Screen6);
    lv_obj_set_width(ui_distance, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_distance, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_distance, 0);
    lv_obj_set_y(ui_distance, 80);
    lv_obj_set_align(ui_distance, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_distance, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_distance, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Supprimer l'ancien label ui_test et le remplacer par de vrais boutons
    
    // Bouton Start/Pause
    start_pause_btn = lv_btn_create(ui_Screen6);
    lv_obj_set_width(start_pause_btn, 80);
    lv_obj_set_height(start_pause_btn, 40);
    lv_obj_set_x(start_pause_btn, -50);
    lv_obj_set_y(start_pause_btn, 0);
    lv_obj_set_align(start_pause_btn, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(start_pause_btn, start_pause_event_cb, LV_EVENT_CLICKED, NULL);
    
    start_pause_label = lv_label_create(start_pause_btn);
    lv_obj_set_align(start_pause_label, LV_ALIGN_CENTER);
    lv_label_set_text(start_pause_label, "Start");
    
    // Bouton Reset
    reset_btn = lv_btn_create(ui_Screen6);
    lv_obj_set_width(reset_btn, 80);
    lv_obj_set_height(reset_btn, 40);
    lv_obj_set_x(reset_btn, 50);
    lv_obj_set_y(reset_btn, 0);
    lv_obj_set_align(reset_btn, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(reset_btn, reset_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_state(reset_btn, LV_STATE_DISABLED); // Désactivé au début
    
    lv_obj_t *reset_label = lv_label_create(reset_btn);
    lv_obj_set_align(reset_label, LV_ALIGN_CENTER);
    lv_label_set_text(reset_label, "Reset");
    
    // Initialiser l'affichage du chronomètre
    lv_label_set_text(ui_Chronometre, "00:00.00");
    
    // Initialiser l'affichage du nombre de pas
    uint32_t initial_steps = 0;
    if (lsm6dso_get_step_count(&initial_steps) == 0) {
        step_count = initial_steps;
    }
    update_step_display();

    // Initialiser l'affichage du chronomètre d'activité
    lv_label_set_text(ui_chronometreActi, "00:00:00");
    
    // Créer un timer pour mettre à jour l'affichage du chronomètre (10 fois par seconde)
    if (chrono_timer == NULL) {
        chrono_timer = lv_timer_create(chrono_update_timer_cb, 100, NULL);
    }

    // Créer un timer pour mettre à jour l'affichage du compteur de pas (chaque seconde)
    static lv_timer_t *step_timer = NULL;
    if (step_timer == NULL) {
        step_timer = lv_timer_create(step_update_timer_cb, 1000, NULL);
    }

    // Créer un timer pour mettre à jour le chronomètre d'activité
    static lv_timer_t *activity_timer = NULL;
    if (activity_timer == NULL) {
        activity_timer = lv_timer_create(activity_timer_cb, 250, NULL); // 4 fois par seconde
    }
}
