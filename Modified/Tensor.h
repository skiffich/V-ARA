#ifndef Tensor_h
#define Tensor_h

#include <Arduino.h>

class Tensor {
public:
  float sum = 0;
  int sumCount = 0;
  int avg = 0;
  Tensor(int pin);
  ~Tensor();
  int TensorRead();
  int Sum();
  int Dmax();
  int Dmin();
  int DRead();
private:
  int minValue, maxValue, pin;
  int TensorCalib(int x);
  int Read;
};

#endif
