#include <Ps3Controller.h>

int player = 1;
int speed = 0;

const int driver_speed = 4;

const int motor_fr = 15;
const int motor_fl = 16;
const int motor_br = 17;
const int motor_bl = 18;

const int full = 255, half = 127;

const int brake_time = 50;

boolean was_forward = false;

void stop() {
  if (was_forward) {
    backward(full);
    delay(brake_time);
    was_forward = false;
  }
  analogWrite(driver_speed, LOW);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, HIGH);
}
void forward(int speed) {
  was_forward = true;
  analogWrite(driver_speed, speed);
  digitalWrite(motor_fr, LOW);
  digitalWrite(motor_fl, LOW);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, HIGH);
}
void backward(int speed) {
  analogWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, LOW);
  digitalWrite(motor_bl, LOW);
  was_forward = false;
}
void forwardright(int speed) {
  analogWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, LOW);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, HIGH);
  was_forward = false;
}
void forwardleft(int speed) {
  analogWrite(driver_speed, speed);
  digitalWrite(motor_fr, LOW);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, HIGH);
  was_forward = false;
}
void backwardright(int speed) {
  analogWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, LOW);
  digitalWrite(motor_bl, HIGH);
  was_forward = false;
}
void backwardleft(int speed) {
  analogWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, HIGH);
  digitalWrite(motor_br, HIGH);
  digitalWrite(motor_bl, LOW);
  was_forward = false;
}
void turnright(int speed) {
  analogWrite(driver_speed, speed);
  digitalWrite(motor_fr, HIGH);
  digitalWrite(motor_fl, LOW);
  digitalWrite(motor_br, LOW);
  digitalWrite(motor_bl, HIGH);
  was_forward = false;
}
void turnleft(int speed) {
  analogWrite(driver_speed, speed);
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
  if (Ps3.event.button_up.cross)
    Serial.println("Released the cross button");

  if (Ps3.event.button_down.square)
    Serial.println("Started pressing the square button");
  if (Ps3.event.button_up.square)
    Serial.println("Released the square button");

  if (Ps3.event.button_down.triangle)
    Serial.println("Started pressing the triangle button");
  if (Ps3.event.button_up.triangle)
    Serial.println("Released the triangle button");

  if (Ps3.event.button_down.circle) {
    Serial.println("Started pressing the circle button");
    speed = half;
    player = 7;
  }
  if (Ps3.event.button_up.circle)
    Serial.println("Released the circle button");
  //---------------- Analog stick value events ---------------
  int lxAxisValue = (Ps3.data.analog.stick.lx);  //left stick - x axis
  int lyAxisValue = (Ps3.data.analog.stick.ly);  //left stick - y axis
  int rxAxisValue = (Ps3.data.analog.stick.rx);  //right stick - x axis
  int ryAxisValue = (Ps3.data.analog.stick.ry);  //right stick - y axis
  if (lyAxisValue > 10 && lxAxisValue < 10 && lxAxisValue > -10) {
    forward(abs(lyAxisValue * 2));
  } else if (lyAxisValue < -10 && lxAxisValue < 10 && lxAxisValue > -10) {
    backward(abs(lyAxisValue * 2));
  } else if (lxAxisValue > 10 && lyAxisValue < 10 && lyAxisValue > -10) {
    turnright(abs(lxAxisValue * 2));
  } else if (lxAxisValue < -10 && lyAxisValue < 10 && lyAxisValue > -10) {
    turnleft(abs(lxAxisValue * 2));
  } else if (lyAxisValue > 10 && lxAxisValue > 10) {
    forwardright(abs(lyAxisValue * 2));
  } else if (lyAxisValue > 10 && lxAxisValue < -10) {
    forwardleft(abs(lyAxisValue * 2));
  } else if (lyAxisValue < -10 && lxAxisValue > 10) {
    backwardright(abs(lyAxisValue * 2));
  } else if (lyAxisValue < -10 && lxAxisValue < -10) {
    backwardleft(abs(lyAxisValue * 2));
  } else if (ryAxisValue > 10 && rxAxisValue < 10 && rxAxisValue > -10) {
    forward(speed);
  } else if (ryAxisValue < -10 && rxAxisValue < 10 && rxAxisValue > -10) {
    backward(speed);
  } else if (rxAxisValue > 10 && ryAxisValue < 10 && ryAxisValue > -10) {
    turnright(speed);
  } else if (rxAxisValue < -10 && ryAxisValue < 10 && ryAxisValue > -10) {
    turnleft(speed);
  } else if (ryAxisValue > 10 && rxAxisValue > 10) {
    forwardright(speed);
  } else if (ryAxisValue > 10 && rxAxisValue < -10) {
    forwardleft(speed);
  } else if (ryAxisValue < -10 && rxAxisValue > 10) {
    backwardright(speed);
  } else if (ryAxisValue < -10 && rxAxisValue < -10) {
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
  pinMode(driver_speed, OUTPUT);
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
  delay(500);
}