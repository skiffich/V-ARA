#ifndef IMU_h
#define IMU_h

#include <Arduino.h>

#include <Wire.h>
#include <L3G4200D.h>
#include <ADXL345.h>
#include <HMC5883L.h>
#include <KalmanFilter.h>
#include <MadgwickAHRS.h>

class IMU {
public:
  IMU();
  ~IMU();
  String str;
  void imuLoop();
  void imuInit();
  void imuCalib();
  
private:
  L3G4200D gyroscope;
  ADXL345 accelerometer;
  HMC5883L compass;

  Madgwick filter;

  float convertRawGyro(int gRaw);
  float convertRawAcceleration(int aRaw);

  float froll;
  float fpitch;
  float fyaw;
  float fAccX;
  float fAccY;
  float fAccZ;
  
  int iAccX;
  int iAccY;
  int iAccZ;
  int iPitch;
  int iRoll;
  int iYaw;
  int iYawPrev;

  int offsetPitch;
  int offsetRoll;

  int offAccX;
  int offAccY;
  int offAccZ;
};

#endif
