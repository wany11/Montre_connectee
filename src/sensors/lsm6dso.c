/*
 * Copyright (c) 2020 Yestin Sun
 *
 * SPDX-License-Identifier: Apache-2.0
 */

 #include <stdio.h>
 #include <zephyr/kernel.h>
 #include <zephyr/device.h>
 #include <zephyr/drivers/sensor.h>
 #include <zephyr/zbus/zbus.h>
 #include <math.h>
 #include "../../inc/sensors.h"
 #include "../../inc/queue.h"
 #include "../../inc/debug.h"  /* Include the debug header */
 
 /* LSM6DSO message types to enum in queue.h:
  * MSG_TYPE_ACCEL_X, MSG_TYPE_ACCEL_Y, MSG_TYPE_ACCEL_Z,
  * MSG_TYPE_GYRO_X, MSG_TYPE_GYRO_Y, MSG_TYPE_GYRO_Z,
  * MSG_TYPE_STEP_COUNT
  */
 
 static const struct device *lsm6dso_dev = NULL;
 static int step_offset = 0;
 static int step_count = 0;
 
 /* Variables for step detection algorithm */
 static float acc_magnitude_prev = 0.0f;
 static bool step_detected = false;
 static float step_threshold = 2.0f;  // Adjustable threshold for step detection
 
 static inline float out_ev(struct sensor_value *val)
 {
     return (val->val1 + (float)val->val2 / 1000000);
 }

 /**
  * @brief Process accelerometer data to detect steps
  * 
  * @param x Accelerometer X value
  * @param y Accelerometer Y value
  * @param z Accelerometer Z value
  * @return true if a step was detected, false otherwise
  */
 static bool detect_step(float x, float y, float z)
 {
     // Calculate magnitude of acceleration vector
     float acc_magnitude = sqrtf(x*x + y*y + z*z);
     
     // Normalize by removing gravity (approximately 9.8 m/s²)
     float acc_magnitude_norm = fabsf(acc_magnitude - 9.8f);
     
     // Detect step on acceleration peak that exceeds threshold
     bool is_step = false;
     
     if (acc_magnitude_norm > step_threshold && acc_magnitude_prev <= step_threshold && !step_detected) {
         // Rising edge detected
         step_detected = true;
         is_step = true;
         step_count++;
         LSM6DSO_INFO("Step detected! Count: %d, Magnitude: %.2f###################################\n", 
                      step_count, (double)acc_magnitude_norm);  // Ajout du cast explicite
     } else if (acc_magnitude_norm <= (step_threshold * 0.7f) && acc_magnitude_prev > (step_threshold * 0.7f)) {
         // Falling edge detected - using 70% of the threshold for hysteresis
         step_detected = false;
     }
     
     // Store current value for next comparison
     acc_magnitude_prev = acc_magnitude_norm;
     
     return is_step;
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
         
         // For simulated device, increment step count by 1 every few samples
         static int sim_sample_count = 0;
         if (++sim_sample_count >= 10) {
             sim_sample_count = 0;
             step_count++;
             store_sensor_reading(MSG_TYPE_STEP_COUNT, (double)step_count);
         }
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
     
     /* Detect steps based on accelerometer data */
     if (detect_step((float)accel_x, (float)accel_y, (float)accel_z)) {
         // Store step count when a new step is detected
         store_sensor_reading(MSG_TYPE_STEP_COUNT, (double)(step_count + step_offset));
     }

     ++obs;
     LSM6DSO_VERBOSE("Observation: %u\n", obs);
 }
 
 static int set_sampling_freq(const struct device *dev)
 {
     int ret = 0;
     struct sensor_value odr_attr;

     /* set accel/gyro sampling frequency to 25 Hz for better step detection */
     odr_attr.val1 = 25;
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
                           const struct sensor_trigger *trig) __attribute__((unused));
                           
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
     
     /* Configure accelerometer for better sensitivity for step detection */
     struct sensor_value full_scale;
     full_scale.val1 = 4;  // 4g full scale
     full_scale.val2 = 0;
     
     if (sensor_attr_set(lsm6dso_dev, SENSOR_CHAN_ACCEL_XYZ, 
                        SENSOR_ATTR_FULL_SCALE, &full_scale) != 0) {
         LSM6DSO_ERROR("Failed to set accelerometer full scale\n");
         // Non-fatal, continue
     }
     
     /* Reset step counter */
     step_count = 0;
     step_offset = 0;
     
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

 /**
  * @brief Fetch current step count
  * 
  * @param steps Pointer to store step count value
  * @return int 0 on success, negative error code otherwise
  */
 int lsm6dso_get_step_count(uint32_t *steps)
 {
     if (!device_is_ready(lsm6dso_dev)) {
         return -ENODEV;
     }
     
     *steps = step_count + step_offset;
     return 0;
 }

 /**
  * @brief Reset step counter to zero
  * 
  * @return int 0 on success, negative error code otherwise
  */
 int lsm6dso_reset_step_count(void)
 {
     if (!device_is_ready(lsm6dso_dev)) {
         return -ENODEV;
     }
     
     /* Update offset to reset step count to zero */
     step_offset = -step_count;
     
     LSM6DSO_INFO("Step counter reset to zero\n");
     return 0;
 }

 // Variables pour la détection d'activité
static float activity_threshold = 10.0f; // Seuil au-dessus de la gravité (1g + marge)
static bool is_activity_detected = false;

// Fonction pour détecter l'activité à partir des données d'accélération
bool lsm6dso_detect_activity(void)
{
    struct sensor_value accel[3];
    float norm;
    
    // Lire les valeurs d'accélération du capteur
    if (lsm6dso_dev && device_is_ready(lsm6dso_dev)) {
        if (sensor_channel_get(lsm6dso_dev, SENSOR_CHAN_ACCEL_XYZ, accel) == 0) {
            // Calculer la norme de l'accélération
            float x = sensor_value_to_float(&accel[0]);
            float y = sensor_value_to_float(&accel[1]);
            float z = sensor_value_to_float(&accel[2]);
            
            norm = sqrtf(x*x + y*y + z*z);
            // info print norm
            // LSM6DSO_INFO("Norm: %.2f\n", norm);

            // Comparer à un seuil (1g [gravité] + marge)
            is_activity_detected = norm > activity_threshold;
            return is_activity_detected;
        }
    }
    
    return false;
}

// Fonction d'API pour obtenir l'état actuel de détection d'activité
bool lsm6dso_is_activity_detected(void)
{
    return is_activity_detected;
}