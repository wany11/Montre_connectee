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
         LSM6DSO_INFO("Device is NULL, using simulated values\n");
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
         LSM6DSO_ERROR("Accelerometer fetch error\n");
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
 
     LSM6DSO_INFO("Accel x:%.3f m/s² y:%.3f m/s² z:%.3f m/s²\n", accel_x, accel_y, accel_z);
 
     /* lsm6dso gyro */
     if (sensor_sample_fetch_chan(dev, SENSOR_CHAN_GYRO_XYZ) < 0) {
         LSM6DSO_ERROR("Gyroscope fetch error\n");
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
 
     LSM6DSO_INFO("Gyro x:%.3f rad/s y:%.3f rad/s z:%.3f rad/s\n", gyro_x, gyro_y, gyro_z);
 
     ++obs;
     LSM6DSO_VERBOSE("Observation: %u\n", obs);
 }
 
 static int set_sampling_freq(const struct device *dev)
 {
     int ret = 0;
     struct sensor_value odr_attr;
 
     /* set accel/gyro sampling frequency to 12.5 Hz */
     odr_attr.val1 = 12.5;
     odr_attr.val2 = 0;
 
     LSM6DSO_INFO("Setting sampling frequency to %d Hz\n", odr_attr.val1);
     
     ret = sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ,
             SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
     if (ret != 0) {
         LSM6DSO_ERROR("Cannot set sampling frequency for accelerometer (err: %d)\n", ret);
         return ret;
     }
 
     ret = sensor_attr_set(dev, SENSOR_CHAN_GYRO_XYZ,
             SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
     if (ret != 0) {
         LSM6DSO_ERROR("Cannot set sampling frequency for gyroscope (err: %d)\n", ret);
         return ret;
     }
 
     LSM6DSO_INFO("Sampling frequency set successfully\n");
     return 0;
 }
 
 static void lsm6dso_handler(const struct device *dev,
                const struct sensor_trigger *trig)
 {
     LSM6DSO_VERBOSE("Trigger handler called\n");
     lsm6dso_process_sample(dev);
 }
 
 bool my_lsm6dso_init(void)
 {
     LSM6DSO_INFO("Starting initialization\n");
 
     /* Try to get the LSM6DSO device */
     lsm6dso_dev = DEVICE_DT_GET_ANY(st_lsm6dso);
     
     if (lsm6dso_dev == NULL) {
         LSM6DSO_VERBOSE("Could not find device with DEVICE_DT_GET_ANY\n");
         
         /* Try alternate method */
         lsm6dso_dev = device_get_binding("LSM6DSO");
         
         if (lsm6dso_dev == NULL) {
             LSM6DSO_ERROR("Sensor not found\n");
             return false;
         }
     }
     
     if (!device_is_ready(lsm6dso_dev)) {
         LSM6DSO_ERROR("Sensor not ready\n");
         lsm6dso_dev = NULL;
         return false;
     }
 
     LSM6DSO_INFO("Device found and ready\n");
 
     if (set_sampling_freq(lsm6dso_dev) != 0) {
         LSM6DSO_ERROR("Failed to set sampling frequency\n");
         return false;
     }
     
     LSM6DSO_INFO("Accelerometer and gyroscope initialized successfully\n");
     return true;
 }
 
 void lsm6dso_sample(void)
 {
     LSM6DSO_VERBOSE("Taking sample\n");
     /* Process the sample with the current device (even if it's NULL,
        the function will handle that case and use simulated values) */
     lsm6dso_process_sample(lsm6dso_dev);
 }