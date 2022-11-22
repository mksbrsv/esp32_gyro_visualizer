#ifndef IEEPROM_MPU_H
#define IEEPROM_MPU_H

class IeepromMPU {
  public:
    virtual int get_xg_offset() = 0;
    virtual int get_yg_offset() = 0;
    virtual int get_zg_offset() = 0;

    virtual int get_xa_offset() = 0;
    virtual int get_ya_offset() = 0;
    virtual int get_za_offset() = 0;

    virtual void set_xg_offset(int os) = 0;
    virtual void set_yg_offset(int os) = 0;
    virtual void set_zg_offset(int os) = 0;

    virtual void set_xa_offset(int os) = 0;
    virtual void set_ya_offset(int os) = 0;
    virtual void set_za_offset(int os) = 0;

    virtual bool get_calibration_flag() = 0; 
    virtual void set_calibration_flag() = 0; // true values
    virtual void reset_calibration_flag() = 0;  // false values
};


#endif // LEEPROM_MPU_H
