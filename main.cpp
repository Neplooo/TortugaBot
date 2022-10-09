#include <Arduino.h> //Arduino Library
#include <AFMotor.h> //Motor Driver Library
#include <nRF24L01.h>//Radio Module
#include <RF24.h>//Radio Module
#include <SPI.h>

#define CE_PIN 6
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);


int  xAxis, yAxis;

//Defines DC Motors
AF_DCMotor backLeft(1);
AF_DCMotor frontLeft(2);
AF_DCMotor backRight(3);
AF_DCMotor frontRight(4);

//Creates a variable for the speed of the DC Motor
int  speed(int percent)
{
  return map(percent, 0, 100, 0, 255);
}

int xFinal;
int yFinal;

struct myjoystick {

  int xFinal;
  int yFinal;

};

myjoystick joystick;

void setup() {

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, 0xB3B4B5B601);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(124);
  radio.setAutoAck(true);

}

//How The Robot Stops
void stop(){

  backLeft.run(RELEASE);
  backRight.run(RELEASE);
  frontLeft.run(RELEASE);
  frontRight.run(RELEASE);

}

//How The Robot Moves Foward
void forward(){

  backLeft.setSpeed( speed(100) );
  backLeft.run(FORWARD);
  backRight.setSpeed( speed(100) );
  backRight.run(FORWARD);
  frontLeft.setSpeed( speed(100) );
  frontLeft.run(FORWARD);
  frontRight.setSpeed( speed(100) );
  frontRight.run(FORWARD);

}

//How The Robot Moves Backwards
void backwards(){

  backLeft.setSpeed( speed(100) );
  backLeft.run(BACKWARD);
  backRight.setSpeed( speed(100) );
  backRight.run(BACKWARD);
  frontLeft.setSpeed( speed(100) );
  frontLeft.run(BACKWARD);
  frontRight.setSpeed( speed(100) );
  frontRight.run(BACKWARD);

}

//How The Robot Moves Left
void left(){

  backLeft.setSpeed( speed(100) );
  backLeft.run(BACKWARD);
  backRight.setSpeed( speed(100) );
  backRight.run(FORWARD);
  frontLeft.setSpeed( speed(100) );
  frontLeft.run(BACKWARD);
  frontRight.setSpeed( speed(100) );
  frontRight.run(FORWARD);

}

//How The Robot Moves Right
void right(){

  backLeft.setSpeed( speed(100) );
  backLeft.run(FORWARD);
  backRight.setSpeed( speed(100) );
  backRight.run(BACKWARD);
  frontLeft.setSpeed( speed(100) );
  frontLeft.run(FORWARD);
  frontRight.setSpeed( speed(100) );
  frontRight.run(BACKWARD);

}

void loop() {
 // put your main code here, to run repeatedly:
  delay(5);

  radio.startListening();
  if (radio.available())
  {
    int angleY;
    int angleX;
    radio.read(&joystick, sizeof(myjoystick));

    angleX = xFinal;
    angleY = yFinal;

    Serial.println("RX: Received");


    stop();

    if (angleY > 55){

    forward();
    }
    
    else if (angleY < 45){

      backwards();

    }

      
    else{

      stop();

    }


    if (angleX > 55){

    left();
    }
    
    else if (angleX < 45){

      right();

    }

    else{

      stop();

    }

    

  }

  //Debug
  Serial.print(" X = ");
  Serial.print(xFinal);
  Serial.print(" Y = ");  
  Serial.print(yFinal);
  delay(1000);

}

