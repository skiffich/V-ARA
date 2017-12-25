#include <Wire.h>
#include <L3G4200D.h>
#include "Mouse.h"
#include "Tensor.h"

L3G4200D gyroscope;

int x, y, z;
int ox, oy, oz;
int cx, cy, cz;
unsigned long t;

void setup() 
{
  Serial.begin(9600);
  
  Serial.print("Calibration");

  while(!gyroscope.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50))
  {
    Serial.println("Could not find a valid L3G4200D sensor, check wiring!");
    delay(500);
  }
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  gyroscope.calibrate();
  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  gyroscope.setThreshold(1.5);

  Mouse.begin();

  t = millis();
}

void loop()
{
  Tensor tensor(1, "A");
  Tensor tensor2(2, "A");
  while(true){
    // Read normalized values in deg/sec
    Vector gyr = gyroscope.readNormalize();
    tensor.tensorRead();
    tensor2.tensorRead();
  
    x = map(round(gyr.XAxis), cx - 100, cx + 100, 0, 10);
    y = map(round(gyr.YAxis), cy - 100, cy + 100, 0, 10);
    z = map(round(gyr.ZAxis), cz - 100, cz + 100, 0, 10);
  
    if(millis() - t < 2000)
    {
      if(millis()%100==0)
        Serial.print(".");
      ox = x;
      oy = y;
      oz = z;
      cx = round(gyr.XAxis);
      cy = round(gyr.YAxis);
      cz = round(gyr.ZAxis);
    }
    else
    {
      x -= ox;
      y -= oy;
      z -= oz;
  
      Mouse.move(-z, y, 0); // 3 параметер це колесо прокрутки тому 0
      
      Serial.print(x);
      Serial.print("\t");
      Serial.print(y);
      Serial.print("\t");
      Serial.print(z);
  
      if(tensor.input_c > 95)
      {
        Serial.print("\t");
        Serial.print("Left");
        if (!Mouse.isPressed(MOUSE_LEFT)) {
          Mouse.press(MOUSE_LEFT);
        }
      }
      else{
        if (Mouse.isPressed(MOUSE_LEFT)) {
          Mouse.release(MOUSE_LEFT);
        }
       }
       if(tensor2.input_c > 95)
      {
        Serial.print("\t");
        Serial.print("Right");
        if (!Mouse.isPressed(MOUSE_RIGHT)) {
          Mouse.press(MOUSE_RIGHT);
        }
      }
      else{
        if (Mouse.isPressed(MOUSE_RIGHT)) {
          Mouse.release(MOUSE_RIGHT);
        }
       }
       Serial.println("");
     }
  }
}
