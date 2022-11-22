#ifndef IMPU_H
#define IMPU_H

class IMPU {
  public:
    virtual void init() = 0;
    virtual void get_ypr(float& y, float& p, float& r) = 0;
    virtual void calibrate() = 0;
    virtual bool is_calibrated() = 0;
    virtual void reset_calibration() = 0;
};


#endif // IMPU_H
