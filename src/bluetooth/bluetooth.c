/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <soc.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include <zephyr/settings/settings.h>

#include "../../inc/sensors.h"
#include "../../inc/queue.h"
#include "../../inc/debug.h"

#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)

/* External reference to the global sensor data structure in sensors.c */
extern sensor_data_t g_sensor_data;

static struct bt_conn *current_conn;
static bool connected_flag = false;
static bool notif_enabled = false;

#define MAX_TRANSMIT_SIZE 240
static char sensor_data_buffer[MAX_TRANSMIT_SIZE];

/* Define Custom Service UUIDs */
#define SENSOR_SERVICE_UUID_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)

#define SENSOR_CHAR_UUID_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)

static struct bt_uuid_128 sensor_service_uuid = BT_UUID_INIT_128(SENSOR_SERVICE_UUID_VAL);
static struct bt_uuid_128 sensor_char_uuid = BT_UUID_INIT_128(SENSOR_CHAR_UUID_VAL);

/* Bluetooth connection callbacks */
static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        printk("Connection failed, err 0x%02x %s\n", err, bt_hci_err_to_str(err));
        return;
    }

    current_conn = bt_conn_ref(conn);
    connected_flag = true;
    printk("Connected\n");
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected, reason 0x%02x %s\n", reason, bt_hci_err_to_str(reason));

    if (current_conn) {
        bt_conn_unref(current_conn);
        current_conn = NULL;
    }

    connected_flag = false;
    notif_enabled = false;
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
};

/* Format sensor data as a string for transmission */
static void format_sensor_data(void)
{
    snprintf(sensor_data_buffer, MAX_TRANSMIT_SIZE,
        "SENSOR_DATA{"
        "\"temp\":%.2f,"
        "\"humidity\":%.2f,"
        "\"accel\":{\"x\":%.2f,\"y\":%.2f,\"z\":%.2f},"
        "\"gyro\":{\"x\":%.2f,\"y\":%.2f,\"z\":%.2f},"
        "\"mag\":{\"x\":%.2f,\"y\":%.2f,\"z\":%.2f}"
        "}",
        g_sensor_data.temperature,
        g_sensor_data.humidity,
        g_sensor_data.accel_x, g_sensor_data.accel_y, g_sensor_data.accel_z,
        g_sensor_data.gyro_x, g_sensor_data.gyro_y, g_sensor_data.gyro_z,
        g_sensor_data.mag_x, g_sensor_data.mag_y, g_sensor_data.mag_z
    );
}

static ssize_t read_sensor_data(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                              void *buf, uint16_t len, uint16_t offset)
{
    format_sensor_data();
    const char *value = attr->user_data;
    return bt_gatt_attr_read(conn, attr, buf, len, offset, value, strlen(value));
}

static void sensor_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
    notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    printk("Notifications %s\n", notif_enabled ? "enabled" : "disabled");
}

/* Define the custom sensor service */
BT_GATT_SERVICE_DEFINE(sensor_svc,
    BT_GATT_PRIMARY_SERVICE(&sensor_service_uuid),
    BT_GATT_CHARACTERISTIC(&sensor_char_uuid.uuid,
                         BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                         BT_GATT_PERM_READ, read_sensor_data, NULL, sensor_data_buffer),
    BT_GATT_CCC(sensor_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

/* BLE advertisement data */
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    // BT_DATA_BYTES(BT_DATA_UUID128_ALL, SENSOR_SERVICE_UUID_VAL),
};

void bluetooth_run(void)
{
    int err;

    printk("Starting Custom Sensor Service\n");

    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    printk("Bluetooth initialized\n");

    if (IS_ENABLED(CONFIG_SETTINGS)) {
        settings_load();
    }
    
    printk("Custom Sensor service initialized\n");

    /* Start advertising */
    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
        return;
    }

    printk("Advertising successfully started\n");

    /* Main loop - periodically send sensor data */
    while (1) {
        if (connected_flag && notif_enabled) {
            /* Format sensor data as a human-readable/JSON string */
            format_sensor_data();
            
            size_t data_len = strlen(sensor_data_buffer);
            if (data_len > 0) {
                printk("Sending sensor data: %s\n", sensor_data_buffer);
                
                /* Send data via custom service notification */
                err = bt_gatt_notify(NULL, &sensor_svc.attrs[1], 
                                    sensor_data_buffer, data_len);
                if (err) {
                    printk("Failed to send notification (err %d)\n", err);
                }
            }
        }
        
        /* Sleep to avoid busy-waiting and preserve battery */
        k_sleep(K_MSEC(500));
    }
}
