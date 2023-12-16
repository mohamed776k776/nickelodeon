//motor driver pin
const int driver_speed = 4;
const int pot = 13;

void setup() {
  Serial.begin(115200);
  pinMode(driver_speed, OUTPUT);
  analogWrite(driver_speed, 0);
}

void loop() {
  analogWrite(driver_speed, (analogRead(pot)/16));
  delay(50);
}