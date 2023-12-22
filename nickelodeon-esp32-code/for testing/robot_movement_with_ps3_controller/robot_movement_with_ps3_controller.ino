#include <Ps3Controller.h>

int player = 7;

//motor driver pin
const int driver_pin = 4;
const int driver_speed = 0;  // channel number

const int motor_fr = 15;
const int motor_fl = 16;
const int motor_br = 17;
const int motor_bl = 18;

const int full = 255, half = 127;

int speed = half;

const int brake_time = 50;

boolean was_forward = false;

void stop() {
  if (was_forward) {
    backward(full);
    delay(brake_time);
    was_forward = false;
  }
  ledcWrite(driver_speed, 0);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, HIGH);
}
void forward(int speed) {
  was_forward = true;
  ledcWrite(driver_speed, speed);
  digitalWrite(motor_fr, LOW);
  digitalWrite(motor_fl, LOW);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, HIGH);
}
void backward(int speed) {
  ledcWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, LOW);
  digitalWrite(motor_bl, LOW);
  was_forward = false;
}
void forwardright(int speed) {
  ledcWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, LOW);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, HIGH);
  was_forward = false;
}
void forwardleft(int speed) {
  ledcWrite(driver_speed, speed);
  digitalWrite(motor_fr, LOW);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, HIGH);
  was_forward = false;
}
void backwardright(int speed) {
  ledcWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, LOW);
  digitalWrite(motor_bl, HIGH);
  was_forward = false;
}
void backwardleft(int speed) {
  ledcWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, LOW);
  was_forward = false;
}
void turnright(int speed) {
  ledcWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, LOW);
  digitalWrite(motor_br, LOW);
  digitalWrite(motor_bl, HIGH);
  was_forward = false;
}
void turnleft(int speed) {
  ledcWrite(driver_speed, speed);
  digitalWrite(motor_fr, LOW);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, LOW);
  was_forward = false;
}
void notify() {
  //--- Digital cross/square/triangle/circle button events ---
  if (Ps3.event.button_down.cross) {
    Serial.println("Started pressing the cross button");
    speed = full;
    player = 10;
  }
  if (Ps3.event.button_down.circle) {
    Serial.println("Started pressing the circle button");
    speed = half;
    player = 7;
  }
  //---------------- Analog stick value events ---------------
  int lyAxisValue = (Ps3.data.analog.stick.ly);  //left stick - y axis
  int ryAxisValue = (Ps3.data.analog.stick.ry);  //right stick - y axis
  if (lyAxisValue > 10 && ryAxisValue > 10) {
    forward(speed);
  } else if (lyAxisValue < -10 && ryAxisValue < -10) {
    backward(speed);
  } else if (lyAxisValue > 10 && ryAxisValue < -10) {
    turnright(speed);
  } else if (lyAxisValue < -10 && ryAxisValue > 10) {
    turnleft(speed);
  } else if (lyAxisValue > 10) {
    forwardright(speed);
  } else if (ryAxisValue > 10) {
    forwardleft(speed);
  } else if (lyAxisValue < -10) {
    backwardright(speed);
  } else if (ryAxisValue < -10) {
    backwardleft(speed);
  } else {
    stop();
  }
}
void onConnect() {
  Serial.println("Connected.");
}
void onDisConnect() {
  stop();
}
void setup() {
  Serial.begin(115200);
  //set pins for motor driver
  ledcSetup(driver_speed, 1000, 8);
  ledcAttachPin(driver_pin, driver_speed);
  pinMode(motor_fr, OUTPUT);
  pinMode(motor_fl, OUTPUT);
  pinMode(motor_br, OUTPUT);
  pinMode(motor_bl, OUTPUT);
  stop();
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("00:1A:7D:DA:71:10");
  Serial.println("Ready.");
}
void loop() {
  if (!Ps3.isConnected())
    return;
  Ps3.setPlayer(player);
  delay(100);
}