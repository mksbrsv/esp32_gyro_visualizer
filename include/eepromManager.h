#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include "IeepromMPU.h"

class eepromManager : public IeepromMPU {
  enum offset_addr {
    x_gyro = 2,
    y_gyro = 4,
    z_gyro = 6,
    x_accel = 8,
    y_accel = 10,
    z_accel = 12
  };

  const int calibration_flag_addr = 0;
  const int calibration_flag_true = 77;
  const int calibration_flag_false = 0;

  int read_int_from_eeprom(int address);
  void write_int_to_eeprom(int address, int value);

  virtual int get_xg_offset() override;
  virtual int get_yg_offset() override;
  virtual int get_zg_offset() override;

  virtual int get_xa_offset() override;
  virtual int get_ya_offset() override;
  virtual int get_za_offset() override;

  virtual void set_xg_offset(int os) override;
  virtual void set_yg_offset(int os) override;
  virtual void set_zg_offset(int os) override;

  virtual void set_xa_offset(int os) override;
  virtual void set_ya_offset(int os) override;
  virtual void set_za_offset(int os) override;

  virtual bool get_calibration_flag() override; 
  virtual void set_calibration_flag() override;
  virtual void reset_calibration_flag() override;

};

#endif // EEPROM_MANAGER_H