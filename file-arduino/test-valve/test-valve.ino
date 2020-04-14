#define VALVE_PIN_1 8
#define VALVE_PIN_2 9

void setup() {
  Serial.begin(115200);
  pinMode(VALVE_PIN_1, OUTPUT);
  pinMode(VALVE_PIN_2, OUTPUT);

}

void loop() {
  digitalWrite(VALVE_PIN_1, HIGH);
  delay(1000);
  digitalWrite(VALVE_PIN_1, LOW);
  delay(1000);

  digitalWrite(VALVE_PIN_2, HIGH);  delay(1000);
  digitalWrite(VALVE_PIN_2, LOW);
  delay(1000);

}
