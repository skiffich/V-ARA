#ifndef AccGyro_h
#define AccGyro_h

#include <Arduino.h>

#include <Wire.h>
#include <L3G4200D.h>
#include <ADXL345.h>
#include <KalmanFilter.h>

class AccGyro {
public:
  AccGyro();
  ~AccGyro();
  String str;
  void accGyroLoop();
  void accGyroInit();
  
private:
  L3G4200D gyroscope;
  ADXL345 accelerometer;
  
  KalmanFilter kalmanX;
  KalmanFilter kalmanY;

  float accPitch;
  float accRoll;
  
  float kalPitch;
  float kalRoll;
  
  // Timers
  unsigned long timer;
  float timeStep;
  
  float yaw;
};

#endif
