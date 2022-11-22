#ifndef WMPU_H
#define WMPU_H
#include "IMPU.h"
#include "IeepromMPU.h"
class WMPU : public IMPU {
  public:
    WMPU(IeepromMPU& ieepromMPU);
    virtual void init() override;
    virtual void get_ypr(float& y, float& p, float& r) override;
    virtual void calibrate() override;
    virtual bool is_calibrated() override;
    virtual void reset_calibration() override;

  private: 
    IeepromMPU& eepromMng;

    void guess_offsets();
    void load_calibration();
    int begin();
};

#endif // WMPU_H