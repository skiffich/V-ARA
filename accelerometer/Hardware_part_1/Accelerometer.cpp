#include "Accelerometer.h"

Acc::Acc() {}

Acc::~Acc(){}

void Acc::accLoop(){
	// Read normalized values
	Vector norm = accelerometer.readNormalize();

	// Output normalized m/s^2
	Serial.print(int(norm.XAxis));
	Serial.print("\t");
	Serial.print(int(norm.YAxis));
	Serial.print("\t");
	Serial.print(int(norm.ZAxis));

	Serial.println();
}

void Acc::accInit(){
  
  // Initialize ADXL345
  if (!accelerometer.begin()) delay(500);

  // Set measurement range
  accelerometer.setRange(ADXL345_RANGE_2G);
  
  str = "X000Y000K";
}

