#include "IMU.h"

IMU::IMU() {
  kalmanX = KalmanFilter(0.001, 0.003, 0.03);
  kalmanY = KalmanFilter(0.001, 0.003, 0.03);
  
  kalPitch = 0;
  kalRoll = 0;
  
  accPitch = 0;
  accRoll = 0;
  
  yaw = 0;
  heading = 0;

  iAccX = 0;
  iAccY = 0;
  iAccZ = 0;

  offsetKalPitch = 0;
  offsetKkalRoll = 0;

  timer = 0;
}

IMU::~IMU(){}

void IMU::imuInit(){
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
  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  gyroscope.setThreshold(0);

  // Initialize Initialize HMC5883L
  while (!compass.begin())
  {
    delay(500);
  }
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

float IMU::tiltCompensate(Vector mag, float pitch, float roll)
{
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

void IMU::imuLoop(){  

  //if(millis() - timer > 10) {
    Vector acc = accelerometer.readNormalize();
    Vector gyr = gyroscope.readNormalize();
    Vector mag = compass.readNormalize();

    // Save gyro data
    gX = gyr.XAxis;
    gY = gyr.YAxis;
    gZ = gyr.ZAxis;
  
    // Calculate Pitch & Roll from accelerometer (deg)
    accPitch = -(atan2(acc.XAxis, sqrt(acc.YAxis*acc.YAxis + acc.ZAxis*acc.ZAxis))*180.0)/M_PI;
    accRoll  = (atan2(acc.YAxis, acc.ZAxis)*180.0)/M_PI;
    
    // Kalman filter
    kalPitch = kalmanY.update(accPitch, gyr.YAxis) - offsetKalPitch;
    kalRoll = kalmanX.update(accRoll, gyr.XAxis) - offsetKkalRoll;
  
    yaw = yaw + gyr.ZAxis*0.01;
    if(yaw > 999) yaw = 0;
    // Calculate heading
    float heading = atan2(mag.YAxis, mag.XAxis);
    // Set declination angle on your location and fix heading
    // You can find your declination on: http://magnetic-declination.com/
    // (+) Positive or (-) for negative
    // For Bytom / Poland declination angle is 4'26E (positive)
    // Formula: (deg + (min / 60.0)) / (180 / M_PI);
    float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
    heading += declinationAngle;
    // Correct for heading < 0deg and heading > 360deg
    if (heading < 0) heading += 2 * PI;
    if (heading > 2 * PI)  heading -= 2 * PI;
    heading = heading * 180/M_PI;
  
    int ikalPitch = round(kalPitch);
    int ikalRoll = round(kalRoll);
    int iyaw = round(yaw);
  
    // Gravity delete
    // /*----------*/ 
    
    // Formatting Acc data
    iAccX = round(acc.XAxis); if(iAccX > 99) iAccX=99;  if(iAccX < -99) iAccX=-99;
    iAccY = round(acc.YAxis); if(iAccY > 99) iAccY=99;  if(iAccY < -99) iAccY=-99;
    iAccZ = round(acc.ZAxis); if(iAccZ > 99) iAccZ=99;  if(iAccZ < -99) iAccZ=-99;
    
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
    if(ikalPitch<0)str+="-";else str+="0";
    if(ikalPitch<100 && ikalPitch>-100)str+="0";
    if(ikalPitch<10  && ikalPitch>-10)str+="0";
    str+=abs(ikalPitch);
  
    str += "E";
    if(ikalRoll<0)str+="-";else str+="0";
    if(ikalRoll<100 && ikalRoll>-100)str+="0";
    if(ikalRoll<10  && ikalRoll>-10)str+="0";
    str+=abs(ikalRoll);
  
    str += "F";
    if(iyaw<0)str+="-";else str+="0";
    if(iyaw<100 && iyaw>-100)str+="0";
    if(iyaw<10  && iyaw>-10)str+="0";
    str+=abs(iyaw);
  
    //timer = millis();}
}

void IMU::imuCalib(){
  yaw = 0;
  offsetKalPitch = kalPitch + offsetKalPitch;
  offsetKkalRoll = kalRoll + offsetKkalRoll;
}

