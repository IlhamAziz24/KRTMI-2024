#include <ESP32Servo.h>
#include <Ps3Controller.h>

//-------------------   SERVO   ----------------------
#include <AX12A.h>
#define DirectionPin  (10u)
#define BaudRate    (1000000ul)
#define ID    12

Servo pgt;
Servo jpt;

//-------------------   MOTOR DC  ------------------
#define ENA 33
#define RM1 27
#define RM2 14
#define LM1 26
#define LM2 25
#define ENB 12

const int motorFreq = 5000;
const int motorResolution = 8;

int rightX = 0;
int leftY = 0;
int sp = 100; // kecepatan awal

const int motorAChannel = 4;
const int motorBChannel = 5;

void notify() {
  if ( Ps3.data.button.start )
  {
    pgt.write(140);
    jpt.write(50);
  }

  if ( Ps3.data.button.l2 ) {
    pgt.write(65);
    Serial.println("PENCET L2");
  }
  if ( Ps3.data.button.r2 ) {
    jpt.write(175);
    Serial.println("PENCET R2");
  }
  if ( Ps3.data.button.l1 ) {
    pgt.write(30);
    Serial.println("PENCET L1");
  }
  if ( Ps3.data.button.r1 ) {
    jpt.write(140);
    Serial.println("PENCET R1");
  }
  if (Ps3.data.button.triangle) {
    sp = 150;
  }
  if ( Ps3.data.button.cross) {
    sp = 70;
  }

  


  rightX = (Ps3.data.analog.stick.rx);
  leftY = (Ps3.data.analog.stick.ly);
  if (leftY > 0) {
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    analogWrite(ENA, sp);
    analogWrite(ENB, sp);
  } else if (leftY < 0) {
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    analogWrite(ENA, sp);
    analogWrite(ENB, sp);
  } else if (rightX > 0) {
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    analogWrite(ENA, sp);
    analogWrite(ENB, sp);
  } else if (rightX < 0) {
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    analogWrite(ENA, sp);
    analogWrite(ENB, sp);
  } else {
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }
  
}

void onConnect() {
  Serial.println("Connected.");
}

void setup() {
  Serial.begin(115200);
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin();
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pgt.attach(32);
  jpt.attach(13);

  jpt.write(180);
  pgt.write(30);

  ax12a.begin(BaudRate, DirectionPin, &Serial2);
  ax12a.setEndless(ID, ON);


  //Set channel Parameters for each motor
  ledcSetup(motorAChannel, motorFreq, motorResolution);
  ledcSetup(motorBChannel, motorFreq, motorResolution);

  // Attach Motor PWM pins to corresponding channels
  ledcAttachPin(ENA, motorAChannel);
  ledcAttachPin(ENB, motorBChannel);

  // Print to Serial Monitor
  Serial.println("Ready.");
}

void loop() {
  if ( Ps3.data.button.up )
  {
    ax12a.ledStatus(ID, OFF);
    ax12a.turn(ID, RIGHT, 500);
    Serial.println("up");
  }

  if ( Ps3.data.button.down )
   {
    ax12a.ledStatus(ID, ON);
    ax12a.turn(ID, LEFT, 500);
    Serial.println("down");
   }
   else{
    ax12a.turn(ID, LEFT, 0);
  }
}
