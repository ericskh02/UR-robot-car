#include <NewPing.h>

//Ultrasonic Sensor Setup
#define TRIGGER_PIN_1 2
#define ECHO_PIN_1 3
#define TRIGGER_PIN_2 4
#define ECHO_PIN_2 5
#define TRIGGER_PIN_3 12
#define ECHO_PIN_3 A1
#define MAX_DISTANCE 200

NewPing front_ultrasonic(TRIGGER_PIN_1, ECHO_PIN_1,MAX_DISTANCE);
NewPing left_ultrasonic(TRIGGER_PIN_2, ECHO_PIN_2,MAX_DISTANCE);


double duration_1;
unsigned int left_distance = 0;
unsigned int front_distance = 0;
int right_distance = 0;
int front_defined_distance = 15;
int left_defined_distance = 20;
int right_defined_distance = 20;
int tooclose_defined_distance = 1;


//Motor Setup
#define motor2_in2 6
#define motor2_in1 9
#define motor1_in1 10
#define motor1_in2 11


int defined_speed = 100; // For bluetooth control
int turn_defined_speed = 50;
int forward_defined_speed = 100;
int left_defined_speed = 35;
int right_defined_speed = 50;
int backward_defined_speed = 100;


///////////////////////Movement of Robot///////////////////////
void brake() {
  analogWrite(motor1_in1, 255);
  analogWrite(motor1_in2, 255);
  analogWrite(motor2_in2, 255);
  analogWrite(motor2_in1, 255);
}

// the connection of the motors is not well set uo and may need further re-soldering
void moveForward(int speed) {
  analogWrite(motor1_in1, speed);
  analogWrite(motor1_in2, 0);
  analogWrite(motor2_in2, speed);
  analogWrite(motor2_in1, 0);
}

void turnLeft(int speed) {
  analogWrite(motor1_in1, 0);
  analogWrite(motor1_in2, speed);
  analogWrite(motor2_in2, speed);
  analogWrite(motor2_in1, 0);
}

void turnRight(int speed) {
  analogWrite(motor1_in1, speed);
  analogWrite(motor1_in2, 0);
  analogWrite(motor2_in2, 0);
  analogWrite(motor2_in1, speed);
}

void moveBackward(int speed) {
  analogWrite(motor1_in1, 0);
  analogWrite(motor1_in2, speed);
  analogWrite(motor2_in2, 0);
  analogWrite(motor2_in1, speed);
}

void deviation_to_right(int speed){
  analogWrite(motor1_in1, speed-30);
  analogWrite(motor1_in2, 0);
  analogWrite(motor2_in2, speed);
  analogWrite(motor2_in1, 0);
}

void deviation_to_left(int speed){
  analogWrite(motor1_in1, speed);
  analogWrite(motor1_in2, 0);
  analogWrite(motor2_in2, speed-30);
  analogWrite(motor2_in1, 0);
}
///////////////////////Movement of Robot///////////////////////




void check_distance(){
  digitalWrite(TRIGGER_PIN_3, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIGGER_PIN_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_3, LOW);
  delayMicroseconds(3);
  duration_1 = pulseIn(A1, HIGH);
  right_distance = (duration_1) * 0.034 / 2;
  front_distance = front_ultrasonic.ping_cm();
  left_distance = left_ultrasonic.ping_cm();
  Serial.print("Left: ");
  Serial.println(left_distance);
  Serial.print("  Front: ");
  Serial.println(front_distance);
  Serial.print("  Right: ");
  Serial.println(right_distance);
}

void validate_distance(){
  
}
void movement(){
  if (front_distance < front_defined_distance){
    brake();
    if(left_distance < left_defined_distance){
      turnRight(25);
    }else{
      turnLeft(25);
    }
  }else{
    if((right_distance - left_distance) > tooclose_defined_distance){
      deviation_to_left(100);
    }else if((left_distance - right_distance) > tooclose_defined_distance){
      deviation_to_right(100);
    }else{
      moveForward(100);
    }
  }
}

void setup() {
  Serial.begin(9600);
  // pinMode of ultrasonic sensors are defined in NewPing library itself
  pinMode(motor1_in1, OUTPUT);
  pinMode(motor1_in2, OUTPUT);
  pinMode(motor2_in2, OUTPUT);
  pinMode(motor2_in1, OUTPUT);
  pinMode(TRIGGER_PIN_3, OUTPUT);
  pinMode(ECHO_PIN_3, INPUT);
}

void loop(){
  check_distance();
  validate_distance();
  movement();
}
