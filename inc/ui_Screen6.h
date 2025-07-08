#ifndef UI_SCREEN6_H
#define UI_SCREEN6_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// États du chronomètre
typedef enum {
    CHRONO_STOPPED,
    CHRONO_RUNNING,
    CHRONO_PAUSED
} chrono_state_t;

// Fonctions pour l'interface utilisateur de l'écran 6
void ui_Screen6_screen_init(void);

// Fonctions pour le chronomètre
void chrono_button_handler(void);
void ui_reset_activity_chrono(void);
void ui_set_step_count(uint32_t steps);

#ifdef __cplusplus
}
#endif

#endif // UI_SCREEN6_H