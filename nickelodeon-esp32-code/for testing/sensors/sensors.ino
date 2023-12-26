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
int distance_fi = 1000, distance_ri = 1000, distance_li = 1000;  // the distance each ultrasonic reads internally
int distance_f1 = 1000, distance_r1 = 1000, distance_l1 = 1000;  // to avreage the reads
int distance_f2 = 1000, distance_r2 = 1000, distance_l2 = 1000;
int distance_f3 = 1000, distance_r3 = 1000, distance_l3 = 1000;
int distance_f4 = 1000, distance_r4 = 1000, distance_l4 = 1000;
int distance_f5 = 1000, distance_r5 = 1000, distance_l5 = 1000;
int distance_f = 1000, distance_r = 1000, distance_l = 1000;  // final reads
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

void readUltrasonicSensor(int trigPin, int echoPin, int& distance) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long timeout = micros();
  while (digitalRead(echoPin) == LOW) {
    if (micros() - timeout > 2000) {  // Timeout after 2000 microseconds
      distance = 1000;                // Set a default value if sensor doesn't respond
      return;
    }
  }

  unsigned long startTime = micros();
  timeout = micros();
  while (digitalRead(echoPin) == HIGH) {
    if (micros() - timeout > 25000) {  // Timeout after 25 milliseconds
      distance = 1000;                 // Set a default value if no echo received
      return;
    }
  }
  unsigned long travelTime = micros() - startTime;
  distance = travelTime * 0.034 / 2;
}

void codeForTask1(void* parameter) {
  for (;;) {
    readUltrasonicSensor(trig_f, echo_f, distance_fi);
    readUltrasonicSensor(trig_r, echo_r, distance_ri);
    readUltrasonicSensor(trig_l, echo_l, distance_li);

    ir_fr = digitalRead(ir_frs);
    ir_fl = digitalRead(ir_fls);
    ir_br = digitalRead(ir_brs);
    ir_bl = digitalRead(ir_bls);

    distance_f1 = distance_f2;
    distance_f2 = distance_f3;
    distance_f3 = distance_f4;
    distance_f4 = distance_f5;
    distance_f5 = distance_fi;
    distance_f = (distance_f1 + distance_f2 + distance_f3 + distance_f4 + distance_f5) / 5;

    distance_r1 = distance_r2;
    distance_r2 = distance_r3;
    distance_r3 = distance_r4;
    distance_r4 = distance_r5;
    distance_r5 = distance_ri;
    distance_r = (distance_r1 + distance_r2 + distance_r3 + distance_r4 + distance_r5) / 5;
    
    distance_l1 = distance_l2;
    distance_l2 = distance_l3;
    distance_l3 = distance_l4;
    distance_l4 = distance_l5;
    distance_l5 = distance_li;
    distance_l = (distance_l1 + distance_l2 + distance_l3 + distance_l4 + distance_l5) / 5;
    
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
    delay(10);
  }
}


void loop() {
  delay(1000);
}
