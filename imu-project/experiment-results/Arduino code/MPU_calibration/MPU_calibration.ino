// I2Cdev and MPU6050 libraries
#include <I2Cdev.h>
#include <MPU6050.h>
#include "Wire.h"

// class default I2C address is 0x68
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu(0x68);

int16_t ax, ay, az;
int i;

// Offsets:
// TO KEEP ! Factory offsets : -2694 -175 1026
double offsetX, offsetY, offsetZ;

#define OUTPUT_READABLE_ACCELGYRO

#define LED_PIN 13
bool blinkState = false;

/* A function that can calibrate the mpu automatically.
   After setting all the offsets to 0, it averages 1024 samples, then sets the new offsets.
   The code is pretty straightforward.
   */
   
void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    
    // Initializing serial communication
    Serial.begin(9600);

    // Initializing device
    Serial.println("Initializing I2C devices...");
    mpu.initialize();

    // Verifying connection
    Serial.println("Testing device connections...");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    
    // Setting and displaying range of accelerometer
    /* 0 = +/- 2g = +/- 250 degrees/sec
     * 1 = +/- 4g = +/- 500 degrees/sec
     * 2 = +/- 8g = +/- 1000 degrees/sec
     * 3 = +/- 16g = +/- 2000 degrees/sec   */
    mpu.setFullScaleAccelRange(2);
    Serial.print("accel range : ");
    Serial.println(mpu.getFullScaleAccelRange());
    
    Serial.println("Initializing offsets...");
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
    
    // Configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);

}

void loop() {
    // Reading raw accel/gyro
  mpu.getAcceleration(&ax, &ay, &az);
  
  offsetX+=ax;
  offsetY+=ay;
  offsetZ+=az;
  
  /*Serial.print("offsets:");
  Serial.print(offsetX); Serial.print("\t");
  Serial.print(offsetY); Serial.print("\t");
  Serial.println(offsetZ);*/
  
  if(i==4096) {
      // Calculating average
      offsetX=offsetX/4096;
      offsetY=offsetY/4096;
      offsetZ=offsetZ/4096;
    Serial.print("Final offsets (TO KEEP)");
    Serial.print(offsetX); Serial.print("\t");
    Serial.print(offsetY); Serial.print("\t");
    Serial.println(offsetZ);
  }
  
  i++;
    
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
}
