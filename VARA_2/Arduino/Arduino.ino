#include <Firmata.h>
#include "IMU.h"
#include "Tensor.h"

IMU imu;

//*
Tensor tensor1(0, "G");
Tensor tensor2(1, "H");
Tensor tensor3(2, "I");
Tensor tensor4(3, "J");
//*/
char string[40] = "A000B000C000D0000E0000F0000G00H00I00J00K" ;

void stringCallback(char *myString)
{
  String str = myString;
  if(str == "N")
  {
    Firmata.sendString(string);
  }
}

void setup() {
  imu.imuInit();
  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  Firmata.attach(STRING_DATA, stringCallback);
  Firmata.begin(57600);
}

void loop() {
  while (Firmata.available()) {
    Firmata.processInput();
  }
  
  imu.imuLoop();
  String resString = "";
  resString =  imu.str;        // Read AHRS string
  /*
  resString += tensor1.tensorRead(); // Read tensor1
  resString += tensor2.tensorRead(); // Read tensor2
  resString += tensor3.tensorRead(); // Read tensor3
  resString += tensor4.tensorRead(); // Read tensor4 //*/
  
  resString += "G00";
  resString += "H00";
  resString += "I00";
  resString += "J00";
  resString += "K";

  snprintf( string, sizeof(string)-1, "%s", resString.c_str() );
  string[sizeof(string)] = '\0';
  
  //Serial.println(resString);
}
