#include "Tensor.h"
Tensor tensor1(0, "G");
Tensor tensor2(1, "H");

void setup()
{
  Serial.begin(57600);
}

void loop()
{
  //tensor2.tensorRead();
  Serial.print(tensor1.tensorRead());
  //Serial.print(analogRead(0));
  Serial.print("\t"); 

  Serial.println(tensor2.tensorRead());
  //Serial.println(analogRead(1));
}

