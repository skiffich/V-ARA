#include <Wire.h>
#include <ADXL345.h>

ADXL345 accelerometer;

void setup(void) 
{
  Serial.begin(9600);

  // Initialize ADXL345
  if (!accelerometer.begin()) delay(500);

  // Set measurement range
  accelerometer.setRange(ADXL345_RANGE_2G);
}

void loop(void) 
{
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
