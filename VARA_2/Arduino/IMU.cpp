#include "IMU.h"

IMU::IMU() {
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
  heading = 0;
  iKalPitch = 0;
  iKalRoll = 0;
  iMagYaw = 0;
}

IMU::~IMU(){}

void IMU::imuInit(){
  // Initialize ADXL345
  while(!accelerometer.begin()) delay(500);
 
  accelerometer.setRange(ADXL345_RANGE_2G);
  
  // Initialize L3G4200D
  while(!gyroscope.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50)) delay(500);

  // Initialize Initialize HMC5883L
  while (!compass.begin()) delay(500);

  // Calibrate gyroscope
  gyroscope.calibrate(100);

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0); 
}


void IMU::imuLoop(){
  timer = millis();
  
  Vector acc = accelerometer.readNormalize();
  Vector gyr = gyroscope.readNormalize();
  Vector mag = compass.readNormalize();

  // Calculate Pitch & Roll from accelerometer (deg)
  accPitch = -(atan2(acc.XAxis, sqrt(acc.YAxis*acc.YAxis + acc.ZAxis*acc.ZAxis))*180.0)/M_PI;
  accRoll  = (atan2(acc.YAxis, acc.ZAxis)*180.0)/M_PI;

  // Kalman filter
  kalPitch = kalmanY.update(accPitch, gyr.YAxis);
  kalRoll = kalmanX.update(accRoll, gyr.XAxis);

  // Read yaw from gyro
  yaw = yaw + gyr.ZAxis * timeStep;
  if(yaw > 360 || yaw < -360) yaw=.0;

  // Read magn data
  heading = tiltCompensate(mag, kalPitch, kalRoll);
  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;
  // Correct for heading < 0deg and heading > 360deg
  heading = correctAngle(heading);
  // Convert to degrees
  heading = heading * 180/M_PI;

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


  // Create string
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

float IMU::tiltCompensate(Vector mag, Vector normAccel){
  // Pitch & Roll 

  float roll;
  float pitch;

  roll = asin(normAccel.YAxis);
  pitch = asin(-normAccel.XAxis);

  if (roll > 0.78 || roll < -0.78 || pitch > 0.78 || pitch < -0.78)
  {
    return -1000;
  }

  // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
  float cosRoll = cos(roll);
  float sinRoll = sin(roll);  
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);

  // Tilt compensation
  float Xh = mag.XAxis * cosPitch + mag.ZAxis * sinPitch;
  float Yh = mag.XAxis * sinRoll * sinPitch + mag.YAxis * cosRoll - mag.ZAxis * sinRoll * cosPitch;

  float heading = atan2(Yh, Xh);

  return heading;
}

float IMU::tiltCompensate(Vector mag, float _pitch, float _roll){
  // Pitch & Roll 

  float roll;
  float pitch;

  roll = asin(_roll);
  pitch = asin(-_pitch);

  if (roll > 0.78 || roll < -0.78 || pitch > 0.78 || pitch < -0.78)
  {
    return -1000;
  }

  // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
  float cosRoll = cos(roll);
  float sinRoll = sin(roll);  
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);

  // Tilt compensation
  float Xh = mag.XAxis * cosPitch + mag.ZAxis * sinPitch;
  float Yh = mag.XAxis * sinRoll * sinPitch + mag.YAxis * cosRoll - mag.ZAxis * sinRoll * cosPitch;

  float heading = atan2(Yh, Xh);

  return heading;
}

float IMU::correctAngle(float _heading){
  if (_heading < 0) { _heading += 2 * PI; }
  if (_heading > 2 * PI) { _heading -= 2 * PI; }

  return _heading;
}
