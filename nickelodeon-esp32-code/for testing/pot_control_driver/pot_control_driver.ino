//motor driver pin
const int driver_pin = 4;
const int driver_speed = 0;  // channel number
const int pot = 14;
//relay pins (labeled as directions that the robot goes to when used)
const int motor_fr = 15;
const int motor_fl = 16;
const int motor_br = 17;
const int motor_bl = 18;

void setup() {
  Serial.begin(115200);
  ledcSetup(driver_speed,1000,8);
  ledcAttachPin(driver_pin, driver_speed);
  ledcWrite(driver_speed, 0);
  //set pins for relay module
  pinMode(motor_fr, OUTPUT);
  pinMode(motor_fl, OUTPUT);
  pinMode(motor_br, OUTPUT);
  pinMode(motor_bl, OUTPUT);
  digitalWrite(motor_fr, LOW);
  digitalWrite(motor_fl, LOW);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, HIGH);
}

void loop() {
  ledcWrite(driver_speed, (63));
  ledcWrite(driver_speed, (analogRead(pot)/16));
  Serial.println(analogRead(pot)/16);
  delay(50);
}