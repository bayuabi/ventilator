#include <Arduino.h>
#include <ArduinoJson.h>
#include "SoftwareSerial.h"
#include "HX711.h"

SoftwareSerial blutut(11, 12);

#define TEST_BLUTUT false
#define TEST_VALVE false
#define TEST_HX711 false

#define VALVE_PIN_1 8
#define VALVE_PIN_2 9

#define HX711_DOUT_PIN    2
#define HX711_SCK_PIN     3

#define HX711_DOUT_PIN_2   5
#define HX711_SCK_PIN_2    4

#define SENSOR_TEKANAN_1 1
#define SENSOR_TEKANAN_2 2

HX711 sensor1;
HX711 sensor2;

#define MODE_VENTILATOR_MANUAL    0
#define MODE_VENTILATOR_OTOMATIS  1

#define MODE_VENTILATOR MODE_VENTILATOR_MANUAL

float sensorTekanan1, sensorTekanan2;
unsigned long startTime;
bool valveOnState, valveOffState = false;
float ambientIntercept;
float sensorIntercept, sensorSlope;

void testBlutut();
void testValve();
void testHX711(int number, bool withCalculation);
bool hx711Read(int number, float *data);
float hx711LinearCalc(float x, float slope, float intercept);
void sendData(float data1, float data2);

void setup() {
  Serial.begin(115200);
  blutut.begin(9600);

  pinMode(VALVE_PIN_1, OUTPUT);
  pinMode(VALVE_PIN_2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  sensor1.begin(HX711_DOUT_PIN, HX711_SCK_PIN, 32);
  sensor2.begin(HX711_DOUT_PIN_2, HX711_SCK_PIN_2, 32);

  for(int i=0; i<20; i++)
    {
      hx711Read(SENSOR_TEKANAN_1, &sensorTekanan1);
      delay(100);
    }
  ambientIntercept = hx711LinearCalc(sensorTekanan1, sensorSlope, sensorIntercept);
  startTime = millis();
}

void loop() {
  #if TEST_BLUTUT
    while(true)
      {
        testBlutut();
      }
  #endif

  #if TEST_VALVE
    while(true)
      {
        testValve();
      }
  #endif

  #if TEST_HX711
    while(true)
      {
        testHX711(SENSOR_TEKANAN_1, true);
      }
  #endif

  if(hx711Read(SENSOR_TEKANAN_1, &sensorTekanan1))
    {
      sensorTekanan1 =  hx711LinearCalc(sensorTekanan1, sensorSlope, sensorIntercept);
      sensorTekanan1 -= ambientIntercept;
      Serial.println(sensorTekanan1);
    }

  sendData(sensorTekanan1, sensorTekanan2);

  switch (MODE_VENTILATOR)
    {
    case MODE_VENTILATOR_MANUAL :
      if(!valveOnState)
        {
          if(millis() - startTime >= 500)   //DURASI NYALA
            {
              valveOnState = !valveOnState;
              startTime = millis();
            }
        }
      if(valveOnState)
        {
          if(millis() - startTime >= 1000)   //DURASI MATI
            {
              valveOnState = !valveOnState;
              startTime = millis();
            }
        }
      digitalWrite(LED_BUILTIN, !valveOnState);

      break;
    
    case MODE_VENTILATOR_OTOMATIS :
      float ambangBawahTekanan = 0.5;
      float ambangAtasTekanan = 10.0;

      if(sensorTekanan1 < -ambangBawahTekanan)
        {
          valveOnState = !valveOnState;
        }
      if(sensorTekanan1 > ambangAtasTekanan)
        {
          valveOnState = !valveOnState;
        }
      digitalWrite(VALVE_PIN_1, valveOnState);

      break;

    default:
      break;
    }

}

void testBlutut(){
 sendData(random(0,100), random(0,50));
}

void testValve(){
  digitalWrite(VALVE_PIN_1, HIGH);
  digitalWrite(VALVE_PIN_2, HIGH);
  delay(100);
  digitalWrite(VALVE_PIN_1, LOW);
  digitalWrite(VALVE_PIN_2, LOW);
  delay(900);
}

void testHX711(int number, bool withCalculation){
  float tekanan;
  if(hx711Read(number, &tekanan))
    {
      if(withCalculation)
        {
          tekanan = hx711LinearCalc(tekanan, 0.00005309, 89.803);
        }
      Serial.println(tekanan);
    }
    else
      {
        Serial.println("CANNOT READ DATA");
      }
    
}

bool hx711Read(int number, float *data){
  switch (number)
    {
      case 1 :
        if (sensor1.is_ready()) 
          {
            *data = sensor1.read();
            return true;
          } 
          else
            {
              return false;
            }
        break;
      
      case 2:
        if (sensor2.is_ready()) 
          {
            *data = sensor2.read();
            return true;
          } 
          else
            {
              return false;
            }
      default:
        break;
    }
}

float hx711LinearCalc(float x, float slope, float intercept){
  return (x*slope) + intercept;
}

void sendData(float data1, float data2){
  String data;
  
  StaticJsonDocument<200> doc;

  doc["tekanan1"] = data1;
  doc["tekanan2"] = data2;

  serializeJson(doc, data);

  blutut.print(" ");
  blutut.println(data);
}