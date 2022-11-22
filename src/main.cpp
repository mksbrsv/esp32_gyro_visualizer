#include <Arduino.h>
#include "IMPU.h"
#include "config.h"
#include "EEPROM.h"

// vars
config cfg;
IMPU& mpu = cfg.get_mpu();
BluetoothSerial bt = cfg.get_bt();
std::array<uint8_t, 12> ypr;

void on_data_cb(const uint8_t* msg, size_t size) {
    if (bth::check_request(msg, size)) {
        bt.println(bth::device_id);
    } else {
        bth::ready_to_send = true;
    }
}

void resize(float yaw, float pitch, float roll) {
    uint8_t yaw_temp[4] = {};
    uint8_t pitch_temp[4] = {};
    uint8_t roll_temp[4] = {};
    memcpy(yaw_temp, &yaw, 4);
    memcpy(pitch_temp, &pitch, 4);
    memcpy(roll_temp, &roll, 4);
    for(int i = 0; i < 12; i++) {
        if (i < 4) {
            ypr[i] = yaw_temp[i];
        }
        if (i >= 4 && i < 8) {
            ypr[i] = pitch_temp[i-4];
        }
        if (i >= 8) {
            ypr[i] = roll_temp[i-8];
        }
    }
}


void setup() {
    bth::error_handler();
    EEPROM.begin(20);
    Serial.begin(115200);
    
    mpu.calibrate();
    mpu.init();
    bt.begin(bth::device_name);
    Serial.println("Bluetooth is enabled, pair it!");
}


void loop() {
    bt.register_callback(bth::bt_callback);
    if (bt.available()) {
        bt.onData(on_data_cb);
    }
    bt.onData(on_data_cb);
    float y, p, r;
    mpu.get_ypr(y, p, r);
    resize(y, p, r);
    if (bth::ready_to_send) {
        bt.write(ypr.data(), ypr.size());
    }
    // Serial.print(y);
    // Serial.print("\t");
    // Serial.print(p);
    // Serial.print("\t");
    // Serial.println(r);
    delay(100);
}