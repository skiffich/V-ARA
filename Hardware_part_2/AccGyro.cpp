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
  iAccX = 0;
  iAccY = 0;
  iAccZ = 0;
}

AccGyro::~AccGyro(){}

void AccGyro::accGyroInit(){
  // Initialize ADXL345
  while(!accelerometer.begin()) delay(500);
 
  accelerometer.setRange(ADXL345_RANGE_2G);
  
  // Initialize L3G4200D
  while(!gyroscope.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50)) delay(500);

  // Calibrate gyroscope
  gyroscope.calibrate(100);
  
  str = "";
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

  int iKalPitch = int(kalPitch);  if(iKalPitch > 99) iKalPitch=99;if(iKalPitch < -99) iKalPitch=-99;
  int iKalRoll  = int(kalRoll);   if(iKalRoll > 99) iKalRoll=99;  if(iKalRoll < -99) iKalRoll=-99;
  int iKalYaw   = int(yaw);       if(iKalYaw > 99) iKalYaw=99;    if(iKalYaw < -99) iKalYaw=-99;

  str += "A";
  if(iAccX<0)str+="-";else str+="0"
  if(iAccX<100)str+="0";
  str+=iAccX;

  str += "B";
  if(iAccY<0)str+="-";else str+="0"
  if(iAccY<100)str+="0";
  str+=iAccY;

  str += "C";
  if(iAccZ<0)str+="-";else str+="0"
  if(iAccZ<100)str+="0";
  str+=iAccZ;

  str += "D";
  if(iKalPitch<0)str+="-";else str+="0"
  if(iKalPitch<10)str+="00";
  if(iKalPitch<100)str+="0";
  str+=iKalPitch;

  str += "E";
  if(iKalRoll<0)str+="-";else str+="0"
  if(iKalRoll<10)str+="00";
  if(iKalRoll<100)str+="0";
  str+=iKalRoll;

  str += "F";
  if(iKalPitch<0)str+="-";else str+="0"
  if(iKalYaw<10)str+="00";
  if(iKalYaw<100)str+="0";
  str+=iKalYaw;
}
