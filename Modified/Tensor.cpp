#include "Tensor.h"

Tensor::Tensor(int pin) {
  minValue = 32767;
  maxValue = 0;
  this->pin = pin;

}

Tensor::~Tensor(){}

int Tensor::TensorRead(){
  Read = analogRead(pin);
  return TensorCalib(Read);
}

int Tensor::TensorCalib(int x){
  sumCount += 1;
  sum += x;
  if (sumCount == 100){
    avg = sum / 100;
    sum = 0;
    sumCount = 0;
    if (avg < minValue) {
      minValue = avg; 
    }
    if (avg > maxValue) {
      maxValue = avg;
    }
    avg = 0;
  }
  return map(x, minValue, maxValue, 0, 100);
}

int Tensor::Dmax()
{
  return maxValue;
}

int Tensor::Dmin()
{
  return minValue;
}
int Tensor::DRead()
{
  return Read;
}
int Tensor::Sum()
{
  return avg;
}


