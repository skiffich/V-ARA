#include "IMU.h"

IMU::IMU() {
  froll = 1.0;
  fpitch = 1.0;
  fyaw = 1.0;
  fAccX = 1.0;
  fAccY = 1.0;
  fAccZ = 1.0;
  
  iAccX = 1;
  iAccY = 1;
  iAccZ = 1;
  iPitch = 1;
  iRoll = 1;
  iYaw = 1;
  iYawPrev = 1;

  offsetPitch = 0;
  offsetRoll = 0;

  offAccX = 0;
  offAccY = 0;
  offAccZ = 0;
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

  filter.begin(1);
}

float IMU::convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float IMU::convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}

void IMU::imuLoop(){  
  Vector acc = accelerometer.readNormalize();
  Vector gyr = gyroscope.readNormalize();
  Vector mag = compass.readNormalize();

  float ax = convertRawAcceleration(acc.XAxis);
  float ay = convertRawAcceleration(acc.YAxis);
  float az = convertRawAcceleration(acc.ZAxis);
  float gx = convertRawGyro(gyr.XAxis);
  float gy = convertRawGyro(gyr.YAxis);
  float gz = convertRawGyro(gyr.ZAxis);
  
  filter.updateIMU(gx, gy, gz, ax, ay, az);

  // Low frequency filters
  // if K less - then greater the compensation
  float K = 0.1; 
  // Roll LFF
  float roll = filter.getRoll();
  froll = (1-K)*froll + K*roll;
  // Pitch LFF
  float pitch = filter.getPitch();
  fpitch = (1-K)*fpitch + K*pitch;
  // Yaw LFF
  float yaw = filter.getYaw();
  fyaw = (1-K)*fyaw + K*yaw;

  // Formatting Gyro data
  iPitch = round(fpitch); if(iPitch > 180) iPitch=180;  if(iPitch < -180) iPitch=-180; iPitch -= offsetPitch; 
  iRoll  = round(froll); if(iRoll > 180) iRoll=180;    if(iRoll < -9180) iRoll=-180; iRoll -= offsetRoll;
  int  iYawCur   = round(fyaw);  //if(iYaw<0) iYaw %= -360;    if(iYaw>0) iYaw %= 360;
  iYaw += iYawCur - iYawPrev;
  if(iYaw < -999 || iYaw > 999)iYaw = 0;

  // Gravity delete
  // /*----------*/ 
  
  // Formatting Acc data
  iAccX = round(acc.XAxis) - offAccX; if(iAccX > 99) iAccX=99;  if(iAccX < -99) iAccX=-99;
  iAccY = round(acc.YAxis) - offAccY; if(iAccY > 99) iAccY=99;  if(iAccY < -99) iAccY=-99;
  iAccZ = round(acc.ZAxis) - offAccZ; if(iAccZ > 99) iAccZ=99;  if(iAccZ < -99) iAccZ=-99;
  
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
  if(iPitch<0)str+="-";else str+="0";
  if(iPitch<100 && iPitch>-100)str+="0";
  if(iPitch<10  && iPitch>-10)str+="0";
  str+=abs(iPitch);

  str += "E";
  if(iRoll<0)str+="-";else str+="0";
  if(iRoll<100 && iRoll>-100)str+="0";
  if(iRoll<10  && iRoll>-10)str+="0";
  str+=abs(iRoll);

  str += "F";
  if(iYaw<0)str+="-";else str+="0";
  if(iYaw<100 && iYaw>-100)str+="0";
  if(iYaw<10  && iYaw>-10)str+="0";
  str+=abs(iYaw);

  iYawPrev = iYawCur;
}

void IMU::imuCalib(){
  iYaw = 0;
  offsetPitch = iPitch + offsetPitch;
  offsetRoll = iRoll + offsetRoll;
  /*
  offAccX = iAccX + offAccX;
  offAccY = iAccY + offAccY;
  offAccZ = iAccZ + offAccZ;
  */
}

