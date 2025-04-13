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
 #include "../../inc/debug.h"  /* Include the debug header */
 
 static const struct device *lis2mdl_dev = NULL;
 
 static inline float out_ev(struct sensor_value *val)
 {
     return (val->val1 + (float)val->val2 / 1000000);
 }
 
 static void lis2mdl_process_sample(const struct device *dev)
 {
     if (dev == NULL) {
         LIS2MDL_INFO("Device is NULL, using simulated values\n");
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
         LIS2MDL_ERROR("Magnetometer fetch error\n");
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
 
     LIS2MDL_INFO("Magnetic field x:%.2f uT y:%.2f uT z:%.2f uT\n", mag_x, mag_y, mag_z);
 
     ++obs;
     LIS2MDL_VERBOSE("Observation: %u\n", obs);
 }
 
 static int set_sampling_freq(const struct device *dev) __attribute__((unused));

 static int set_sampling_freq(const struct device *dev)
 {
     int ret = 0;
     struct sensor_value odr_attr;
 
     /* set magnetometer sampling frequency to 10 Hz (a typical value for LIS2MDL) */
     odr_attr.val1 = 10;
     odr_attr.val2 = 0;
 
     LIS2MDL_INFO("Setting sampling frequency to %d Hz\n", odr_attr.val1);
     ret = sensor_attr_set(dev, SENSOR_CHAN_MAGN_XYZ,
             SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
     if (ret != 0) {
         LIS2MDL_ERROR("Cannot set sampling frequency for magnetometer (err: %d)\n", ret);
         return ret;
     }
 
     return 0;
 }
 
 static void lis2mdl_handler(const struct device *dev,
                const struct sensor_trigger *trig) __attribute__((unused));
                
static void lis2mdl_handler(const struct device *dev,
                const struct sensor_trigger *trig)
{
    LIS2MDL_VERBOSE("Trigger handler called\n");
    lis2mdl_process_sample(dev);
}

 bool my_lis2mdl_init(void)
 {
     LIS2MDL_INFO("Starting initialization\n");
 
     /* Try to get the lis2mdl device */
     lis2mdl_dev = DEVICE_DT_GET_ANY(st_lis2mdl);
     
     if (lis2mdl_dev == NULL) {
         LIS2MDL_VERBOSE("Could not find device with DEVICE_DT_GET_ANY\n");
         
         /* Try alternate method */
         lis2mdl_dev = device_get_binding("LIS2MDL");
         
         if (lis2mdl_dev == NULL) {
             LIS2MDL_ERROR("Sensor not found\n");
             return false;
         }
     }
     
     if (!device_is_ready(lis2mdl_dev)) {
         LIS2MDL_ERROR("Sensor not ready\n");
         lis2mdl_dev = NULL;
         return false;
     }
 
     LIS2MDL_INFO("Device found and ready\n");
 
    //  if (set_sampling_freq(lis2mdl_dev) != 0) {
    //      LIS2MDL_ERROR("Failed to set sampling frequency\n");
    //      return false;
    //  }
     
     LIS2MDL_INFO("Magnetometer initialized successfully\n");
     return true;
 }
 
 void lis2mdl_sample(void)
 {
     LIS2MDL_VERBOSE("Taking sample\n");
     /* Process the sample with the current device (even if it's NULL,
        the function will handle that case and use simulated values) */
     lis2mdl_process_sample(lis2mdl_dev);
 }