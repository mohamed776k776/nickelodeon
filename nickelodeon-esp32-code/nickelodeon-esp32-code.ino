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
//motor driver pin
const int driver_pin = 4;
const int driver_speed = 0;  // channel number
//relay pins (labeled as directions that the robot goes to when used)
const int motor_fr = 15;
const int motor_fl = 16;
const int motor_br = 17;
const int motor_bl = 18;
//sensors state (shared between all cores)
int distance_fi = 1000, distance_ri = 1000, distance_li = 1000;  // the distance each ultrasonic reads internally
int distance_f1 = 1000, distance_r1 = 1000, distance_l1 = 1000;  // to avreage the reads
int distance_f2 = 1000, distance_r2 = 1000, distance_l2 = 1000;
int distance_f3 = 1000, distance_r3 = 1000, distance_l3 = 1000;
int distance_f4 = 1000, distance_r4 = 1000, distance_l4 = 1000;
int distance_f5 = 1000, distance_r5 = 1000, distance_l5 = 1000;
int distance_f = 1000, distance_r = 1000, distance_l = 1000;  // final reads
int ir_fr = 1, ir_fl = 1, ir_br = 1, ir_bl = 1;               // ir is 1 when not detecting and 0 when detecting (inverted 🙃)
//sets how far the ultrasonic can detect in CM (bigger is better but may give false detections)
const int range = 100;
//sets a head to head detection range (smaller is better but may not give any detections)
const int head_range = 10;
//sets the speed of the motor
const int full = 255, half = 127;
//sets the time to give up and get away (x10 mille second)
const int determination_time = 2000;
int determination_counter = determination_time;  //fills you with determination (undertale 😂) (sorry if you do not know what is undertale but i had to do it 😅)
//sets the time before returning to normal code when the ir detect an edge (x10 mille second)
const int cooldown = 50;
int cooldown_counter = cooldown;
//sets the time of running away (x10 mille second)
const int run_away = 200;
int run_away_counter = run_away;
//sets braking time in mille second
const int brake_time = 50;
//sets forward time before searching (x10 mille second)
int forward_search_time = 20;
//sets searching time (x10 mille second)
int search_time = 100;
//sets start delay to go acording to the rules in mille second
const int time_to_go = 5000;
//to know if braking is needed
boolean was_forward = false;
//modes
//inverted arena mode to test the robot without the arena
boolean inverted_arena = false;
//task handle for core 0 (core 1 does not need one because it runs the loop function)
TaskHandle_t Task1;
//method for ultrasonic sensor
void read_ultrasonic(int trigPin, int echoPin, int& distance) {  //(int&) means that this value will be changed for all the code (not internal value)
  digitalWrite(trigPin, LOW);                                    //ensure it was not trigered before
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);  //triger
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //wait for the sensor to get ready
  unsigned long timeout = micros();
  while (digitalRead(echoPin) == LOW) {
    if (micros() - timeout > 2000) {  // Timeout after 2000 microseconds
      distance = 1000;                // Set a default value if sensor doesn't respond
      return;
    }
  }
  //wait for the echo of the sensor
  unsigned long startTime = micros();
  timeout = micros();
  while (digitalRead(echoPin) == HIGH) {
    if (micros() - timeout > 10000) {  // Timeout after 10 milliseconds
      distance = 1000;                 // Set a default value if no echo received
      return;
    }
  }
  unsigned long travelTime = micros() - startTime;
  distance = travelTime * 0.034 / 2;  //calculate distance using time and the constant speed of sound
}
//methods to control motors directions (each relay on the module is ON when LOW and OFF when HIGH (inverted 🙃))
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
//attack code to allow a smarter actions when in fight
void attack() {
  while (determination_counter >= 0) {
    if (determination_counter == 0) {
      determination_counter = determination_time;  //fills you with determination (undertale 😂)
      run_away_counter = run_away; //renews the counter
      if (distance_r >= distance_l) {
        while (run_away_counter > 0) {
          if (ir_br == 1 && ir_bl == 1 && distance_f < range) {
            backwardright(full);
            run_away_counter--;
            delay(10);
          } else {
            break;
          }
        }
      } else if (distance_r < distance_l) {
        while (run_away_counter > 0) {
          if (ir_br == 1 && ir_bl == 1 && distance_f < range) {
            backwardleft(full);
            run_away_counter--;
            delay(10);
          } else {
            break;
          }
        }
      }
    } else if (distance_f < head_range || distance_r < head_range || distance_l < head_range) {
      forward(full);
      determination_counter--;
      delay(10);
    } else {
      break;
    }
  }
}
//first code to run when esp32 powers up
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
  //set pins for motor driver
  ledcSetup(driver_speed, 1000, 8);
  ledcAttachPin(driver_pin, driver_speed);
  //set pins for relay module
  pinMode(motor_fr, OUTPUT);
  pinMode(motor_fl, OUTPUT);
  pinMode(motor_br, OUTPUT);
  pinMode(motor_bl, OUTPUT);
  //ensure the robot start in a stop state
  stop();
  //set up the function that runs in core 0 (core 1 does not need one because it runs the loop function)
  xTaskCreatePinnedToCore(codeForTask1, "task_1", 1000, NULL, 1, &Task1, 0);
  delay(500);
  //rules start delay
  if (!inverted_arena) {
    delay(time_to_go - 500);
  }
  //first scenario to run (because at this point we know about the oppoent whereabouts)
  while (forward_search_time > 0) {
    if (ir_fr == 1 && ir_fl == 1) {
      forward(full);
      forward_search_time--;
      delay(10);
    } else {
      break;
    }
  }
  if (distance_f > range && distance_r > range && distance_l > range) {
    stop();
  }
  while (search_time > 0) {
    if (distance_f > range && distance_r > range && distance_l > range) {
      turnright(full);
      search_time--;
      delay(10);
    } else {
      break;
    }
  }
}
//core 0 code
void codeForTask1(void* parameter) {
  for (;;) {
    read_ultrasonic(trig_f, echo_f, distance_fi);
    read_ultrasonic(trig_r, echo_r, distance_ri);
    read_ultrasonic(trig_l, echo_l, distance_li);

    ir_fr = digitalRead(ir_frs);
    ir_fl = digitalRead(ir_fls);
    ir_br = digitalRead(ir_brs);
    ir_bl = digitalRead(ir_bls);

    distance_f1 = distance_f2;  // to avreage the reads
    distance_f2 = distance_f3;
    distance_f3 = distance_f4;
    distance_f4 = distance_f5;
    distance_f5 = distance_fi;
    distance_f = (distance_f1 + distance_f2 + distance_f3 + distance_f4 + distance_f5) / 5;

    distance_r1 = distance_r2;  // to avreage the reads
    distance_r2 = distance_r3;
    distance_r3 = distance_r4;
    distance_r4 = distance_r5;
    distance_r5 = distance_ri;
    distance_r = (distance_r1 + distance_r2 + distance_r3 + distance_r4 + distance_r5) / 5;

    distance_l1 = distance_l2;  // to avreage the reads
    distance_l2 = distance_l3;
    distance_l3 = distance_l4;
    distance_l4 = distance_l5;
    distance_l5 = distance_li;
    distance_l = (distance_l1 + distance_l2 + distance_l3 + distance_l4 + distance_l5) / 5;
    //to see if the readings is right (remove if not needed)
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
    delay(5);
  }
}
//core 1 code
void loop() {
  //inverted arena mode to test the robot without the arena by inverting the ir values (which is inverted to begain with 🥲)
  if (inverted_arena) {
    ir_fr ^= 1;  // (^=) is an XOR operator so if was 1 will be 0 and if 0 then 1
    ir_fl ^= 1;
    ir_br ^= 1;
    ir_bl ^= 1;
  }
  //the main code for core 1
  cooldown_counter = cooldown;                                                          //renews the counter
  determination_counter = determination_time;                                           //fills you with determination (undertale 😂)
  if (distance_f < head_range || distance_r < head_range || distance_l < head_range) {  // (Y.O.L.O.)ing
    attack();
  } else if (ir_fr == 0 && ir_fl == 1 && ir_br == 1) {  // detect the edge on the right front
    stop();
    while (cooldown_counter > 0) {
      if (ir_fl == 1 && distance_f > (range / 4) && distance_r > (range / 4) && distance_l > (range / 4)) {
        turnleft(full);
        cooldown_counter--;
        delay(10);
      } else {
        break;
      }
    }
  } else if (ir_fr == 0 && ir_fl == 1 && ir_br == 0) {  // detect the edge on the right side
    while (cooldown_counter > 0) {
      if (ir_fr == 0 && ir_fl == 1) {
        turnleft(full);
        cooldown_counter--;
        delay(10);
      } else {
        break;
      }
    }
  } else if (ir_fr == 1 && ir_fl == 0 && ir_bl == 1) {  // detect the edge on the left front
    stop();
    while (cooldown_counter > 0) {
      if (ir_fr == 1 && distance_f > (range / 4) && distance_r > (range / 4) && distance_l > (range / 4)) {
        turnright(full);
        cooldown_counter--;
        delay(10);
      } else {
        break;
      }
    }
  } else if (ir_fr == 1 && ir_fl == 0 && ir_bl == 0) {  // detect the edge on the left side
    while (cooldown_counter > 0) {
      if (ir_fr == 1 && ir_fl == 0) {
        turnright(full);
        cooldown_counter--;
        delay(10);
      } else {
        break;
      }
    }
  } else if (ir_fr == 0 && ir_fl == 0) {  // detect the edge on the front
    while (cooldown_counter > (cooldown / 2)) {
      if (distance_f > (range / 4) && distance_r > (range / 4) && distance_l > (range / 4)) {
        backward(full);
        cooldown_counter--;
        delay(10);
      } else {
        break;
      }
    }
    while (cooldown_counter > 0) {
      if (ir_fr == 1 && ir_fl == 1 && ir_br == 1 && ir_bl == 1 && distance_f > range && distance_r > range && distance_l > range) {
        turnleft(full);
        cooldown_counter--;
        delay(10);
      } else {
        break;
      }
    }
  } else if (ir_fr == 1 && ir_fl == 1 && (ir_br == 0 || ir_bl == 0)) {  // detect the edge from behind
    while (cooldown_counter > 0) {
      if (ir_fr == 1 && ir_fl == 1 && distance_f > range && distance_r > range && distance_l > range) {
        forward(full);
        cooldown_counter--;
        delay(10);
      } else {
        break;
      }
    }
  } else if (distance_f < range && ir_fr == 1 && ir_fl == 1) {  // detect the opponent on the front
    forward(full);
  } else if (distance_r < range && distance_l > range && distance_f > range && ir_fr == 1) {  // detect the opponent on the right front
    forwardright(full);
  } else if (distance_r > range && distance_l < range && distance_f > range && ir_fl == 1) {  // detect the opponent on the left front
    forwardleft(full);
  } else if (distance_r < range && distance_l < range && ir_fr == 1 && ir_fl == 1) {  // call an ambulance !  call an ambulance ! ... but not for me 😂
    forward(full);
  } else {  // i see no evil there is no evil 😣
    forward(half);
  }
}