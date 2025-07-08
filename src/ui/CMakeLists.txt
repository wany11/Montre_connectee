SET(SOURCES components/ui_comp_image1.c
    components/ui_comp.c
    screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_1780325426.c
    images/ui_img_clockwise_hour_png.c
    images/ui_img_clockwise_min_png.c
    images/ui_img_clockwise_sec_png.c
    images/ui_img_weather_sun_cloud_png.c
    images/ui_img_weather_sun_png.c
    images/ui_img_weather_cloud_fog_png.c
    images/ui_img_weather_cloud_png.c)

add_library(ui ${SOURCES})
