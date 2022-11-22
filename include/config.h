#ifndef CONFIG_H
#define CONFIG_H
#include "IMPU.h"
#include "WMPU.h"
#include "eepromManager.h"
#include <BluetoothSerial.h>

namespace bth {
  constexpr char* device_id = "CustomController";
  constexpr char* device_name = "esp32";
  bool ready_to_send = false;

  void bt_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t* param) {
    if (event == ESP_SPP_SRV_OPEN_EVT) {
      Serial.println("Client connected");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (event == ESP_SPP_CLOSE_EVT) {
      Serial.println("Client disconnected");
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  void error_handler() {
    #if !defined (CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
    #error Bluetooth is not enabled!
    #endif
  }
  bool check_request(const uint8_t* msg, size_t size) {
    if ((char)msg[0] == 'g' && (char)msg[1] == 'e' && (char)msg[2] == 't')  {
      return true;
    }
    return false;
  }
}

class config {
  public:
  IMPU& get_mpu() { return mpu;}
  BluetoothSerial& get_bt() {return bt;}
  private:
  eepromManager eepromMng;
  WMPU mpu = WMPU(eepromMng);
  BluetoothSerial bt;
};

#endif // CONFIG_H