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
  iKalPitch = 0;
  iKalRoll = 0;
  iMagYaw = 0;
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

  // Formatting Gyro data
  iKalPitch = int(kalPitch);  if(iKalPitch > 180) iKalPitch=180;  if(iKalPitch < -180) iKalPitch=-180;
  iKalRoll  = int(kalRoll);   if(iKalRoll > 180) iKalRoll=180;    if(iKalRoll < -9180) iKalRoll=-180;
  iMagYaw   = int(yaw);       //if(iMagYaw<0) iMagYaw %= -360;    if(iMagYaw>0) iMagYaw %= 360;

  // Formatting Acc data
  iAccX = int(acc.XAxis); if(iAccX > 99) iAccX=99;  if(iAccX < -99) iAccX=-99;
  iAccY = int(acc.YAxis); if(iAccY > 99) iAccY=99;  if(iAccY < -99) iAccY=-99;
  iAccZ = int(acc.ZAxis); if(iAccZ > 99) iAccZ=99;  if(iAccZ < -99) iAccZ=-99;
  // Gravity delete
  //if(iAccX<=10 && iAccX>=-10)iAccX=0; if(iAccY<=10 && iAccY>=-10)iAccY=0; if(iAccZ<=10 && iAccZ>=-10)iAccZ=0;

  str = "A";
  if(iAccX<0)str+="-";else str+="0";
  if(iAccX<10 && iAccX>-10)str+="0";
  str+=abs(iAccX);

  str += "B";
  if(iAccY<0)str+="-";else str+="0";
  if(iAccY<10 && iAccY>-10)str+="0";
  str+=abs(iAccY);

  str += "C";
  if(iAccZ<0)str+="-";else str+="0";
  if(iAccZ<10 && iAccZ>-10)str+="0";
  str+=abs(iAccZ);

  str += "D";
  if(iKalPitch<0)str+="-";else str+="0";
  if(iKalPitch<100 && iKalPitch>-100)str+="0";
  if(iKalPitch<10  && iKalPitch>-10)str+="0";
  str+=abs(iKalPitch);

  str += "E";
  if(iKalRoll<0)str+="-";else str+="0";
  if(iKalRoll<100 && iKalRoll>-100)str+="0";
  if(iKalRoll<10  && iKalRoll>-10)str+="0";
  str+=abs(iKalRoll);

  str += "F";
  if(iMagYaw<0)str+="-";else str+="0";
  if(iMagYaw<100 && iMagYaw>-100)str+="0";
  if(iMagYaw<10  && iMagYaw>-10)str+="0";
  str+=abs(iMagYaw);
}
