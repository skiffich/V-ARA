#include "Tensor.h"
#include "IMU.h"

Tensor tensor1(0, "G");
Tensor tensor2(1, "H");
Tensor tensor3(2, "I");
Tensor tensor4(3, "J");

IMU imu;
Acc acc;

boolean sending = false;

void setup()
{
  Serial.begin(57600);
}

void loop() {

  String string = acc.readAcc(); // return something like this A099B001C-15 as string
  string += imu.readGyro(); // return something like this D0179E-179F0025 as string
  string += tensor1.read(); // return something like this G01 as string
  string += tensor2.read(); // return something like this H99 as string
  string += tensor3.read(); // return something like this I38 as string
  string += tensor4.read(); // return something like this J65 as string
  string += "K";


if (sending) {
    Serial.print(string);
    sending = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == 'N') {
      sending = true;
    }
  }
}

