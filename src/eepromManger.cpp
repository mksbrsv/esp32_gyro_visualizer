#include "eepromManager.h"
#include "EEPROM.h"


int eepromManager::read_int_from_eeprom(int address) {
  int value;
  value = EEPROM.read(address);
  value |= EEPROM.read(address + 1) << 8;
  return value;
}
void eepromManager::write_int_to_eeprom(int address, int value) {
  EEPROM.write(address, value & 0xff);
  EEPROM.write(address + 1, value >> 8);
}

int eepromManager::get_xg_offset()  {
  return read_int_from_eeprom(x_gyro);
}

int eepromManager::get_yg_offset()  {
  return read_int_from_eeprom(y_gyro);
}

int eepromManager::get_zg_offset()  {
  return read_int_from_eeprom(z_gyro);
}

int eepromManager::get_xa_offset()  {
  return read_int_from_eeprom(x_accel);
}

int eepromManager::get_ya_offset()  {
  return read_int_from_eeprom(y_accel);
}

int eepromManager::get_za_offset()  {
  return read_int_from_eeprom(z_accel);
}

void eepromManager::set_xg_offset(int os)  {
  write_int_to_eeprom(x_gyro, os);
}

void eepromManager::set_yg_offset(int os)  {
  write_int_to_eeprom(y_gyro, os);
}

void eepromManager::set_zg_offset(int os)  {
  write_int_to_eeprom(z_gyro, os);
}

void eepromManager::set_xa_offset(int os)  {
  write_int_to_eeprom(x_accel, os);
}

void eepromManager::set_ya_offset(int os)  {
  write_int_to_eeprom(y_accel, os);
}

void eepromManager::set_za_offset(int os)  {
  write_int_to_eeprom(z_accel, os);
}

bool eepromManager::get_calibration_flag()  {
  return (read_int_from_eeprom(calibration_flag_addr) == calibration_flag_true);
}

void eepromManager::set_calibration_flag()  {
  write_int_to_eeprom(calibration_flag_addr, calibration_flag_true);
}
void eepromManager::reset_calibration_flag()  {
  write_int_to_eeprom(calibration_flag_addr, calibration_flag_false);
}