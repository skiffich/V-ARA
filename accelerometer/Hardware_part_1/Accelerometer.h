#ifndef Accelerometer_h
#define Accelerometer_h

#include <Arduino.h>
#include <Wire.h>
#include <ADXL345.h>

class Acc {
public:
	Acc();
	~Acc();
	String str;
	void accLoop();
  void accInit();
private:
	ADXL345 accelerometer;
};

#endif
