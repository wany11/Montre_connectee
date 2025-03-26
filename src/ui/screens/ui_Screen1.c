// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: Proj_test

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_HourLabel = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_HourLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_HourLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_HourLabel, 0);
    lv_obj_set_y(ui_HourLabel, -100);
    lv_obj_set_align(ui_HourLabel, LV_ALIGN_CENTER);

    ui_LabelSensor = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LabelSensor, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelSensor, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelSensor, -85);
    lv_obj_set_y(ui_LabelSensor, -60);
    lv_obj_set_align(ui_LabelSensor, LV_ALIGN_CENTER);

    ui_LabelTemp = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_LabelTemp, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelTemp, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTemp, 75);
    lv_obj_set_y(ui_LabelTemp, -60);
    lv_obj_set_align(ui_LabelTemp, LV_ALIGN_CENTER);

    ui_accel_x = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_accel_x, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_accel_x, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_accel_x, -110);
    lv_obj_set_y(ui_accel_x, -15);
    lv_obj_set_align(ui_accel_x, LV_ALIGN_CENTER);

    ui_accel_y = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_accel_y, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_accel_y, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_accel_y, 0);
    lv_obj_set_y(ui_accel_y, -15);
    lv_obj_set_align(ui_accel_y, LV_ALIGN_CENTER);

    ui_accel_z = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_accel_z, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_accel_z, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_accel_z, 100);
    lv_obj_set_y(ui_accel_z, -15);
    lv_obj_set_align(ui_accel_z, LV_ALIGN_CENTER);

    ui_gyro_x = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_gyro_x, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_gyro_x, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_gyro_x, -110);
    lv_obj_set_y(ui_gyro_x, 40);
    lv_obj_set_align(ui_gyro_x, LV_ALIGN_CENTER);

    ui_gyro_y = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_gyro_y, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_gyro_y, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_gyro_y, 0);
    lv_obj_set_y(ui_gyro_y, 40);
    lv_obj_set_align(ui_gyro_y, LV_ALIGN_CENTER);

    ui_gyro_z = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_gyro_z, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_gyro_z, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_gyro_z, 100);
    lv_obj_set_y(ui_gyro_z, 40);
    lv_obj_set_align(ui_gyro_z, LV_ALIGN_CENTER);

    ui_mag_x = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_mag_x, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_mag_x, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_mag_x, -110);
    lv_obj_set_y(ui_mag_x, 90);
    lv_obj_set_align(ui_mag_x, LV_ALIGN_CENTER);

    ui_mag_y = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_mag_y, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_mag_y, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_mag_y, 0);
    lv_obj_set_y(ui_mag_y, 90);
    lv_obj_set_align(ui_mag_y, LV_ALIGN_CENTER);

    ui_mag_z = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_mag_z, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_mag_z, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_mag_z, 100);
    lv_obj_set_y(ui_mag_z, 90);
    lv_obj_set_align(ui_mag_z, LV_ALIGN_CENTER);
}