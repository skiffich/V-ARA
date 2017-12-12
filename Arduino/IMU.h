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
  
private:
  L3G4200D gyroscope;
  ADXL345 accelerometer;
  HMC5883L compass;
  
  KalmanFilter kalmanX;
  KalmanFilter kalmanY;

  // Tilt compensation
  float tiltCompensate(Vector mag, Vector normAccel);
  float tiltCompensate(Vector mag, float _pitch, float _roll);
  // Correct angle
  float correctAngle(float _heading);

  float accPitch;
  float accRoll;
  
  float kalPitch;
  float kalRoll;
  
  // Timers
  unsigned long timer;
  float timeStep;
  
  float yaw;

  int iAccX;
  int iAccY;
  int iAccZ;

  int iKalPitch;
  int iKalRoll;
  int iMagYaw;

  float heading;
};

#endif
