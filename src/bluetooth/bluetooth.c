/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

 #include <zephyr/types.h>
 #include <stddef.h>
 #include <string.h>
 #include <errno.h>
 #include <zephyr/sys/printk.h>
 #include <zephyr/sys/byteorder.h>
 #include <zephyr/kernel.h>
 
 #include <zephyr/settings/settings.h>
 
 #include <zephyr/bluetooth/bluetooth.h>
 #include <zephyr/bluetooth/hci.h>
 #include <zephyr/bluetooth/conn.h>
 #include <zephyr/bluetooth/uuid.h>
 #include <zephyr/bluetooth/gatt.h>
 #include <zephyr/bluetooth/services/bas.h>
 #include <zephyr/bluetooth/services/cts.h>
 #include <zephyr/bluetooth/services/hrs.h>
 #include <zephyr/bluetooth/services/ias.h>
 
 /* Custom Service Variables */
 #define BT_UUID_CUSTOM_SERVICE_VAL \
     BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)
 
 static const struct bt_uuid_128 vnd_uuid = BT_UUID_INIT_128(
     BT_UUID_CUSTOM_SERVICE_VAL);
 
 // Keep just one characteristic for read/write
 static const struct bt_uuid_128 vnd_rw_uuid = BT_UUID_INIT_128(
     BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1));
 
 #define VND_MAX_LEN 20
 
 // Device information to be exposed via the vendor service
 static uint8_t vnd_value[VND_MAX_LEN + 1] = { 'D', 'e', 'v', 'i', 'c', 'e', 'I', 'n', 'f', 'o' };
 
 static ssize_t read_vnd(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             void *buf, uint16_t len, uint16_t offset)
 {
     const char *value = attr->user_data;
 
     return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
                  strlen(value));
 }
 
 static ssize_t write_vnd(struct bt_conn *conn, const struct bt_gatt_attr *attr,
              const void *buf, uint16_t len, uint16_t offset,
              uint8_t flags)
 {
     uint8_t *value = attr->user_data;
 
     if (offset + len > VND_MAX_LEN) {
         return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
     }
 
     memcpy(value + offset, buf, len);
     value[offset + len] = 0;
 
     printk("Received data: %s\n", value);
     return len;
 }
 
 /* Simplified Vendor Primary Service Declaration - read/write only */
 BT_GATT_SERVICE_DEFINE(vnd_svc,
     BT_GATT_PRIMARY_SERVICE(&vnd_uuid),
     BT_GATT_CHARACTERISTIC(&vnd_rw_uuid.uuid,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                    read_vnd, write_vnd, vnd_value)
 );
 
 static const struct bt_data ad[] = {
     BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
     BT_DATA_BYTES(BT_DATA_UUID16_ALL,
               BT_UUID_16_ENCODE(BT_UUID_HRS_VAL),
               BT_UUID_16_ENCODE(BT_UUID_BAS_VAL)),
     BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_CUSTOM_SERVICE_VAL),
 };
 
 static const struct bt_data sd[] = {
     BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
 };
 
 void mtu_updated(struct bt_conn *conn, uint16_t tx, uint16_t rx)
 {
     printk("Updated MTU: TX: %d RX: %d bytes\n", tx, rx);
 }
 
 static struct bt_gatt_cb gatt_callbacks = {
     .att_mtu_updated = mtu_updated
 };
 
 static void connected(struct bt_conn *conn, uint8_t err)
 {
     if (err) {
         printk("Connection failed, err 0x%02x %s\n", err, bt_hci_err_to_str(err));
     } else {
         printk("Connected\n");
     }
 }
 
 static void disconnected(struct bt_conn *conn, uint8_t reason)
 {
     printk("Disconnected, reason 0x%02x %s\n", reason, bt_hci_err_to_str(reason));
 }
 
 static void alert_stop(void)
 {
     printk("Alert stopped\n");
 }
 
 static void alert_start(void)
 {
     printk("Mild alert started\n");
 }
 
 static void alert_high_start(void)
 {
     printk("High alert started\n");
 }
 
 BT_CONN_CB_DEFINE(conn_callbacks) = {
     .connected = connected,
     .disconnected = disconnected,
 };
 
 BT_IAS_CB_DEFINE(ias_callbacks) = {
     .no_alert = alert_stop,
     .mild_alert = alert_start,
     .high_alert = alert_high_start,
 };
 
 static void bt_ready(void)
 {
     int err;
 
     printk("Bluetooth initialized\n");
 
     if (IS_ENABLED(CONFIG_SETTINGS)) {
         settings_load();
     }
 
     err = bt_le_adv_start(BT_LE_ADV_CONN_ONE_TIME, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
     if (err) {
         printk("Advertising failed to start (err %d)\n", err);
         return;
     }
 
     printk("Advertising successfully started\n");
 }
 
 static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
 {
     char addr[BT_ADDR_LE_STR_LEN];
 
     bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
 
     printk("Passkey for %s: %06u\n", addr, passkey);
 }
 
 static void auth_cancel(struct bt_conn *conn)
 {
     char addr[BT_ADDR_LE_STR_LEN];
 
     bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
 
     printk("Pairing cancelled: %s\n", addr);
 }
 
 static struct bt_conn_auth_cb auth_cb_display = {
     .passkey_display = auth_passkey_display,
     .passkey_entry = NULL,
     .cancel = auth_cancel,
 };
 
 
 /**
  * variable to hold reference milliseconds to epoch when device booted
  * this is only for demo purpose, for more precise synchronization please
  * review clock_settime API implementation.
  */
 static int64_t unix_ms_ref;
 static bool cts_notification_enabled;
 
 void bt_cts_notification_changed(bool enabled)
 {
     cts_notification_enabled = enabled;
 }
 
 int bt_cts_cts_time_write(struct bt_cts_time_format *cts_time)
 {
     int err;
     int64_t unix_ms;
 
     if (IS_ENABLED(CONFIG_BT_CTS_HELPER_API)) {
         err = bt_cts_time_to_unix_ms(cts_time, &unix_ms);
         if (err) {
             return err;
         }
     } else {
         return -ENOTSUP;
     }
 
     /* recalculate reference value */
     unix_ms_ref = unix_ms - k_uptime_get();
     return 0;
 }
 
 int bt_cts_fill_current_cts_time(struct bt_cts_time_format *cts_time)
 {
     int64_t unix_ms = unix_ms_ref + k_uptime_get();
 
     if (IS_ENABLED(CONFIG_BT_CTS_HELPER_API)) {
         return bt_cts_time_from_unix_ms(cts_time, unix_ms);
     } else {
         return -ENOTSUP;
     }
 }
 
 const struct bt_cts_cb cts_cb = {
     .notification_changed = bt_cts_notification_changed,
     .cts_time_write = bt_cts_cts_time_write,
     .fill_current_cts_time = bt_cts_fill_current_cts_time,
 };

 int launchBLE(void) {
        int err;
    
        err = bt_enable(NULL);
        if (err) {
            printk("Bluetooth init failed (err %d)\n", err);
            return 0;
        }
    
        bt_ready();
    
        bt_gatt_cb_register(&gatt_callbacks);
        bt_conn_auth_cb_register(&auth_cb_display);
    
        /* Implement notification. At the moment there is no suitable way
        * of starting delayed work so we do it here
        */
        while (1) {
            k_sleep(K_SECONDS(1));
    
            /* No vendor indication simulation needed - using simple read/write */
        }
        return 0;
 }