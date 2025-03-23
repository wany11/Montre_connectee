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

/* LSM6DSO message types to enum in queue.h:
 * MSG_TYPE_ACCEL_X, MSG_TYPE_ACCEL_Y, MSG_TYPE_ACCEL_Z,
 * MSG_TYPE_GYRO_X, MSG_TYPE_GYRO_Y, MSG_TYPE_GYRO_Z
 */

static const struct device *lsm6dso_dev = NULL;

static inline float out_ev(struct sensor_value *val)
{
    return (val->val1 + (float)val->val2 / 1000000);
}

static void lsm6dso_process_sample(const struct device *dev)
{
    if (dev == NULL) {
        printk("LSM6DSO device is NULL, using simulated values\n");
        /* Use simulated values */
        store_sensor_reading(MSG_TYPE_ACCEL_X, 0.0);
        store_sensor_reading(MSG_TYPE_ACCEL_Y, 0.0);
        store_sensor_reading(MSG_TYPE_ACCEL_Z, 9.8);
        store_sensor_reading(MSG_TYPE_GYRO_X, 0.0);
        store_sensor_reading(MSG_TYPE_GYRO_Y, 0.0);
        store_sensor_reading(MSG_TYPE_GYRO_Z, 0.0);
        return;
    }

    struct sensor_value x, y, z;
    static int obs;

    /* lsm6dso accel */
    if (sensor_sample_fetch_chan(dev, SENSOR_CHAN_ACCEL_XYZ) < 0) {
        printk("LSM6DSO accel fetch error\n");
        return;
    }
    
    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &x);
    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &y);
    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &z);

    double accel_x = out_ev(&x);
    double accel_y = out_ev(&y);
    double accel_z = out_ev(&z);
    
    store_sensor_reading(MSG_TYPE_ACCEL_X, accel_x);
    store_sensor_reading(MSG_TYPE_ACCEL_Y, accel_y);
    store_sensor_reading(MSG_TYPE_ACCEL_Z, accel_z);

    printk("Accel x:%f m/s² y:%f m/s² z:%f m/s²\n", accel_x, accel_y, accel_z);

    /* lsm6dso gyro */
    if (sensor_sample_fetch_chan(dev, SENSOR_CHAN_GYRO_XYZ) < 0) {
        printk("LSM6DSO gyro fetch error\n");
        return;
    }
    
    sensor_channel_get(dev, SENSOR_CHAN_GYRO_X, &x);
    sensor_channel_get(dev, SENSOR_CHAN_GYRO_Y, &y);
    sensor_channel_get(dev, SENSOR_CHAN_GYRO_Z, &z);

    double gyro_x = out_ev(&x);
    double gyro_y = out_ev(&y);
    double gyro_z = out_ev(&z);
    
    store_sensor_reading(MSG_TYPE_GYRO_X, gyro_x);
    store_sensor_reading(MSG_TYPE_GYRO_Y, gyro_y);
    store_sensor_reading(MSG_TYPE_GYRO_Z, gyro_z);

    printk("Gyro x:%f rad/s y:%f rad/s z:%f rad/s\n", gyro_x, gyro_y, gyro_z);

    ++obs;
    printk("LSM6DSO Observation: %u\n\n", obs);
}

static int set_sampling_freq(const struct device *dev)
{
    int ret = 0;
    struct sensor_value odr_attr;

    /* set accel/gyro sampling frequency to 12.5 Hz */
    odr_attr.val1 = 12.5;
    odr_attr.val2 = 0;

    ret = sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ,
            SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
    if (ret != 0) {
        printk("Cannot set sampling frequency for accelerometer.\n");
        return ret;
    }

    ret = sensor_attr_set(dev, SENSOR_CHAN_GYRO_XYZ,
            SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
    if (ret != 0) {
        printk("Cannot set sampling frequency for gyro.\n");
        return ret;
    }

    return 0;
}

static void lsm6dso_handler(const struct device *dev,
               const struct sensor_trigger *trig)
{
    lsm6dso_process_sample(dev);
}

bool my_lsm6dso_init(void)
{
    printk("Starting LSM6DSO initialization\n");

    /* Try to get the LSM6DSO device */
    lsm6dso_dev = DEVICE_DT_GET_ANY(st_lsm6dso);
    
    if (lsm6dso_dev == NULL) {
        printk("Could not find LSM6DSO with DEVICE_DT_GET_ANY\n");
        
        /* Try alternate method */
        lsm6dso_dev = device_get_binding("LSM6DSO");
        
        if (lsm6dso_dev == NULL) {
            printk("LSM6DSO sensor: device not found.\n");
            return false;
        }
    }
    
    if (!device_is_ready(lsm6dso_dev)) {
        printk("LSM6DSO sensor: device not ready.\n");
        lsm6dso_dev = NULL;
        return false;
    }

    printk("LSM6DSO device found and ready\n");

    if (set_sampling_freq(lsm6dso_dev) != 0) {
        printk("Failed to set sampling frequency\n");
        return false;
    }

#ifdef CONFIG_LSM6DSO_TRIGGER
    printk("Setting up LSM6DSO trigger\n");
    struct sensor_trigger trig = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_ACCEL_XYZ,
    };
    if (sensor_trigger_set(lsm6dso_dev, &trig, lsm6dso_handler) < 0) {
        printk("Cannot configure LSM6DSO trigger\n");
        return false;
    }
#endif
    
    printk("LSM6DSO accelerometer and gyroscope initialized successfully\n");
    return true;
}

void lsm6dso_sample(void)
{
    /* Process the sample with the current device (even if it's NULL,
       the function will handle that case and use simulated values) */
    lsm6dso_process_sample(lsm6dso_dev);
}
 
//  int main(void)
//  {
//      const struct device *const dev = DEVICE_DT_GET_ONE(st_lsm6dso);
 
//      if (!device_is_ready(dev)) {
//          printk("%s: device not ready.\n", dev->name);
//          return 0;
//      }
 
//  #ifdef CONFIG_LSM6DSO_TRIGGER
//      printf("Testing LSM6DSO sensor in trigger mode.\n\n");
//      test_trigger_mode(dev);
//  #else
//      printf("Testing LSM6DSO sensor in polling mode.\n\n");
//      test_polling_mode(dev);
//  #endif
//      return 0;
//  }
 