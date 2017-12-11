#ifndef Tensor_h
#define Tensor_h

#include <Arduino.h>

class Tensor {
public:
  Tensor(int pin, String letter);
  ~Tensor();
  String tensorRead();
private:
  int tensorCalib(int input);
  String str, letter;
  int pin, input_c, avgMin, avgMax, sumMin, sumMax, counterMin, counterMax;
};

#endif

