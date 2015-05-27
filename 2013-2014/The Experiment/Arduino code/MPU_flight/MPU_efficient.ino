unsigned char toSendSpi [16]; // tableau de unsigned char 

int rangToSendSpi = 0; 

// I2Cdev and MPU6050 libraries
#include <I2Cdev.h>
#include <MPU6050.h>
#include "Wire.h"

// class default I2C address is 0x68
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu(0x68);

/* *********************************************************************************** */
/* IMU CODE => READ RAW ACC, CONVERSION, AVERAGING, FILTERING WINDOWS, DATA TRANSFERT  */
/* *********************************************************************************** */


int16_t ax, ay, az; // raw accelerations
int16_t gx, gy, gz; // raw gyro
float accelerationX, accelerationY, accelerationZ; // accelerations in m*s^-2
float realAccelerationX=0, realAccelerationY=0, realAccelerationZ=0; // averaged and filtered accelerations
float rotationX, rotationY, rotationZ; // rotation in deg/s
float realRotationX=0, realRotationY=0, realRotationZ=0; // averaged and filtered rotation

int i=0; // counter for main loop

float conversion=9.81/4096; // sensivity (for accekerometer range = 2) : 4096 LSB/g where g = 9.81 m*s^-2
float conversion_g=1/32.8;

// Offsets:
// TO KEEP ! Factory offsets : -2694 -175 1026
double offsetX, offsetY, offsetZ;

// Necessary to output accelerations
#define OUTPUT_READABLE_ACCELGYRO

#define LED_PIN 13
bool blinkState = false;


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
     
    mpu.setFullScaleAccelRange(2); // set acceleromter range
    Serial.print("accel range : ");
    Serial.println(mpu.getFullScaleAccelRange());
    
    mpu.setFullScaleGyroRange(2); // set gyrometer range
    Serial.print("gyro range : ");
    Serial.println(mpu.getFullScaleGyroRange());
    
    
    // Configuring function offsets for accelerometer (Nous n'utilisons PAS cette fonction pour régler les OFFSETS)
    // Setting acc offsets to 0
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
    
    // Setting gyro offsets to 0
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    
    
    
    // Configure Arduino LED for
    //pinMode(LED_PIN, OUTPUT);
    
    
        toSendSpi[0] = 97 ;
        toSendSpi[1] = 129 ;
        toSendSpi[2] = 217 ;
        toSendSpi[3] = 16 ;
        toSendSpi[4] = 5 ;
        toSendSpi[5] = 0 ;
        toSendSpi[6] = 4 ;
        
    pinMode(10, INPUT);
    pinMode(11, INPUT);
    pinMode(12, OUTPUT);
    pinMode(13, INPUT);
    
	SPCR = (1 << SPIE) | (1 << SPE) | (0 << DORD) | (0 << MSTR) | (0 << CPOL) | (0 << CPHA) | (1 << SPR1) | (1 << SPR0);
	SPSR = (0 << SPIF) | (0 << WCOL) | (0 << SPI2X);


}




void loop() {     
    // Reading raw accel
    mpu.getAcceleration(&ax, &ay, &az);  
     
    // Reading raw rotation
    mpu.getRotation(&gx, &gy, &gz);
    
    
    
    //Converting to real values with offsets
    accelerationX=ax*conversion-15.60; 
    accelerationY=ay*conversion-0.99;
    accelerationZ=az*conversion+5.47;  
    
    
    // Calculating accelerations every 64th time and getting the average
    realAccelerationX+=accelerationX;
    realAccelerationY+=accelerationY;
    realAccelerationZ+=accelerationZ;
    if(i==64) {
        realAccelerationX=realAccelerationX/64;
        realAccelerationY=realAccelerationY/64;
        realAccelerationZ=realAccelerationZ/64; 

    
        // Filtering low accelerations (eliminates <|0.5|m/s²)
        if (realAccelerationX<=0.5 && realAccelerationX>=-0.5) {
            realAccelerationX=0; }
        if (realAccelerationY<=0.5 && realAccelerationY>=-0.5) {
            realAccelerationY=0; }
        if (realAccelerationZ<=10.31 && realAccelerationZ>=9.31) {
            realAccelerationZ=9.81; }
      
      
      
      
      //Converting to real values with offsets
    rotationX=gx*conversion_g; 
    rotationY=gy*conversion_g;
    rotationZ=gz*conversion_g;  
    
    
    // Calculating accelerations every 64th time and getting the average
    realRotationX+=rotationX;
    realRotationY+=rotationY;
    realRotationZ+=rotationZ;
    
        realRotationX=realRotationX/64;
        realRotationY=realRotationY/64;
        realRotationZ=realRotationZ/64; 

    
      
      
      
      
      

        //Displaying accelerations
        Serial.print("a:");
        Serial.print(realAccelerationX); Serial.print("\t");
        Serial.print(realAccelerationY); Serial.print("\t");
        Serial.println(realAccelerationZ);Serial.print("\t");
        
        
        //Displaying rotaions
        Serial.print("g:");
        Serial.print(realRotationX); Serial.print("\t");
        Serial.print(realRotationY); Serial.print("\t");
        Serial.println(realRotationZ);
        
        
        //Data transfert
        toSendSpi[7] = (realAccelerationX*100 + 32768)/256 ;
        toSendSpi[8] = realAccelerationX*100 +32768;
        toSendSpi[9] = (realAccelerationY*100 +32768)/256;
        toSendSpi[10] = realAccelerationY*100 +32768;
        toSendSpi[11] = (realAccelerationZ*100 +32768)/256;
        toSendSpi[12] = realAccelerationZ*100 +32768;
        //toSendSpi[7] = (realAccelerationX*10 + 1024)/256 + (realRotationX/6)*8; 
        //toSendSpi[8] = (realAccelerationX*10 +1024);
        //toSendSpi[9] = (realAccelerationY*10 + 1024)/256 + (realRotationY/6)*8; 
        //toSendSpi[10] = (realAccelerationY*10 +1024);
        //toSendSpi[11] = (realAccelerationZ*10 + 1024)/256 + (realRotationZ/6)*8;
        //toSendSpi[12] = (realAccelerationY*10 +1024);
        
        
        
        toSendSpi[13] = (unsigned char)((int)(realRotationX)/2+128) ;
        toSendSpi[14] = realRotationY/2.0+128.0 ;
        toSendSpi[15] = realRotationZ/2.0+128.0 ;
        
        
        //Serial.print("SPI:");
        //Serial.print(toSendSpi[13]);Serial.print("\t");
        //Serial.print(toSendSpi[14]);Serial.print("\t");
        //Serial.println(toSendSpi[15]);
      
        // initializing
        realAccelerationX=0;
        realAccelerationY=0;
        realAccelerationZ=0;
        realRotationX=0;
        realRotationY=0;
        realRotationZ=0;
        i=0;
    }
    
    i++;
    
    // Update frequency
    delay(1);
    
    // blink LED to indicate activity
    blinkState = !blinkState;
    //digitalWrite(LED_PIN, blinkState);

}



ISR ( SPI_STC_vect ) 
{
SPDR = toSendSpi [ rangToSendSpi ] ; 

rangToSendSpi ++ ; 

if ( rangToSendSpi >= 16) 
    rangToSendSpi = 0 ; 

}
