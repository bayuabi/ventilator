#include "HX711.h"

#define VALVE_PIN_1 8
#define VALVE_PIN_2 9

float ambangBawahTekanan = 0.5;
float ambangAtasTekanan = 10.0;
int periode = 3000;
int waktuNyalaValve = 1000;
bool stateValveNyala, stateValveMati;
float intercept;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const int LOADCELL_DOUT_PIN2 = 5;
const int LOADCELL_SCK_PIN2 = 4;

long sensor_data[2];
float tekanan;

HX711 sensor1;
HX711 sensor2;

unsigned long previousMillis, currentMillis, startTime;

void setup() {
  Serial.begin(115200);
  sensor1.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN, 32);
  sensor2.begin(LOADCELL_DOUT_PIN2, LOADCELL_SCK_PIN2, 32);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VALVE_PIN_1, OUTPUT);
  pinMode(VALVE_PIN_2, OUTPUT);
  for(int i=0; i<20; i++){
    bacaSensor(intercept);
    delay(100);
  }
  intercept = tekanan;
}

void loop() {
  bacaSensor(intercept);
//  if(tekanan < 0){
//    digitalWrite(LED_BUILTIN, HIGH);
//  }
//  if(tekanan > ambangTekananAtas){
//    digitalWrite(LED_BUILTIN, LOW);
//  }
  if(tekanan < -ambangBawahTekanan){
    stateValveNyala = true;
    stateValveMati = false;
//    startTime = millis();
  }
  if(tekanan > ambangAtasTekanan){
    stateValveMati = true;
    stateValveNyala = false;
  }
  if(stateValveNyala){
//    if(millis() - startTime < waktuNyalaValve){
      digitalWrite(VALVE_PIN_1, HIGH);
      digitalWrite(VALVE_PIN_2, HIGH);
//    }
//    else{
//      stateValveNyala = false;
//      startTime = millis();
//    }
  }

  if(stateValveMati){
    digitalWrite(VALVE_PIN_1, LOW);
    digitalWrite(VALVE_PIN_2, LOW);
  }
  
  
//  if (sen/

//  if(tekanan < ambangBawah)
//    {
//      digitalWrite(VALVE_PIN_1, HIGH); 
//    }
//  delay(200);
//  if(tekanan > ambangAtas)
//    {
//      digitalWrite(VALVE_P/IN_1, LOW);
//    }
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

//  delay(1);
  
}

void bacaSensor(float intercept){
  if (sensor1.is_ready()) {
    sensor_data[0] = sensor1.read();
    tekanan = (float)sensor_data[0]*0.00005309;
    tekanan = tekanan + 89.803;
    tekanan = tekanan - intercept;
    Serial.println(tekanan);
  } 
}
