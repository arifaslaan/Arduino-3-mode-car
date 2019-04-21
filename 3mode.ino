#include <Servo.h>
#include <NewPing.h>
#define IN1 10 //SOL GERİ
#define IN2 9 //SOL İLERİ
#define IN3 5 //SAG İLERİ
#define IN4 6 //SAG GERİ
#define L_LEFT 2
#define L_CENTER 3
#define L_RIGHT 4
#define U_TRIG 7
#define U_ECHO 8
#define SERVO 11
int carStt = 0;
int bluStt = 0;
int myStt = 0;
double distance = 0;
double distanceL = 0;
double distanceR = 0;

NewPing Ultrasonic(U_TRIG, U_ECHO, 300);
Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO);
  myServo.write(90);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  while(myStt==0){
    carStt=Serial.read();
    if(carStt=='W' || carStt=='U' || carStt=='V'){
      myStt = 1;
    }
  }
}

void loop() {
  if(carStt == 'W'){
    lineMode();
  }
  if(carStt == 'U'){
    ultMode();
  }
  if(carStt == 'V'){
    bluMode();
  }
}
void lineMode(){
  if(digitalRead(L_LEFT) == 1 && digitalRead(L_CENTER)== 0 && digitalRead(L_RIGHT)== 0){
    carLeft();
  }
  if(digitalRead(L_LEFT) == 0 && digitalRead(L_CENTER)== 1 && digitalRead(L_RIGHT)== 0){
    carForward();
  }
  if(digitalRead(L_LEFT) == 0 && digitalRead(L_CENTER)== 0 && digitalRead(L_RIGHT)== 1){
    carRight();
  }
  if(digitalRead(L_LEFT) == 1 && digitalRead(L_CENTER)== 1 && digitalRead(L_RIGHT)== 1){
    carStop();
  }
}
void ultMode(){
  distance = Ultrasonic.ping_cm();
  delay(50);
  if(distance >= 20 || distance == 0){
    carForward();
  }
  else{
    carStop();
    myServo.write(20);
    delay(100);
    distanceR = Ultrasonic.ping_cm();
    delay(500);
    myServo.write(160);
    delay(100);
    distanceL = Ultrasonic.ping_cm();
    delay(500);
    if(distanceR > distanceL){
      carRight();
      delay(300);
      myServo.write(90);
      delay(50);
    }
    if(distanceR == distanceL){
      myServo.write(90);
      carBack();
      delay(500);
      carStop();
      myServo.write(20);
      delay(100);
      distanceR = Ultrasonic.ping_cm();
      delay(500);
      myServo.write(160);
      delay(100);
      distanceL = Ultrasonic.ping_cm();
      delay(500);
      if(distanceR > distanceL){
        carRight();
        delay(500);
        myServo.write(90);
        delay(50);
      }
      else{
        carLeft();
        delay(300);
        myServo.write(90);
        delay(50);
      }
    }
    else{
      carLeft();
      delay(300);
      myServo.write(90);
      delay(50);
    }
  }
}
void bluMode(){
  if(Serial.available() > 0){
    bluStt = Serial.read();
    if(bluStt=='F'){
    carForward();
    }
    else if(bluStt=='B'){
      carBack();
    }
    else if(bluStt=='R'){
      carRight();
    }
    else if(bluStt=='L'){
      carLeft();
    }
    else{
      carStop();
    }
  }
}
void carForward(){
  analogWrite(IN1, 0);
  analogWrite(IN2, 255);
  analogWrite(IN3, 255);
  analogWrite(IN4, 0);
}
void carBack(){
  analogWrite(IN1, 255);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 255);
}
void carRight(){
  analogWrite(IN1, 0);
  analogWrite(IN2, 255);
  analogWrite(IN3, 0);
  analogWrite(IN4, 255);
}
void carLeft(){
  analogWrite(IN1, 255);
  analogWrite(IN2, 0);
  analogWrite(IN3, 255);
  analogWrite(IN4, 0);
}
void carStop(){
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

