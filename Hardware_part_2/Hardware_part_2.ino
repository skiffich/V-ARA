#include "AccGyro.h"

AccGyro accGyro;

void setup() {
  // put your setup code here, to run once:
  accGyro.accGyroInit();

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  accGyro.accGyroLoop();
  Serial.println(accGyro.str);
}
