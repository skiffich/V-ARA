#include "Tensor.h"

Tensor::Tensor(int pin, String letter) {
  avgMin = 10000;
  avgMax = -10000;
  this->pin = pin;
  this->letter += letter;
  counterMin = counterMax = sumMin = sumMax = 0;
}

Tensor::~Tensor(){}

int Tensor::tensorCalib(int input){

String s;
  //s += "i ";s += input;s += "\tcMin ";s += counterMin;s += "\tcMax ";s += counterMax;s += "\tsMin ";s += sumMin;s += "\tsMax ";s += sumMax;s += "\taMin ";s += avgMin;s += "\taMax ";s += avgMax;s += "\tcal ";
  //Serial.print(s);
  
  if(input < avgMin - (avgMin / 30) && input > 300) {
    counterMin++;
    sumMin += input;
    avgMin = sumMin / counterMin;
    if(counterMin > 100) {
      counterMin = 1;
      sumMin = avgMin;      
    }
  }
  if(input > avgMax + (avgMax / 30)) {
    counterMax++;
    sumMax += input;
    avgMax = sumMax / counterMax;
    if(counterMax > 100) {
      counterMax = 1;
      sumMax = avgMax;      
    }
  }
  
  input = map(input, avgMin, avgMax, 0, 90);
  
  if (input >= 100) {
    input = 99;
  }
  
  if (input < 0) {
    input = 0;
  }


  //Serial.println(input);
  return input;
}

String Tensor::tensorRead(){
  str = "";
  str += letter;
  
  input_c = tensorCalib(analogRead(pin));
  
  if (input_c >= 0  & input_c <= 9) {
    str += "0";
  }
  
  str += input_c;
  return str;
}


