#include "AccGyro.h"

AccGyro::AccGyro() {
  kalmanX = KalmanFilter(0.001, 0.003, 0.03);
  kalmanY = KalmanFilter(0.001, 0.003, 0.03);

  accPitch = 0;
  accRoll = 0;
  kalPitch = 0;
  kalRoll = 0;
  timer = 0;
  timeStep = 0.01;
  yaw = 0;
}

AccGyro::~AccGyro(){}

void AccGyro::accGyroInit(){
  Serial.begin(115200);
  
  // Initialize ADXL345
  while(!accelerometer.begin())
  {
    delay(500);
  }
 
  accelerometer.setRange(ADXL345_RANGE_2G);
  
  // Initialize L3G4200D
  while(!gyroscope.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50))
  {
    delay(500);
  }
 
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  gyroscope.calibrate(100);
  
  str = "A000B000C000D0000E0000F0000G00H00I00J00K";
}


void AccGyro::accGyroLoop(){
  timer = millis();
  
  Vector acc = accelerometer.readNormalize();
  Vector gyr = gyroscope.readNormalize();

  // Calculate Pitch & Roll from accelerometer (deg)
  accPitch = -(atan2(acc.XAxis, sqrt(acc.YAxis*acc.YAxis + acc.ZAxis*acc.ZAxis))*180.0)/M_PI;
  accRoll  = (atan2(acc.YAxis, acc.ZAxis)*180.0)/M_PI;

  // Kalman filter
  kalPitch = kalmanY.update(accPitch, gyr.YAxis);
  kalRoll = kalmanX.update(accRoll, gyr.XAxis);

  yaw = yaw + gyr.ZAxis * timeStep;

  Serial.print(" Pitch = ");
  Serial.print(int(kalPitch));
  Serial.print("\t Roll = ");
  Serial.print(int(kalRoll));
  Serial.print("\t Yaw = ");
  Serial.println(int(yaw));
  
  Serial.println();
}
