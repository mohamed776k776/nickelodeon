//ultrasonic pins (front - right - left) triger and echo each
const int trig_f = 32;
const int echo_f = 33;
const int trig_r = 25;
const int echo_r = 26;
const int trig_l = 27;
const int echo_l = 13;
//ir pins (front right sensor - front left sensor - back right sensor - back left sensor)
const int ir_frs = 36;
const int ir_fls = 39;
const int ir_brs = 34;
const int ir_bls = 35;

//sensors state (shared between all cores)
int distance_f = 1000, distance_r = 1000, distance_l = 1000;  // the distance each ultrasonic reads
long time_f, time_r, time_l;                                  // used to store time to calculate distance for the ultrasonic (if we know the time and the speed of sound is constant then we can calculate distance 🤓)
int ir_fr = 1, ir_fl = 1, ir_br = 1, ir_bl = 1;               // ir is 1 when not detecting and 0 when detecting (inverted 🙃)

//task handle for core 0 (core 1 does not need one because it runs the loop function)
TaskHandle_t Task1;

void setup() {
  Serial.begin(115200);  //remove if not using serial monitor
  //set pins for ultrasonic sensors (an output pin is used to triger the sensor and then the readings (echo) of the sensor is the input)
  pinMode(trig_f, OUTPUT);
  pinMode(echo_f, INPUT);
  pinMode(trig_r, OUTPUT);
  pinMode(echo_r, INPUT);
  pinMode(trig_l, OUTPUT);
  pinMode(echo_l, INPUT);
  //set pins for ir sensors
  pinMode(ir_frs, INPUT);
  pinMode(ir_fls, INPUT);
  pinMode(ir_brs, INPUT);
  pinMode(ir_bls, INPUT);
  xTaskCreatePinnedToCore(codeForTask1, "task_1", 1000, NULL, 1, &Task1, 0);
  delay(500);
}

//core 0 code
void codeForTask1(void* parameter) {
  for (;;) {
    //front ultrasonic sensor
    digitalWrite(trig_f, LOW);  //ensure it was not trigered before
    delayMicroseconds(2);
    digitalWrite(trig_f, HIGH);  //triger
    delayMicroseconds(10);
    digitalWrite(trig_f, LOW);
    time_f = pulseIn(echo_f, HIGH);   //wait for the echo of the sensor
    distance_f = time_f * 0.034 / 2;  //calculate distance using time and the constant speed of sound

    //right ultrasonic sensor (same as the front)
    digitalWrite(trig_r, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_r, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_r, LOW);
    time_r = pulseIn(echo_r, HIGH);
    distance_r = time_r * 0.034 / 2;

    //left ultrasonic sensor (same as the front)
    digitalWrite(trig_l, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_l, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_l, LOW);
    time_l = pulseIn(echo_l, HIGH);
    distance_l = time_l * 0.034 / 2;

    //ir sensors
    ir_fr = digitalRead(ir_frs);
    ir_fl = digitalRead(ir_fls);
    ir_br = digitalRead(ir_brs);
    ir_bl = digitalRead(ir_bls);
    Serial.print(distance_l);
    Serial.print("  ");
    Serial.print(distance_f);
    Serial.print("  ");
    Serial.print(distance_r);
    Serial.print("  ");
    Serial.print(ir_bl);
    Serial.print("  ");
    Serial.print(ir_br);
    Serial.print("  ");
    Serial.print(ir_fl);
    Serial.print("  ");
    Serial.println(ir_fr);
  }
}

void loop() {
  delay(1000);
}
