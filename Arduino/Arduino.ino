#include "IMU.h"

IMU imu;

void setup() {
  // put your setup code here, to run once:
  imu.imuInit();

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  imu.imuLoop();
  Serial.println(imu.str);
}
