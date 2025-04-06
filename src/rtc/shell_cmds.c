#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <stdlib.h> 
#include <string.h>
#include "../../inc/rtc.h"

/* ****************************** */
/*          Commande RTC          */
/* ****************************** */

/* Commande simplifiée pour définir l'heure */
static int cmd_rtc_set(const struct shell *sh, size_t argc, char **argv)
{
    if (argc != 4) {
        shell_print(sh, "Usage: rtc set <heures> <minutes> <secondes>");
        return 0;
    }

    int h, m, s;
    
    /* Conversion simple des arguments */
    h = atoi(argv[1]);
    m = atoi(argv[2]);
    s = atoi(argv[3]);
    
    /* Validation des valeurs */
    if (h < 0 || h > 23 || m < 0 || m > 59 || s < 0 || s > 59) {
        shell_print(sh, "Valeurs invalides: h(0-23) m(0-59) s(0-59)");
        return 0;
    }
    
    /* Récupérer la date actuelle */
    uint16_t annee;
    uint8_t jour, mois;
    uint8_t heure, minute, seconde;
    
    int ret_get = rtc_get_datetime(&annee, &mois, &jour, &heure, &minute, &seconde);
    if (ret_get) {
        shell_error(sh, "Erreur lors de la récupération de la date: %d", ret_get);
        return ret_get;
    }
    
    /* Définir l'heure avec la date actuelle */
    int ret = rtc_set_datetime(annee, mois, jour, h, m, s);
    if (ret) {
        shell_error(sh, "Erreur: %d", ret);
    } else {
        shell_print(sh, "Heure définie: %02d:%02d:%02d", h, m, s);
    }
    
    return 0;
}

/* Commande pour définir la date */
static int cmd_rtc_set_date(const struct shell *sh, size_t argc, char **argv)
{
    if (argc != 4) {
        shell_print(sh, "Usage: rtc setdate <jour(1-31)> <mois(1-12)> <année(2000-2099)>");
        return 0;
    }

    int jour, mois, annee;
    
    /* Conversion des arguments */
    jour = atoi(argv[1]);
    mois = atoi(argv[2]);
    annee = atoi(argv[3]);
    
    /* Validation des valeurs */
    if (jour < 1 || jour > 31 || mois < 1 || mois > 12 || annee < 2000 || annee > 2099) {
        shell_print(sh, "Valeurs invalides: jour(1-31) mois(1-12) année(2000-2099)");
        return 0;
    }
    
    /* Récupérer l'heure actuelle */
    uint16_t annee_actuelle;
    uint8_t jour_actuel, mois_actuel;
    uint8_t heure, minute, seconde;
    
    int ret_get = rtc_get_datetime(&annee_actuelle, &mois_actuel, &jour_actuel, &heure, &minute, &seconde);
    if (ret_get) {
        shell_error(sh, "Erreur lors de la récupération de l'heure: %d", ret_get);
        return ret_get;
    }
    
    /* Définir la date avec l'heure actuelle */
    int ret = rtc_set_datetime(annee, mois, jour, heure, minute, seconde);
    if (ret) {
        shell_error(sh, "Erreur de définition de la date: %d", ret);
    } else {
        shell_print(sh, "Date définie: %02d/%02d/%04d", jour, mois, annee);
    }
    
    return 0;
}

/* Commande pour afficher l'heure actuelle */
static int cmd_rtc_get(const struct shell *sh, size_t argc, char **argv)
{
    uint16_t annee;
    uint8_t jour, mois;
    uint8_t heure, minute, seconde;
    
    int ret = rtc_get_datetime(&annee, &mois, &jour, &heure, &minute, &seconde);
    if (ret) {
        shell_error(sh, "Erreur de lecture RTC: %d", ret);
    } else {
        shell_print(sh, "Heure RTC: %02d:%02d:%02d", heure, minute, seconde);
    }
    
    return 0;
}

/* Commande pour afficher la date actuelle */
static int cmd_rtc_get_date(const struct shell *sh, size_t argc, char **argv)
{
    uint16_t annee;
    uint8_t jour, mois;
    uint8_t heure, minute, seconde;
    
    int ret = rtc_get_datetime(&annee, &mois, &jour, &heure, &minute, &seconde);
    if (ret) {
        shell_error(sh, "Erreur de lecture de la date RTC: %d", ret);
    } else {
        shell_print(sh, "Date RTC: %02u/%02u/%04u", jour, mois, annee);
    }
    
    return 0;
}

/* Commande RTC principale */
static int cmd_rtc(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 1) {
        shell_help(sh);
        return 0;
    }

    shell_error(sh, "%s: commande inconnue", argv[1]);
    return -EINVAL;
}

/* Définition des sous-commandes */
SHELL_STATIC_SUBCMD_SET_CREATE(rtc_cmds,
    SHELL_CMD(set, NULL, "Définir l'heure (h m s)", cmd_rtc_set),
    SHELL_CMD(get, NULL, "Afficher l'heure actuelle", cmd_rtc_get),
    SHELL_CMD(setdate, NULL, "Définir la date (j m a)", cmd_rtc_set_date),
    SHELL_CMD(getdate, NULL, "Afficher la date actuelle", cmd_rtc_get_date),
    SHELL_SUBCMD_SET_END
);

/* Enregistrement de la commande principale */
SHELL_CMD_REGISTER(rtc, &rtc_cmds, "Commandes RTC", cmd_rtc);

// /* ****************************** */
// /*        Commande Sensors        */
// /* ****************************** */

// /* Déclaration des variables externes pour les périodes d'échantillonnage */
// extern uint32_t hts221_sampling_period_ms;
// extern uint32_t lis2mdl_sampling_period_ms;
// extern uint32_t lsm6dso_sampling_period_ms;

// /* Commande pour définir la période d'échantillonnage des capteurs */
// static int cmd_sensors_set_period(const struct shell *sh, size_t argc, char **argv)
// {
//     if (argc != 3) {
//         shell_print(sh, "Usage: sensors period <capteur> <période_sec>");
//         shell_print(sh, "Capteurs disponibles: hts221, lis2mdl, lsm6dso, tous");
//         return 0;
//     }

//     char *sensor = argv[1];
//     double period_sec = atof(argv[2]);  // Utiliser double au lieu de float
    
//     /* Vérifier que la période est valide (entre 0.1s et 60s) */
//     if (period_sec < 0.1 || period_sec > 60) {
//         shell_error(sh, "La période doit être entre 0.1 et 60 secondes");
//         return -EINVAL;
//     }
    
//     /* Convertir les secondes en millisecondes */
//     uint32_t period_ms = (uint32_t)(period_sec * 1000);
    
//     /* Définir la période pour le capteur spécifié ou tous les capteurs */
//     if (strcmp(sensor, "hts221") == 0) {
//         hts221_sampling_period_ms = period_ms;
//         shell_print(sh, "Période HTS221 définie à %.1f secondes", period_sec);
//     } 
//     else if (strcmp(sensor, "lis2mdl") == 0) {
//         lis2mdl_sampling_period_ms = period_ms;
//         shell_print(sh, "Période LIS2MDL définie à %.1f secondes", period_sec);
//     } 
//     else if (strcmp(sensor, "lsm6dso") == 0) {
//         lsm6dso_sampling_period_ms = period_ms;
//         shell_print(sh, "Période LSM6DSO définie à %.1f secondes", period_sec);
//     }
//     else if (strcmp(sensor, "tous") == 0) {
//         hts221_sampling_period_ms = period_ms;
//         lis2mdl_sampling_period_ms = period_ms;
//         lsm6dso_sampling_period_ms = period_ms;
//         shell_print(sh, "Période de tous les capteurs définie à %.1f secondes", period_sec);
//     }
//     else {
//         shell_error(sh, "Capteur inconnu: %s", sensor);
//         shell_print(sh, "Capteurs disponibles: hts221, lis2mdl, lsm6dso, tous");
//         return -EINVAL;
//     }
    
//     return 0;
// }

// /* Commande pour afficher les périodes d'échantillonnage actuelles */
// static int cmd_sensors_get_period(const struct shell *sh, size_t argc, char **argv)
// {
//     shell_print(sh, "Périodes d'échantillonnage actuelles:");
//     shell_print(sh, "HTS221: %.1f secondes", hts221_sampling_period_ms / 1000.0);
//     shell_print(sh, "LIS2MDL: %.1f secondes", lis2mdl_sampling_period_ms / 1000.0);
//     shell_print(sh, "LSM6DSO: %.1f secondes", lsm6dso_sampling_period_ms / 1000.0);
//     return 0;
// }

// /* Commande sensors principale */
// static int cmd_sensors(const struct shell *sh, size_t argc, char **argv)
// {
//     if (argc == 1) {
//         shell_help(sh);
//         return 0;
//     }

//     shell_error(sh, "%s: commande inconnue", argv[1]);
//     return -EINVAL;
// }

// /* Définition des sous-commandes pour sensors */
// SHELL_STATIC_SUBCMD_SET_CREATE(sensors_cmds,
//     SHELL_CMD(period, NULL, "Définir la période d'échantillonnage (capteur période_sec)", cmd_sensors_set_period),
//     SHELL_CMD(status, NULL, "Afficher les périodes d'échantillonnage actuelles", cmd_sensors_get_period),
//     SHELL_SUBCMD_SET_END
// );

// /* Enregistrement de la commande principale */
// SHELL_CMD_REGISTER(sensors, &sensors_cmds, "Commandes de configuration des capteurs", cmd_sensors);