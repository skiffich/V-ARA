#include "Tensor.h"

Tensor::Tensor(int pin/*, char letter*/) {
  minValue = 32767;
  maxValue = 0;
  this->pin = pin;
  /*this->letter = letter;*/

}

Tensor::~Tensor(){}

int Tensor::TensorRead(){
  Read = analogRead(pin);
  return TensorCalib(Read);
}

int Tensor::TensorCalib(int x){
  sumCount += 1;
  sum += x;
  avg = sum / sumCount;
  if (avg < minValue) {
    minValue = avg; 
  }
  if (avg > maxValue) {
    maxValue = avg;
  }
  if (sumCount == 100){
    sum = 0;
    sumCount = 0;
    avg = 0;
  }
  /*value = map(x, minValue, maxValue, 0, 100);
  if (value >= 100) {
    value = "99";
  }
  if (value < 0) {
    value = '0';
  }
  if (value >= 0  & value <= 9) {
    value = '0' + value;
  }
  return value*/
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

