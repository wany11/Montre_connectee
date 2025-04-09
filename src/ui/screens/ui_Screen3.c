// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: Proj_test

#include "../ui.h"

void ui_Screen3_screen_init(void)
{
    ui_Screen3 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Clock_Group2 = lv_obj_create(ui_Screen3);
    lv_obj_set_width(ui_Clock_Group2, 244);
    lv_obj_set_height(ui_Clock_Group2, 244);
    lv_obj_set_align(ui_Clock_Group2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Clock_Group2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Clock_Group2, 10000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Clock_Group2, lv_color_hex(0x262F29), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Clock_Group2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_HourLabel = lv_label_create(ui_Clock_Group2);
    lv_obj_set_width(ui_HourLabel, LV_SIZE_CONTENT);   /// 5
    lv_obj_set_height(ui_HourLabel, LV_SIZE_CONTENT);    /// 5
    lv_obj_set_x(ui_HourLabel, 0);
    lv_obj_set_y(ui_HourLabel, 0);
    lv_obj_set_align(ui_HourLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_HourLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_HourLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_HourLabel, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelTemp = lv_label_create(ui_Clock_Group2);
    lv_obj_set_width(ui_LabelTemp, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelTemp, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTemp, 70);
    lv_obj_set_y(ui_LabelTemp, -50);
    lv_obj_set_align(ui_LabelTemp, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_LabelTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTemp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelHumi = lv_label_create(ui_Clock_Group2);
    lv_obj_set_width(ui_LabelHumi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHumi, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHumi, -70);
    lv_obj_set_y(ui_LabelHumi, -50);
    lv_obj_set_align(ui_LabelHumi, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_LabelHumi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHumi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_nbPas = lv_label_create(ui_Clock_Group2);
    lv_obj_set_width(ui_nbPas, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_nbPas, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_nbPas, 0);
    lv_obj_set_y(ui_nbPas, -70);
    lv_obj_set_align(ui_nbPas, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_nbPas, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_nbPas, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image1 = lv_img_create(ui_Screen3);
    lv_img_set_src(ui_Image1, &ui_img_weather_sun_cloud_png);
    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);   /// 37
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);    /// 30
    lv_obj_set_x(ui_Image1, 0);
    lv_obj_set_y(ui_Image1, 61);
    lv_obj_set_align(ui_Image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image2 = lv_img_create(ui_Screen3);
    lv_img_set_src(ui_Image2, &ui_img_weather_sun_png);
    lv_obj_set_width(ui_Image2, LV_SIZE_CONTENT);   /// 28
    lv_obj_set_height(ui_Image2, LV_SIZE_CONTENT);    /// 28
    lv_obj_set_x(ui_Image2, 0);
    lv_obj_set_y(ui_Image2, 61);
    lv_obj_set_align(ui_Image2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image3 = lv_img_create(ui_Screen3);
    lv_img_set_src(ui_Image3, &ui_img_weather_cloud_fog_png);
    lv_obj_set_width(ui_Image3, LV_SIZE_CONTENT);   /// 37
    lv_obj_set_height(ui_Image3, LV_SIZE_CONTENT);    /// 29
    lv_obj_set_x(ui_Image3, 0);
    lv_obj_set_y(ui_Image3, 61);
    lv_obj_set_align(ui_Image3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image3, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image4 = lv_img_create(ui_Screen3);
    lv_img_set_src(ui_Image4, &ui_img_weather_cloud_png);
    lv_obj_set_width(ui_Image4, LV_SIZE_CONTENT);   /// 37
    lv_obj_set_height(ui_Image4, LV_SIZE_CONTENT);    /// 30
    lv_obj_set_x(ui_Image4, 0);
    lv_obj_set_y(ui_Image4, 61);
    lv_obj_set_align(ui_Image4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image4, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image5 = lv_img_create(ui_Screen3);
    lv_img_set_src(ui_Image5, &ui_img_step_png);
    lv_obj_set_width(ui_Image5, LV_SIZE_CONTENT);   /// 19
    lv_obj_set_height(ui_Image5, LV_SIZE_CONTENT);    /// 15
    lv_obj_set_x(ui_Image5, 0);
    lv_obj_set_y(ui_Image5, -50);
    lv_obj_set_align(ui_Image5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image5, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
}
