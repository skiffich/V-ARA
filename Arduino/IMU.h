#ifndef IMU_h
#define IMU_h

#include <Arduino.h>

#include <Wire.h>
#include <L3G4200D.h>
#include <ADXL345.h>
#include <HMC5883L.h>
#include <KalmanFilter.h>

class IMU {
public:
  IMU();
  ~IMU();
  String str;
  void imuLoop();
  void imuInit();
  void imuCalib();

  double gX, gY, gZ;
  
private:
  L3G4200D gyroscope;
  ADXL345 accelerometer;
  HMC5883L compass;

  float tiltCompensate(Vector mag, float pitch, float roll);

  KalmanFilter kalmanX;
  KalmanFilter kalmanY;
  
  float kalPitch;
  float kalRoll;
  
  float accPitch;
  float accRoll;
  
  float yaw;
  float heading;

  float offsetKalPitch;
  float offsetKkalRoll;

  int iAccX;
  int iAccY;
  int iAccZ;

  unsigned long timer;
};

#endif
