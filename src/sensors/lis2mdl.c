/*
 * Copyright (c) 2020 Yestin Sun
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include "../../inc/sensors.h"
#include "../../inc/queue.h"


static const struct device *lis2mdl_dev = NULL;

static inline float out_ev(struct sensor_value *val)
{
    return (val->val1 + (float)val->val2 / 1000000);
}

static void lis2mdl_process_sample(const struct device *dev)
{
    if (dev == NULL) {
        printk("lis2mdl device is NULL, using simulated values\n");
        /* Use simulated values for magnetometer */
        store_sensor_reading(MSG_TYPE_MAG_X, 0.0);
        store_sensor_reading(MSG_TYPE_MAG_Y, 0.0);
        store_sensor_reading(MSG_TYPE_MAG_Z, 0.0);
        return;
    }

    struct sensor_value x, y, z;
    static int obs;

    /* lis2mdl magnetometer */
    if (sensor_sample_fetch_chan(dev, SENSOR_CHAN_MAGN_XYZ) < 0) {
        printk("lis2mdl magnetometer fetch error\n");
        return;
    }
    
    sensor_channel_get(dev, SENSOR_CHAN_MAGN_X, &x);
    sensor_channel_get(dev, SENSOR_CHAN_MAGN_Y, &y);
    sensor_channel_get(dev, SENSOR_CHAN_MAGN_Z, &z);

    double mag_x = out_ev(&x);
    double mag_y = out_ev(&y);
    double mag_z = out_ev(&z);
    
    store_sensor_reading(MSG_TYPE_MAG_X, mag_x);
    store_sensor_reading(MSG_TYPE_MAG_Y, mag_y);
    store_sensor_reading(MSG_TYPE_MAG_Z, mag_z);

    printk("Magnetic field x:%f uT y:%f uT z:%f uT\n", mag_x, mag_y, mag_z);

    ++obs;
    printk("lis2mdl Observation: %u\n\n", obs);
}

static int set_sampling_freq(const struct device *dev)
{
    int ret = 0;
    struct sensor_value odr_attr;

    /* set magnetometer sampling frequency to 10 Hz (a typical value for LIS2MDL) */
    odr_attr.val1 = 10;
    odr_attr.val2 = 0;

    ret = sensor_attr_set(dev, SENSOR_CHAN_MAGN_XYZ,
            SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
    if (ret != 0) {
        printk("Cannot set sampling frequency for magnetometer.\n");
        return ret;
    }

    return 0;
}

static void lis2mdl_handler(const struct device *dev,
               const struct sensor_trigger *trig)
{
    lis2mdl_process_sample(dev);
}

bool my_lis2mdl_init(void)
{
    printk("Starting lis2mdl initialization\n");

    /* Try to get the lis2mdl device */
    lis2mdl_dev = DEVICE_DT_GET_ANY(st_lis2mdl);
    
    if (lis2mdl_dev == NULL) {
        printk("Could not find lis2mdl with DEVICE_DT_GET_ANY\n");
        
        /* Try alternate method */
        lis2mdl_dev = device_get_binding("LIS2MDL");
        
        if (lis2mdl_dev == NULL) {
            printk("lis2mdl sensor: device not found.\n");
            return false;
        }
    }
    
    if (!device_is_ready(lis2mdl_dev)) {
        printk("lis2mdl sensor: device not ready.\n");
        lis2mdl_dev = NULL;
        return false;
    }

    printk("lis2mdl device found and ready\n");

    if (set_sampling_freq(lis2mdl_dev) != 0) {
        printk("Failed to set sampling frequency\n");
        return false;
    }

#ifdef CONFIG_LIS2MDL_TRIGGER
    printk("Setting up lis2mdl trigger\n");
    struct sensor_trigger trig = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_MAGN_XYZ,
    };
    if (sensor_trigger_set(lis2mdl_dev, &trig, lis2mdl_handler) < 0) {
        printk("Cannot configure lis2mdl trigger\n");
        return false;
    }
#endif
    
    printk("lis2mdl magnetometer initialized successfully\n");
    return true;
}

void lis2mdl_sample(void)
{
    /* Process the sample with the current device (even if it's NULL,
       the function will handle that case and use simulated values) */
    lis2mdl_process_sample(lis2mdl_dev);
}