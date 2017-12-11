#include "AccGyro.h"

AccGyro accGyro;

void setup() {
  // put your setup code here, to run once:
  accGyro.accGyroInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  accGyro.accGyroLoop();
}
