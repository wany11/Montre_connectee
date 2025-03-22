#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include <zephyr/sys/util.h>
#include "../inc/sensors.h"
#include "../inc/temp_queue.h"

static const struct device *hts221_dev = NULL;

static void hts221_process_sample(const struct device *dev)
{
    if (dev == NULL) {
        printk("HTS221 device is NULL, using simulated values\n");
        /* Use simulated values instead */
        store_sensor_reading(MSG_TYPE_TEMPERATURE, 22.5);
        store_sensor_reading(MSG_TYPE_HUMIDITY, 48.5);
        return;
    }

    static unsigned int obs;
    struct sensor_value temp, hum;
    if (sensor_sample_fetch(dev) < 0) {
        printk("HTS221 sample update error - using simulated values\n");
        /* Use simulated values instead */
        store_sensor_reading(MSG_TYPE_TEMPERATURE, 23.5);
        store_sensor_reading(MSG_TYPE_HUMIDITY, 47.5);
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
        printk("Cannot read HTS221 temperature channel\n");
        /* Use simulated temperature instead */
        store_sensor_reading(MSG_TYPE_TEMPERATURE, 21.0);
    } else {
        /* Get and store temperature value */
        double temperature = sensor_value_to_double(&temp);
        store_sensor_reading(MSG_TYPE_TEMPERATURE, temperature);
        printk("HTS221 Temperature: %.1f C\n", temperature);
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum) < 0) {
        printk("Cannot read HTS221 humidity channel\n");
        /* Use simulated humidity instead */
        store_sensor_reading(MSG_TYPE_HUMIDITY, 50.0);
    } else {
        /* Get and store humidity value */
        double humidity = sensor_value_to_double(&hum);
        store_sensor_reading(MSG_TYPE_HUMIDITY, humidity);
        printk("HTS221 Humidity: %.1f%%\n", humidity);
    }

    ++obs;
    printk("HTS221 Observation: %u\n", obs);
}

static void hts221_handler(const struct device *dev,
                           const struct sensor_trigger *trig)
{
    hts221_process_sample(dev);
}

bool my_hts221_init(void)
{
    printk("Starting HTS221 initialization\n");

    /* Try to get the HTS221 device with various methods */
    
    /* First, try to use DT_ALIAS */
    hts221_dev = DEVICE_DT_GET_ANY(st_hts221);
    
    if (hts221_dev == NULL) {
        printk("Could not find HTS221 with DEVICE_DT_GET_ANY\n");
        
        /* If that fails, try a more direct approach */
        hts221_dev = device_get_binding("HTS221");
        
        if (hts221_dev == NULL) {
            printk("HTS221 sensor: device not found with binding either.\n");
            return false;
        }
    }
    
    if (!device_is_ready(hts221_dev)) {
        printk("HTS221 sensor: device not ready.\n");
        hts221_dev = NULL;
        return false;
    }

    printk("HTS221 device found and ready\n");

    // if (IS_ENABLED(CONFIG_HTS221_TRIGGER)) {
    //     printk("Setting up HTS221 trigger\n");
    //     struct sensor_trigger trig = {
    //         .type = SENSOR_TRIG_DATA_READY,
    //         .chan = SENSOR_CHAN_ALL,
    //     };
    //     if (sensor_trigger_set(hts221_dev, &trig, hts221_handler) < 0) {
    //         printk("Cannot configure HTS221 trigger\n");
    //         return false;
    //     }
    // }
    
    printk("HTS221 temperature and humidity sensor initialized successfully\n");
    return true;
}

void hts221_sample(void)
{
    /* Process the sample with the current device (even if it's NULL,
       the function will handle that case and use simulated values) */
    hts221_process_sample(hts221_dev);
}