#include <Firmata.h>
#include "Accelerometer.h"

Acc acc;

void stringCallback(char *myString)
{
  String str = myString;
  if(str == "N")
  {
    Firmata.sendString(acc.str);
  }
}

void setup()
{
  acc.accInit();
  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  Firmata.attach(STRING_DATA, stringCallback);
  Firmata.begin(57600);
}

void loop()
{
  while (Firmata.available()) {
    Firmata.processInput();
  }
  acc.accLoop();
}
