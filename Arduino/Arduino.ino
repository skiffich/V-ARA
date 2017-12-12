#include <Firmata.h>

// Result string template
char string[40] = "A000B000C000D0000E0000F0000G00H00I00J00K" ;

// String received handler
void stringCallback(char *myString)
{
  String str = myString; // convert to String format
  if(str == "N")
  {
    // If received string is "N" - send result string
    Firmata.sendString(string);
  }
}

void setup()
{
  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  Firmata.attach(STRING_DATA, stringCallback); // Add String received handler
  Firmata.begin(57600); // Start Firmata with 57600 baud rate
}

void loop()
{
  // Check if something received by Firmata protocol
  while (Firmata.available()) {
    Firmata.processInput();
  }
}

