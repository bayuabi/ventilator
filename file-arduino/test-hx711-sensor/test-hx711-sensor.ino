#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const int LOADCELL_DOUT_PIN2 = 5;
const int LOADCELL_SCK_PIN2 = 4;

long sensor_data[2];

HX711 sensor1;
HX711 sensor2;

void setup() {
  Serial.begin(115200);
  sensor1.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN, 32);
  sensor2.begin(LOADCELL_DOUT_PIN2, LOADCELL_SCK_PIN2, 32);
}

void loop() {

  if (sensor1.is_ready()) {
    sensor_data[0] = sensor1.read();
//    Serial.print("HX71/1 1 reading: ");
    float data = (float)sensor_data[0]*0.00005309;
    data = data + 89.803;
//    float data = (float)sensor_data[0];
    Serial.println(data);
  } 
//  else {
//    Serial.println("HX711 1 not found.");
//  }
//  if (sensor2.is_ready()) {
//    sensor_data[1] = sensor2.read();
//    Serial.print("\t");
//    Serial.print("HX711 2 reading: ");
//    float data = (float)sensor_data[1]*0.00005309;
//    data = data + 89.803;
////  float data = (float)sensor_data[1];
//    Serial.println(data);
//  } else {
//    Serial.println("HX711 1 not found.");
//  }


//  if (sensor2.is_ready()) {
//    sensor_data[1] = sensor2.read();
//    Serial.print("\t");
//    Serial.print("HX711 2 reading: ");
//    float data = (float)sensor_data[1]*0.00053078556;
//    data = data - 274.4;
//    Serial.println(data);
//  } else {
//    Serial.println("HX711 2 not found.");
//  }

  delay(100);
  
}
