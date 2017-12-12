#include "IMU.h"
#include "Tensor.h"

IMU imu;

Tensor tensor1(0, "G");
Tensor tensor2(1, "H");
Tensor tensor3(2, "I");
Tensor tensor4(3, "J");

String resString = "";

void setup() {
  imu.imuInit();

  Serial.begin(9600);
}

void loop() {
  imu.imuLoop();
  resString =  imu.str;        // Read AHRS string
  resString += tensor1.tensorRead(); // Read tensor1
  resString += tensor2.tensorRead(); // Read tensor2
  resString += tensor3.tensorRead(); // Read tensor3
  resString += tensor4.tensorRead(); // Read tensor4
  resString += "K";
  
  Serial.println(resString);
}
