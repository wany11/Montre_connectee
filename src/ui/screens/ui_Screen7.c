// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: Proj_test

#include "../ui.h"
#include <time.h>
#include <stdio.h>
#include "../../../inc/rtc.h"  // Ajout de l'inclusion pour le RTC

// Ajouter ces déclarations externes en haut du fichier, après les includes
extern uint16_t g_current_year;
extern uint8_t g_current_month;
extern uint8_t g_current_day;
extern uint8_t g_current_hour;
extern uint8_t g_current_minute;
extern uint8_t g_current_second;
extern int rtc_update_global_time(void);

static lv_obj_t *calendar_days[42]; // Max 6 lignes de 7 jours
static lv_style_t style_current_day;
static int current_day = 0;
static const char *month_names[] = {"January", "February", "March", "April", "May", "June", 
                                   "July", "August", "September", "October", "November", "December"};

// Prototypes de fonctions
void update_calendar(void);
void calendar_update_task(lv_timer_t *timer);
static void screen7_event_handler(lv_event_t *e);

void ui_Screen7_screen_init(void)
{
    ui_Screen7 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    // Ajouter le gestionnaire d'événements à l'écran
    lv_obj_add_event_cb(ui_Screen7, screen7_event_handler, LV_EVENT_ALL, NULL);

    ui_Clock_Group5 = lv_obj_create(ui_Screen7);
    lv_obj_set_width(ui_Clock_Group5, 244);
    lv_obj_set_height(ui_Clock_Group5, 244);
    lv_obj_set_align(ui_Clock_Group5, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Clock_Group5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Clock_Group5, 10000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Clock_Group5, lv_color_hex(0x262F29), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Clock_Group5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_month = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_month, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_month, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_month, -80);
    lv_obj_set_y(ui_month, -75);
    lv_obj_set_align(ui_month, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_month, "Mois");
    lv_obj_set_style_text_color(ui_month, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_month, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_year = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_year, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_year, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_year, 80);
    lv_obj_set_y(ui_year, -75);
    lv_obj_set_align(ui_year, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_year, "2000");
    lv_obj_set_style_text_color(ui_year, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_year, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lundi = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_lundi, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_lundi, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_lundi, -95);
    lv_obj_set_y(ui_lundi, -53);
    lv_obj_set_align(ui_lundi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lundi, "M");
    lv_obj_set_style_text_color(ui_lundi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lundi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_mardi = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_mardi, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_mardi, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_mardi, -64);
    lv_obj_set_y(ui_mardi, -53);
    lv_obj_set_align(ui_mardi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_mardi, "T");
    lv_obj_set_style_text_color(ui_mardi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_mardi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_mercredi = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_mercredi, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_mercredi, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_mercredi, -32);
    lv_obj_set_y(ui_mercredi, -53);
    lv_obj_set_align(ui_mercredi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_mercredi, "W");
    lv_obj_set_style_text_color(ui_mercredi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_mercredi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_jeudi = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_jeudi, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_jeudi, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_jeudi, 0);
    lv_obj_set_y(ui_jeudi, -53);
    lv_obj_set_align(ui_jeudi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_jeudi, "T");
    lv_obj_set_style_text_color(ui_jeudi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_jeudi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_vendredi = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_vendredi, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_vendredi, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_vendredi, 31);
    lv_obj_set_y(ui_vendredi, -53);
    lv_obj_set_align(ui_vendredi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_vendredi, "F");
    lv_obj_set_style_text_color(ui_vendredi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_vendredi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_samedi = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_samedi, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_samedi, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_samedi, 63);
    lv_obj_set_y(ui_samedi, -53);
    lv_obj_set_align(ui_samedi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_samedi, "S");
    lv_obj_set_style_text_color(ui_samedi, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_samedi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_dimanche = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_dimanche, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_dimanche, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_dimanche, 94);
    lv_obj_set_y(ui_dimanche, -53);
    lv_obj_set_align(ui_dimanche, LV_ALIGN_CENTER);
    lv_label_set_text(ui_dimanche, "S");
    lv_obj_set_style_text_color(ui_dimanche, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_dimanche, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Ajouter une ligne horizontale sous les jours de la semaine
    lv_obj_t *separator_line = lv_line_create(ui_Screen7);
    static lv_point_t line_points[] = {{-200, 0}, {200, 0}};
    lv_line_set_points(separator_line, line_points, 2);
    lv_obj_set_y(separator_line, -43);
    lv_obj_set_align(separator_line, LV_ALIGN_CENTER);
    lv_obj_set_style_line_width(separator_line, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(separator_line, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(separator_line, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Initialisation de tous les pointeurs à NULL
    for (int i = 0; i < 42; i++) {
        calendar_days[i] = NULL;
    }
    
    // Style pour le jour courant (carré rouge)
    lv_style_init(&style_current_day);
    lv_style_set_bg_color(&style_current_day, lv_color_hex(0xFF0000));
    lv_style_set_bg_opa(&style_current_day, LV_OPA_70);
    lv_style_set_radius(&style_current_day, 4);  // Coins légèrement arrondis
    lv_style_set_pad_left(&style_current_day, 8);    // Padding horizontal plus large
    lv_style_set_pad_right(&style_current_day, 8);   // Padding horizontal plus large
    lv_style_set_pad_top(&style_current_day, 3);     // Padding vertical plus étroit
    lv_style_set_pad_bottom(&style_current_day, 3);  // Padding vertical plus étroit
    
    // Initialiser le calendrier mais ne pas le mettre à jour tout de suite
    // (la mise à jour se fera au moment de l'événement SCREEN_LOADED)
}

void update_calendar(void) {
    // Mettre à jour les variables globales
    rtc_update_global_time();
    
    // Utiliser les variables globales au lieu d'appeler rtc_get_datetime
    uint16_t year = g_current_year;
    uint8_t month = g_current_month;
    uint8_t day = g_current_day;
    
    // Attention: month est de 1 à 12 dans le RTC mais de 0 à 11 dans le tableau
    int month_idx = month - 1;
    if (month_idx >= 0 && month_idx < 12) {
        // Mise à jour du mois affiché
        lv_label_set_text(ui_month, month_names[month_idx]);
    }

    // Mise à jour de l'année affichée
    char year_str[5];
    sprintf(year_str, "%d", year);
    lv_label_set_text(ui_year, year_str);
    
    // Calculer le premier jour du mois (formule de Zeller)
    int first_weekday = 0;
    
    // Version modifiée de la formule de Zeller pour le 1er du mois
    int m = month;
    int y = year;
    if (m < 3) {
        m += 12;
        y -= 1;
    }
    int h = (1 + (13*(m+1))/5 + y + y/4 - y/100 + y/400) % 7;
    // h donne 0=samedi, 1=dimanche, ..., 6=vendredi
    // Convertir en 1=lundi, ..., 7=dimanche
    first_weekday = ((h + 5) % 7) + 1;
    
    // Nombre de jours dans le mois
    int days_in_month;
    if (month == 2) { // février
        days_in_month = ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        days_in_month = 30;
    } else {
        days_in_month = 31;
    }
    
    // Position de départ pour les dates (sous les jours de la semaine)
    int start_x = 58; // Même position que "L"
    int start_y = 80; // 30 pixels sous les jours de la semaine
    int cell_width = 32;
    int cell_height = 22;
    
    // Créer ou mettre à jour les dates
    int cell = 0;
    
    // Jours du mois précédent (si nécessaire)
    for (int i = 1; i < first_weekday; i++) {
        if (calendar_days[cell] == NULL) {
            calendar_days[cell] = lv_label_create(ui_Screen7);
            lv_obj_set_style_text_color(calendar_days[cell], lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        
        lv_label_set_text(calendar_days[cell], ""); 
        lv_obj_set_pos(calendar_days[cell], start_x + ((cell % 7) * cell_width), start_y + ((cell / 7) * cell_height));
        lv_obj_remove_style(calendar_days[cell], &style_current_day, LV_PART_MAIN);
        cell++;
    }
    
    // Dans la fonction update_calendar(), pour l'affichage des jours du mois
    for (int i = 1; i <= days_in_month; i++) {
        if (calendar_days[cell] == NULL) {
            calendar_days[cell] = lv_label_create(ui_Screen7);
        }
        
        // Afficher le numéro du jour
        char day_str[3];
        sprintf(day_str, "%d", i);
        lv_label_set_text(calendar_days[cell], day_str);
        
        // Offset de base selon que le jour est à 1 ou 2 chiffres
        int x_offset = (i < 10) ? 3 : 0;
        
        // Position de base commune à tous les jours
        int x_pos = start_x + ((cell % 7) * cell_width) + x_offset;
        int y_pos = start_y + ((cell / 7) * cell_height);
        
        // Jour courant ou non
        if (i == day) {
            // Configurer le style selon le nombre de chiffres
            lv_style_set_pad_left(&style_current_day, (i < 10) ? 8 : 4);
            lv_style_set_pad_right(&style_current_day, (i < 10) ? 8 : 4);
            
            // Ajustement supplémentaire pour le jour courant à 1 chiffre
            if (i < 10) {
                x_pos -= 8;
            }
            else {
                x_pos += -4;
            }
            
            // Appliquer le style du jour courant
            lv_obj_add_style(calendar_days[cell], &style_current_day, LV_PART_MAIN);
            current_day = cell;
        } else {
            // Vérifie si c'est un samedi (5) ou dimanche (6) dans la grille du calendrier
            if ((cell % 7) == 5 || (cell % 7) == 6) {
                // Style pour weekend (gris)
                lv_obj_set_style_text_color(calendar_days[cell], lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);
            } else {
                // Style normal pour les jours de semaine (blanc)
                lv_obj_set_style_text_color(calendar_days[cell], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            lv_obj_remove_style(calendar_days[cell], &style_current_day, LV_PART_MAIN);
        }
        
        // Positionner l'élément
        lv_obj_set_pos(calendar_days[cell], x_pos, y_pos);
        
        cell++;
    }
    
    // Cacher les cellules non utilisées
    for (; cell < 42; cell++) {
        if (calendar_days[cell] != NULL) {
            lv_label_set_text(calendar_days[cell], "");
            lv_obj_remove_style(calendar_days[cell], &style_current_day, LV_PART_MAIN);
        }
    }


}

// Dans votre fonction de gestion des événements ou dans un timer
void calendar_update_task(lv_timer_t *timer) {
    update_calendar();
}

// Ajouter cette fonction qui sera appelée lorsqu'un événement se produit sur l'écran
static void screen7_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    // Vérifier si c'est l'événement "écran chargé"
    if(code == LV_EVENT_SCREEN_LOADED) {
        // Mettre à jour immédiatement le calendrier
        update_calendar();
        
        // Optionnellement, créer un timer pour des mises à jour périodiques
        // lv_timer_t *update_timer = lv_timer_create(calendar_update_task, 60000, NULL); // Mise à jour toutes les minutes
    }
}

