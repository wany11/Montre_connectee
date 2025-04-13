// This file is for the date and time setting screen
// LVGL version: 8.3.11

#include "../ui.h"
#include <time.h>
#include <stdio.h>
#include "../../../inc/rtc.h"
#include <zephyr/kernel.h>
#include "../../../inc/timSec.h"

// External variables from RTC module
extern uint16_t g_current_year;
extern uint8_t g_current_month;
extern uint8_t g_current_day;
extern uint8_t g_current_hour;
extern uint8_t g_current_minute;
extern uint8_t g_current_second;
extern int rtc_sync_global_time(void);
extern int rtc_set_datetime(uint16_t year, uint8_t month, uint8_t day,
                           uint8_t hour, uint8_t minute, uint8_t second);

// Local variables for the date and time being set
static uint16_t set_year;
static uint8_t set_month;
static uint8_t set_day;
static uint8_t set_hour;
static uint8_t set_minute;
static uint8_t set_second;

// Labels for displaying the values
static lv_obj_t *year_label;
static lv_obj_t *month_label;
static lv_obj_t *day_label;
static lv_obj_t *hour_label;
static lv_obj_t *minute_label;
static lv_obj_t *second_label;

// Button event handlers
static void year_up_event_cb(lv_event_t *e);
static void year_down_event_cb(lv_event_t *e);
static void month_up_event_cb(lv_event_t *e);
static void month_down_event_cb(lv_event_t *e);
static void day_up_event_cb(lv_event_t *e);
static void day_down_event_cb(lv_event_t *e);
static void hour_up_event_cb(lv_event_t *e);
static void hour_down_event_cb(lv_event_t *e);
static void minute_up_event_cb(lv_event_t *e);
static void minute_down_event_cb(lv_event_t *e);
static void second_up_event_cb(lv_event_t *e);
static void second_down_event_cb(lv_event_t *e);
static void save_button_event_cb(lv_event_t *e);

// Update the displayed values
static void update_datetime_labels(void);

// Helper function to create up/down buttons with arrow symbols
static lv_obj_t* create_arrow_button(lv_obj_t *parent, int x, int y, const char* symbol, 
                                    lv_event_cb_t event_cb);

// Helper function to check days in month
static uint8_t get_days_in_month(uint16_t year, uint8_t month);

void ui_Screen8_screen_init(void)
{
    
    // Check if ui_Screen8 already exists and delete it if needed
    if (ui_Screen8 != NULL) {
        lv_obj_del(ui_Screen8);
        ui_Screen8 = NULL;
    }

    // Initialize with current time - add error handling
    if (rtc_sync_global_time() != 0) {
        // Use default values if RTC update fails
        set_year = 2023;
        set_month = 1;
        set_day = 1;
        set_hour = 0;
        set_minute = 0;
        set_second = 0;
    } else {
        set_year = g_current_year;
        set_month = g_current_month;
        set_day = g_current_day;
        set_hour = g_current_hour;
        set_minute = g_current_minute;
        set_second = g_current_second;
    }
    
    // Create the base screen
    ui_Screen8 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen8, LV_OBJ_FLAG_SCROLLABLE);

    // Create a base container with the same style as other screens
    lv_obj_t *main_container = lv_obj_create(ui_Screen8);
    lv_obj_set_width(main_container, 244);
    lv_obj_set_height(main_container, 244);
    lv_obj_set_align(main_container, LV_ALIGN_CENTER);
    lv_obj_clear_flag(main_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(main_container, 10000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(main_container, lv_color_hex(0x262F29), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(main_container, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Title
    lv_obj_t *title = lv_label_create(ui_Screen8);
    lv_obj_set_width(title, LV_SIZE_CONTENT);
    lv_obj_set_height(title, LV_SIZE_CONTENT);
    lv_obj_set_x(title, 0);
    lv_obj_set_y(title, -100);
    lv_obj_set_align(title, LV_ALIGN_CENTER);
    lv_label_set_text(title, "Date & Time Settings");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Date section title
    lv_obj_t *date_title = lv_label_create(ui_Screen8);
    lv_obj_set_width(date_title, LV_SIZE_CONTENT);
    lv_obj_set_height(date_title, LV_SIZE_CONTENT);
    lv_obj_set_x(date_title, -70);
    lv_obj_set_y(date_title, -70);
    lv_obj_set_align(date_title, LV_ALIGN_CENTER);
    lv_label_set_text(date_title, "Date");
    lv_obj_set_style_text_color(date_title, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Time section title
    lv_obj_t *time_title = lv_label_create(ui_Screen8);
    lv_obj_set_width(time_title, LV_SIZE_CONTENT);
    lv_obj_set_height(time_title, LV_SIZE_CONTENT);
    lv_obj_set_x(time_title, 70);
    lv_obj_set_y(time_title, -70);
    lv_obj_set_align(time_title, LV_ALIGN_CENTER);
    lv_label_set_text(time_title, "Time");
    lv_obj_set_style_text_color(time_title, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Create labels for the date fields
    lv_obj_t *year_desc = lv_label_create(ui_Screen8);
    lv_obj_set_x(year_desc, -90);
    lv_obj_set_y(year_desc, -40);
    lv_obj_set_align(year_desc, LV_ALIGN_CENTER);
    lv_label_set_text(year_desc, "Year:");
    lv_obj_set_style_text_color(year_desc, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    year_label = lv_label_create(ui_Screen8);
    lv_obj_set_x(year_label, -60);
    lv_obj_set_y(year_label, -40);
    lv_obj_set_align(year_label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(year_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Month
    lv_obj_t *month_desc = lv_label_create(ui_Screen8);
    lv_obj_set_x(month_desc, -90);
    lv_obj_set_y(month_desc, -10);
    lv_obj_set_align(month_desc, LV_ALIGN_CENTER);
    lv_label_set_text(month_desc, "Month:");
    lv_obj_set_style_text_color(month_desc, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    month_label = lv_label_create(ui_Screen8);
    lv_obj_set_x(month_label, -60);
    lv_obj_set_y(month_label, -10);
    lv_obj_set_align(month_label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(month_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Day
    lv_obj_t *day_desc = lv_label_create(ui_Screen8);
    lv_obj_set_x(day_desc, -90);
    lv_obj_set_y(day_desc, 20);
    lv_obj_set_align(day_desc, LV_ALIGN_CENTER);
    lv_label_set_text(day_desc, "Day:");
    lv_obj_set_style_text_color(day_desc, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    day_label = lv_label_create(ui_Screen8);
    lv_obj_set_x(day_label, -60);
    lv_obj_set_y(day_label, 20);
    lv_obj_set_align(day_label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(day_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Create labels for the time fields
    lv_obj_t *hour_desc = lv_label_create(ui_Screen8);
    lv_obj_set_x(hour_desc, 40);
    lv_obj_set_y(hour_desc, -40);
    lv_obj_set_align(hour_desc, LV_ALIGN_CENTER);
    lv_label_set_text(hour_desc, "Hour:");
    lv_obj_set_style_text_color(hour_desc, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    hour_label = lv_label_create(ui_Screen8);
    lv_obj_set_x(hour_label, 70);
    lv_obj_set_y(hour_label, -40);
    lv_obj_set_align(hour_label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(hour_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Minute
    lv_obj_t *minute_desc = lv_label_create(ui_Screen8);
    lv_obj_set_x(minute_desc, 40);
    lv_obj_set_y(minute_desc, -10);
    lv_obj_set_align(minute_desc, LV_ALIGN_CENTER);
    lv_label_set_text(minute_desc, "Min:");
    lv_obj_set_style_text_color(minute_desc, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    minute_label = lv_label_create(ui_Screen8);
    lv_obj_set_x(minute_label, 70);
    lv_obj_set_y(minute_label, -10);
    lv_obj_set_align(minute_label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(minute_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Second
    lv_obj_t *second_desc = lv_label_create(ui_Screen8);
    lv_obj_set_x(second_desc, 40);
    lv_obj_set_y(second_desc, 20);
    lv_obj_set_align(second_desc, LV_ALIGN_CENTER);
    lv_label_set_text(second_desc, "Sec:");
    lv_obj_set_style_text_color(second_desc, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    second_label = lv_label_create(ui_Screen8);
    lv_obj_set_x(second_label, 70);
    lv_obj_set_y(second_label, 20);
    lv_obj_set_align(second_label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(second_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Create buttons for date
    create_arrow_button(ui_Screen8, -30, -50, LV_SYMBOL_UP, year_up_event_cb);
    create_arrow_button(ui_Screen8, -30, -30, LV_SYMBOL_DOWN, year_down_event_cb);
    
    create_arrow_button(ui_Screen8, -30, -10, LV_SYMBOL_UP, month_up_event_cb);
    create_arrow_button(ui_Screen8, -30, 10, LV_SYMBOL_DOWN, month_down_event_cb);
    
    create_arrow_button(ui_Screen8, -30, 30, LV_SYMBOL_UP, day_up_event_cb);
    create_arrow_button(ui_Screen8, -30, 50, LV_SYMBOL_DOWN, day_down_event_cb);

    // Create buttons for time
    create_arrow_button(ui_Screen8, 100, -50, LV_SYMBOL_UP, hour_up_event_cb);
    create_arrow_button(ui_Screen8, 100, -30, LV_SYMBOL_DOWN, hour_down_event_cb);
    
    create_arrow_button(ui_Screen8, 100, -10, LV_SYMBOL_UP, minute_up_event_cb);
    create_arrow_button(ui_Screen8, 100, 10, LV_SYMBOL_DOWN, minute_down_event_cb);
    
    create_arrow_button(ui_Screen8, 100, 30, LV_SYMBOL_UP, second_up_event_cb);
    create_arrow_button(ui_Screen8, 100, 50, LV_SYMBOL_DOWN, second_down_event_cb);

    // Save button
    lv_obj_t *save_btn = lv_btn_create(ui_Screen8);
    lv_obj_set_width(save_btn, 120);
    lv_obj_set_height(save_btn, 40);
    lv_obj_set_x(save_btn, 0);
    lv_obj_set_y(save_btn, 80);
    lv_obj_set_align(save_btn, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(save_btn, save_button_event_cb, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *save_label = lv_label_create(save_btn);
    lv_obj_set_align(save_label, LV_ALIGN_CENTER);
    lv_label_set_text(save_label, "Save");

    // Update displayed values
    update_datetime_labels();
    
}

static lv_obj_t* create_arrow_button(lv_obj_t *parent, int x, int y, const char* symbol, 
                                    lv_event_cb_t event_cb) {
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_width(btn, 20);
    lv_obj_set_height(btn, 20);
    lv_obj_set_x(btn, x);
    lv_obj_set_y(btn, y);
    lv_obj_set_align(btn, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, symbol);
    
    return btn;
}

static void update_datetime_labels(void) {
    char buffer[16];
    
    // Update year
    snprintf(buffer, sizeof(buffer), "%d", set_year);
    lv_label_set_text(year_label, buffer);
    
    // Update month
    snprintf(buffer, sizeof(buffer), "%02d", set_month);
    lv_label_set_text(month_label, buffer);
    
    // Update day
    snprintf(buffer, sizeof(buffer), "%02d", set_day);
    lv_label_set_text(day_label, buffer);
    
    // Update hour
    snprintf(buffer, sizeof(buffer), "%02d", set_hour);
    lv_label_set_text(hour_label, buffer);
    
    // Update minute
    snprintf(buffer, sizeof(buffer), "%02d", set_minute);
    lv_label_set_text(minute_label, buffer);
    
    // Update second
    snprintf(buffer, sizeof(buffer), "%02d", set_second);
    lv_label_set_text(second_label, buffer);
}

static uint8_t get_days_in_month(uint16_t year, uint8_t month) {
    if (month == 2) { // February
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            return 29; // Leap year
        } else {
            return 28; // Non-leap year
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30; // April, June, September, November
    } else {
        return 31; // All other months
    }
}

// Year event handlers
static void year_up_event_cb(lv_event_t *e) {
    if (set_year < 2099) {
        set_year++;
        
        // Adjust day if necessary for February in leap years
        uint8_t days_in_month = get_days_in_month(set_year, set_month);
        if (set_day > days_in_month) {
            set_day = days_in_month;
        }
        
        update_datetime_labels();
    }
}

static void year_down_event_cb(lv_event_t *e) {
    if (set_year > 2000) {
        set_year--;
        
        // Adjust day if necessary for February in leap years
        uint8_t days_in_month = get_days_in_month(set_year, set_month);
        if (set_day > days_in_month) {
            set_day = days_in_month;
        }
        
        update_datetime_labels();
    }
}

// Month event handlers
static void month_up_event_cb(lv_event_t *e) {
    if (set_month < 12) {
        set_month++;
    } else {
        set_month = 1;
    }
    
    // Adjust day if necessary for the new month
    uint8_t days_in_month = get_days_in_month(set_year, set_month);
    if (set_day > days_in_month) {
        set_day = days_in_month;
    }
    
    update_datetime_labels();
}

static void month_down_event_cb(lv_event_t *e) {
    if (set_month > 1) {
        set_month--;
    } else {
        set_month = 12;
    }
    
    // Adjust day if necessary for the new month
    uint8_t days_in_month = get_days_in_month(set_year, set_month);
    if (set_day > days_in_month) {
        set_day = days_in_month;
    }
    
    update_datetime_labels();
}

// Day event handlers
static void day_up_event_cb(lv_event_t *e) {
    uint8_t days_in_month = get_days_in_month(set_year, set_month);
    
    if (set_day < days_in_month) {
        set_day++;
    } else {
        set_day = 1;
    }
    
    update_datetime_labels();
}

static void day_down_event_cb(lv_event_t *e) {
    uint8_t days_in_month = get_days_in_month(set_year, set_month);
    
    if (set_day > 1) {
        set_day--;
    } else {
        set_day = days_in_month;
    }
    
    update_datetime_labels();
}

// Hour event handlers
static void hour_up_event_cb(lv_event_t *e) {
    if (set_hour < 23) {
        set_hour++;
    } else {
        set_hour = 0;
    }
    
    update_datetime_labels();
}

static void hour_down_event_cb(lv_event_t *e) {
    if (set_hour > 0) {
        set_hour--;
    } else {
        set_hour = 23;
    }
    
    update_datetime_labels();
}

// Minute event handlers
static void minute_up_event_cb(lv_event_t *e) {
    if (set_minute < 59) {
        set_minute++;
    } else {
        set_minute = 0;
    }
    
    update_datetime_labels();
}

static void minute_down_event_cb(lv_event_t *e) {
    if (set_minute > 0) {
        set_minute--;
    } else {
        set_minute = 59;
    }
    
    update_datetime_labels();
}

// Second event handlers
static void second_up_event_cb(lv_event_t *e) {
    if (set_second < 59) {
        set_second++;
    } else {
        set_second = 0;
    }
    
    update_datetime_labels();
}

static void second_down_event_cb(lv_event_t *e) {
    if (set_second > 0) {
        set_second--;
    } else {
        set_second = 59;
    }
    
    update_datetime_labels();
}

// Save button event handler
static void close_msgbox_event_handler(lv_event_t * e) 
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_msgbox_close(obj);
}

static void close_msgbox_timer_cb(lv_timer_t *t) 
{
    lv_obj_t *msgbox = (lv_obj_t *)t->user_data;
    if (msgbox != NULL) {
        lv_msgbox_close(msgbox);
    }
}

static void save_button_event_cb(lv_event_t *e) 
{    
    // Set the RTC with the new time
    int ret = set_time(set_hour, set_minute, set_second, set_year, set_month, set_day);
    
    static const char * btns[] = {"OK", ""};
    lv_obj_t *msgbox;
    
    if (ret == 0) {
        // Success, update global time variables
        rtc_sync_global_time();
        
        // Show success message
        msgbox = lv_msgbox_create(NULL, "Success", "Date and time updated", btns, false);
    } else {
        // Error with RTC hardware, but we should still update our software variables
        // to maintain functionality even if hardware is not responding
        g_current_year = set_year;
        g_current_month = set_month;
        g_current_day = set_day;
        g_current_hour = set_hour;
        g_current_minute = set_minute;
        g_current_second = set_second;
        
        // Show partial success message
        msgbox = lv_msgbox_create(NULL, "Warning", "RTC hardware not detected.\nSoftware time updated.", btns, false);
    }
    
    lv_obj_center(msgbox);
    lv_obj_add_event_cb(msgbox, close_msgbox_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    
    // Auto-close after 2 seconds
    lv_timer_t *timer = lv_timer_create(close_msgbox_timer_cb, 2000, msgbox);
    lv_timer_set_repeat_count(timer, 1);
}