/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;

// Set your access point network credentials
const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";  

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readax() {
  return String(AcX);
 }

String readay() {
  return String(AcY);
}

String readaz() {
  return String(AcZ);
}

String readgx() {
  return String(GyX);
  //return String(1.8 * bme.readTemperature() + 32);
}

String readgy() {
  return String(GyY);
}

String readgz() {
  return String(GyZ);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println(); 
 Wire.begin();
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);  // PWR_MGMT_1 register
 Wire.write(0);     // set to zero (wakes up the MPU-6050)
 Wire.endTransmission(true);
 Serial.println("Wrote to IMU");
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/ax", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readax().c_str());
  });
  server.on("/ay", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readay().c_str());
  });
  server.on("/az", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readaz().c_str());
  });

  server.on("/gx", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readgx().c_str());
  });
  server.on("/gy", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readgy().c_str());
  });
  server.on("/gz", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readgz().c_str());
  });
  
  bool status;
  
  // Start server
  server.begin();
}
 
void loop(){  
mpu_read();
}
void mpu_read(){
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
 Wire.endTransmission(false);
 Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
 AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
 AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
 AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
 //Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
 GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
 GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
 GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
 Serial.print("Accelerometer Values: \n");
 Serial.print("AcX: "); Serial.print(AcX); Serial.print("\nAcY: "); Serial.print(AcY); Serial.print("\nAcZ: "); Serial.print(AcZ);   
 //Serial.print("\nTemperature: " );  Serial.print(Tmp);
 Serial.print("\nGyroscope Values: \n");
 Serial.print("GyX: "); Serial.print(GyX); Serial.print("\nGyY: "); Serial.print(GyY); Serial.print("\nGyZ: "); Serial.print(GyZ);
 Serial.print("\n");
 delay(3000);
 }
