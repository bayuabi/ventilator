#define VALVE_PIN_1 8
#define VALVE_PIN_2 9

void setup() {
  Serial.begin(115200);
  pinMode(VALVE_PIN_1, OUTPUT);
  pinMode(VALVE_PIN_2, OUTPUT);

}

void loop() {
  digitalWrite(VALVE_PIN_1, HIGH);
  digitalWrite(VALVE_PIN_2, HIGH);
  delay(100);
  digitalWrite(VALVE_PIN_1, LOW);
  digitalWrite(VALVE_PIN_2, LOW);
  delay(900);
}
