#include <Ps3Controller.h>
#include <ESP32_Servo.h>

Servo servo1;
Servo servo2;

//Right motor
int enableRightMotor = 33;
int rightMotorPin1 = 25;
int rightMotorPin2 = 26;
//Left motor
int enableLeftMotor = 12;
int leftMotorPin1 = 27;
int leftMotorPin2 = 14;

const int PWMFreq = 5000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

void notify()
{
  //--- Digital cross/square/triangle/circle button events ---

  if ( Ps3.data.button.r1 )
  {
    servo2.write(15);
    Serial.println("Started pressing the square button");
  }

  if ( Ps3.data.button.r2 )
  {
    servo2.write(100);
    Serial.println("Started pressing the circle button");
  }
  if ( Ps3.data.button.l2 )
  {
    servo1.write(45);
    Serial.println("Started pressing the cross button");
  }

  if ( Ps3.data.button.l1 )
  {
    servo1.write(170);
    Serial.println("Started pressing the triangle button");
  }

  if ( Ps3.data.button.start )
  {
    servo1.write(170);
    servo2.write(70);
  }




  int yAxisValue = (Ps3.data.analog.stick.ly); //Left stick  - y axis - forward/backward car movement
  int xAxisValue = (Ps3.data.analog.stick.rx); //Right stick - x axis - left/right car movement

  int throttle = map( yAxisValue, 127, -127, -255, 255);
  int steering = map( xAxisValue, -127, 127, -255, 255);
  int motorDirection = 1;

  if (throttle < 0)       //Move car backward
  {
    motorDirection = -1;
  }

  int rightMotorSpeed, leftMotorSpeed;
  rightMotorSpeed =  abs(throttle) - steering;
  leftMotorSpeed =  abs(throttle) + steering;
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 210);
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 210);

  //Serial.println(rightMotorSpeed);
  //Serial.println(leftMotorSpeed);
  rotateMotor(rightMotorSpeed * motorDirection, leftMotorSpeed * motorDirection);

}

void onConnect()
{
  Serial.println("Connected!.");
}

void onDisConnect()
{
  rotateMotor(0, 0);
  Serial.println("Disconnected!.");
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }
  else
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  }
  else
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }
  ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
  ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));
}

void setUpPinModes()
{
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  //Set up PWM for motor speed
  ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcAttachPin(enableRightMotor, rightMotorPWMSpeedChannel);
  ledcAttachPin(enableLeftMotor, leftMotorPWMSpeedChannel);

  rotateMotor(0, 0);
}


void setup()
{
  setUpPinModes();
  Serial.begin(115200);
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.attachOnDisconnect(onDisConnect);
  Ps3.begin();
  Serial.println("Ready.");

  servo1.attach(18);  //pin servo
  servo2.attach(21);

  servo1.write(170);   //posisi awal
  servo2.write(100);
}

void loop()
{
}
