#include "Tensor.h"

Tensor tensor1(0);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print(tensor1.DRead());
  Serial.print(" ");
  Serial.print(tensor1.Dmax());
  Serial.print(" ");
  Serial.print(tensor1.Dmin());
  Serial.print(" ");
  Serial.println(tensor1.TensorRead());
}

