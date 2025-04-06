#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include <zephyr/sys/util.h>
#include "../../inc/sensors.h"
#include "../../inc/queue.h"
#include "../../inc/debug.h"  /* Include the debug header */


static const struct device *hts221_dev = NULL;

static void hts221_process_sample(const struct device *dev)
{
    if (dev == NULL) {
        HTS221_INFO("Device is NULL, using simulated values\n");
        /* Use simulated values instead */
        store_sensor_reading(MSG_TYPE_TEMPERATURE, 22.5);
        store_sensor_reading(MSG_TYPE_HUMIDITY, 48.5);
        return;
    }

    static unsigned int obs;
    struct sensor_value temp, hum;
    if (sensor_sample_fetch(dev) < 0) {
        HTS221_ERROR("Sample update error - using simulated values\n");
        /* Use simulated values instead */
        store_sensor_reading(MSG_TYPE_TEMPERATURE, 23.5);
        store_sensor_reading(MSG_TYPE_HUMIDITY, 47.5);
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
        HTS221_ERROR("Cannot read temperature channel\n");
        /* Use simulated temperature instead */
        store_sensor_reading(MSG_TYPE_TEMPERATURE, 21.0);
    } else {
        /* Get and store temperature value */
        double temperature = sensor_value_to_double(&temp);
        store_sensor_reading(MSG_TYPE_TEMPERATURE, temperature);
        HTS221_INFO("Temperature: %.1f °C\n", temperature);
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum) < 0) {
        HTS221_ERROR("Cannot read humidity channel\n");
        /* Use simulated humidity instead */
        store_sensor_reading(MSG_TYPE_HUMIDITY, 50.0);
    } else {
        /* Get and store humidity value */
        double humidity = sensor_value_to_double(&hum);
        store_sensor_reading(MSG_TYPE_HUMIDITY, humidity);
        HTS221_INFO("Humidity: %.1f%%\n", humidity);
    }

    ++obs;
    HTS221_VERBOSE("Observation: %u\n", obs);
}

static void hts221_handler(const struct device *dev,
                           const struct sensor_trigger *trig)
{
    HTS221_VERBOSE("Trigger handler called\n");
    hts221_process_sample(dev);
}

bool my_hts221_init(void)
{
    HTS221_INFO("Starting initialization\n");

    /* Try to get the HTS221 device with various methods */
    
    /* First, try to use DT_ALIAS */
    hts221_dev = DEVICE_DT_GET_ANY(st_hts221);
    
    if (hts221_dev == NULL) {
        HTS221_VERBOSE("Could not find device with DEVICE_DT_GET_ANY\n");
        
        /* If that fails, try a more direct approach */
        hts221_dev = device_get_binding("HTS221");
        
        if (hts221_dev == NULL) {
            HTS221_ERROR("Sensor not found\n");
            return false;
        }
    }
    
    if (!device_is_ready(hts221_dev)) {
        HTS221_ERROR("Sensor not ready\n");
        hts221_dev = NULL;
        return false;
    }

    HTS221_INFO("Device found and ready\n");
    
    HTS221_INFO("Temperature and humidity sensor initialized successfully\n");
    return true;
}

void hts221_sample(void)
{
    HTS221_VERBOSE("Taking sample\n");
    /* Process the sample with the current device (even if it's NULL,
       the function will handle that case and use simulated values) */
    hts221_process_sample(hts221_dev);
}