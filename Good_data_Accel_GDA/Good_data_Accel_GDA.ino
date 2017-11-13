#include "Wire.h"

// I2Cdev and ADXL345 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "ADXL345.h"
#include "L3G4200D.h"
#include "BMP085.h"
#include "HMC5883L.h"

// accel
ADXL345 accel;
int16_t ax, ay, az;

// gyro
L3G4200D gyro;
int avx, avy, avz;

// baro
BMP085 baro;
float temperature;
float pressure;
float altitude;
int pot1;
int pot2;
int pot3;
float K = 0.01;
int32_t lastMicros;

// magneto
HMC5883L mag;
int16_t mx, my, mz;

#define LED_PIN 0 // (Arduino is 13, Teensy is 6)
bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accel.initialize();
    gyro.initialize();
    baro.initialize();
    mag.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accel.testConnection() ? "ADXL345 connection successful" : "ADXL345 connection failed");
    Serial.println(gyro.testConnection() ? "L3G4200D connection successful" : "L3G4200D connection failed");
    Serial.println(baro.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");
    Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
    
    gyro.setFullScale(2000);
}

void loop() {
    
    //accelerometer();
    gyroscope();
    //barometer();
    //magneto();
    
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
    // delay 100 msec to allow visually parsing blink and any serial output
    delay(10);
}

void accelerometer(){
    // read raw accel measurements from device
    accel.getAcceleration(&ax, &ay, &az);

    // display tab-separated accel x/y/z values
    Serial.print("accel:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.println(az);
}

void gyroscope(){
    gyro.getAngularVelocity(&avx, &avy, &avz);
    
    pot1 = (1 - K) * pot1 + K * avx;
    pot2 = (1 - K) * pot2 + K * avy;
    pot3 = (1 - K) * pot3 + K * avz;
    Serial.print("angular velocity:\t");
    Serial.print(pot1); Serial.print("\t");
    Serial.print(pot2); Serial.print("\t");
    Serial.println(pot3);  
}

void barometer(){
    // request temperature
    baro.setControl(BMP085_MODE_TEMPERATURE);
    
    // wait appropriate time for conversion (4.5ms delay)
    lastMicros = micros();
    while (micros() - lastMicros < baro.getMeasureDelayMicroseconds());

    // read calibrated temperature value in degrees Celsius
    temperature = baro.getTemperatureC();

    // request pressure (3x oversampling mode, high detail, 23.5ms delay)
    baro.setControl(BMP085_MODE_PRESSURE_3);
    while (micros() - lastMicros < baro.getMeasureDelayMicroseconds());

    // read calibrated pressure value in Pascals (Pa)
    pressure = baro.getPressure();

    // calculate absolute altitude in meters based on known pressure
    // (may pass a second "sea level pressure" parameter here,
    // otherwise uses the standard value of 101325 Pa)
    altitude = baro.getAltitude(pressure);

    // display measured values if appropriate
    Serial.print("T/P/A\t");
    Serial.print(temperature); Serial.print("\t");
    Serial.print(pressure); Serial.print("\t");
    Serial.print(altitude);
    Serial.println("");
}

void magneto(){
   // read raw heading measurements from device
    mag.getHeading(&mx, &my, &mz);

    // display tab-separated gyro x/y/z values
    Serial.print("mag:\t");
    Serial.print(mx); Serial.print("\t");
    Serial.print(my); Serial.print("\t");
    Serial.print(mz); Serial.print("\t");
    
    // To calculate heading in degrees. 0 degree indicates North
    float heading = atan2(my, mx);
    if(heading < 0)
      heading += 2 * M_PI;
    Serial.print("heading:\t");
    Serial.println(heading * 180/M_PI);
}