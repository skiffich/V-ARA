#include "Tensor.h"
Tensor tensor1(0/*, 'G'*/);


void setup()
{
  Serial.begin(57600);
}

void loop()
{
  Serial.print(tensor1.DRead());
  Serial.print(" ");
  Serial.print(tensor1.Dmax());
  Serial.print(" ");
  Serial.print(tensor1.Dmin());
  Serial.print(" ");
  Serial.print(tensor1.TensorRead());
  Serial.println(" ");
  /*String string = "";
  string += String(tensor1.TensorRead());
  Serial.println(string);*/
}
