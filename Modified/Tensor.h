#ifndef Tensor_h
#define Tensor_h

#include <Arduino.h>

class Tensor {
public:
  Tensor(int pin/*, char letter*/);
  ~Tensor();
  int TensorRead();
  int Sum();
  int Dmax();
  int Dmin();
  int DRead();
  char letter;
private:
  int minValue, maxValue, pin;
  int TensorCalib(int x);
  int Read;
  float sum = 0;
  int sumCount = 0;
  int avg = 0;
  char value;
  char str;
};

#endif
