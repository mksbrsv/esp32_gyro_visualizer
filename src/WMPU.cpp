#include "WMPU.h"
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

WMPU::WMPU(IeepromMPU& ieepromMPU) : eepromMng(ieepromMPU) {}

namespace ns_mpu {
  MPU6050 mpu;
}

int WMPU::begin() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    ns_mpu::mpu.initialize();
    ns_mpu::mpu.testConnection();
    uint8_t dev_status = ns_mpu::mpu.dmpInitialize();
    guess_offsets();
    return dev_status;
}

void WMPU::init() {

  uint8_t dev_status = begin();
  if (dev_status == 0) {
    if (is_calibrated()) {
      load_calibration();
      ns_mpu::mpu.setDMPEnabled(true);
    } else {
      Serial.println("MPU is not calibrated. Halting execution");
      while(true) {
        //failure
      }

    }
  } else {
    Serial.println("Bad devstatus. Halting execution");
    while(true) {
      //failure
    }
  }

}

void WMPU::get_ypr(float& y, float& p, float& r) {
  uint8_t fifoBuffer[64];
  Quaternion q;
  if (ns_mpu::mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet 
    ns_mpu::mpu.dmpGetQuaternion(&q, fifoBuffer);

    float q0 = q.w;
    float q1 = q.x;
    float q2 = q.y;
    float q3 = q.z;

    float yr = -atan2(-2 * q1 * q2 + 2 * q0 * q3, q2 * q2 - q3 * q3 - q1 * q1 + q0 * q0);
    float pr = asin(2 * q2 * q3 + 2 * q0 * q1);
    float rr = atan2(-2 * q1 * q3 + 2 * q0 * q2, q3 * q3 - q2 * q2 - q1 * q1 + q0 * q0);

    // y = yr;
    // p = pr;
    // r = rr;

    y = yr * 180 / M_PI;
    p = pr * 180 / M_PI;
    r = rr * 180 / M_PI;
  }
}

void WMPU::guess_offsets() {
  // supply your own gyro offsets here, scaled for min sensitivity
  ns_mpu::mpu.setXGyroOffset(220);
  ns_mpu::mpu.setYGyroOffset(76);
  ns_mpu::mpu.setZGyroOffset(-85);
  ns_mpu::mpu.setZAccelOffset(1788); 
}

void WMPU::calibrate() {
  Serial.println("Assuming module is flat and still!");
  uint8_t dev_status = begin();
  if (dev_status == 0) {
    ns_mpu::mpu.CalibrateAccel(6);
    ns_mpu::mpu.CalibrateGyro(6);

    eepromMng.set_xg_offset(ns_mpu::mpu.getXGyroOffset());
    eepromMng.set_yg_offset(ns_mpu::mpu.getYGyroOffset());
    eepromMng.set_zg_offset(ns_mpu::mpu.getZGyroOffset());

    eepromMng.set_xa_offset(ns_mpu::mpu.getXAccelOffset());
    eepromMng.set_ya_offset(ns_mpu::mpu.getYAccelOffset());
    eepromMng.set_za_offset(ns_mpu::mpu.getZAccelOffset());

    eepromMng.set_calibration_flag();
    Serial.println("Calibration complete");
  } else {
    Serial.println("Bad devstatus. Halting execution");
    while(true) {
      //failure
    }
  }
}
bool WMPU::is_calibrated() {
  return eepromMng.get_calibration_flag();
}
void WMPU::reset_calibration() {
  eepromMng.reset_calibration_flag();
  Serial.println("Calibration flag reset");
}


void WMPU::load_calibration() {
  ns_mpu::mpu.setXGyroOffset(eepromMng.get_xg_offset());
  ns_mpu::mpu.setYGyroOffset(eepromMng.get_yg_offset());
  ns_mpu::mpu.setZGyroOffset(eepromMng.get_zg_offset());

  ns_mpu::mpu.setXAccelOffset(eepromMng.get_xa_offset());
  ns_mpu::mpu.setYAccelOffset(eepromMng.get_ya_offset());
  ns_mpu::mpu.setZAccelOffset(eepromMng.get_za_offset());
}