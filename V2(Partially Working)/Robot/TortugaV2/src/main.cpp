#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <AFMotor.h>

RF24 radio(7, 8);

byte addresses[][6] = {"1Node","2Node"};

//Creates a variable for the speed of the DC Motor
int  speed(int percent)
{
  return map(percent, 0, 100, 0, 255);
}

//Defines DC Motors
AF_DCMotor backLeft(1);
AF_DCMotor frontLeft(2);
AF_DCMotor backRight(3);
AF_DCMotor frontRight(4);


struct data {

  int joystickX;
  int joystickY;

};

data myData;


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

/*

void diaLeftFront(){

  backLeft.setSpeed( speed(100) );
  backLeft.run(FORWARD);
  backRight.setSpeed( speed(100) );
  backRight.run(FORWARD);
  frontLeft.setSpeed( speed(1000) );
  frontLeft.run(FORWARD);
  frontRight.setSpeed( speed(0) );
  frontRight.run(RELEASE);

}

void diaRightFront(){

backLeft.setSpeed( speed(100) );
  backLeft.run(FORWARD);
  backRight.setSpeed( speed(100) );
  backRight.run(BACKWARD);
  frontLeft.setSpeed( speed(0) );
  frontLeft.run(RELEASE);
  frontRight.setSpeed( speed(100) );
  frontRight.run(BACKWARD);

}

void diaLeftBack(){

  backLeft.setSpeed( speed(100) );
  backLeft.run(BACKWARD);
  backRight.setSpeed( speed(100) );
  backRight.run(BACKWARD);
  frontLeft.setSpeed( speed(1000) );
  frontLeft.run(BACKWARD);
  frontRight.setSpeed( speed(0) );
  frontRight.run(RELEASE);

}

void diaRightBack(){

  backLeft.setSpeed( speed(100) );
  backLeft.run(BACKWARD);
  backRight.setSpeed( speed(100) );
  backRight.run(BACKWARD);
  frontLeft.setSpeed( speed(0) );
  frontLeft.run(RELEASE);
  frontRight.setSpeed( speed(100) );
  frontRight.run(BACKWARD);

}

*/

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



void setup() {
  Serial.begin(9600);
  Serial.println("THIS IS THE RECEIVER CODE - YOU NEED THE OTHER ARDUINO TO TRANSMIT");

  // Initiate the radio object
  radio.begin();

  // Set the transmit power to lowest available to prevent power supply related issues
  radio.setPALevel(RF24_PA_LOW);

  // Set the speed of the transmission to the quickest available
  radio.setDataRate(RF24_2MBPS);

  // Use a channel unlikely to be used by Wifi, Microwave ovens etc
  radio.setChannel(124);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);

  // Start the radio listening for data
  radio.startListening();
}


void loop() {


  // Is there any data for us to get?
  if ( radio.available()) {

    // Go and read the data and put it into that variable
    while (radio.available()) {
      radio.read( &myData, sizeof(data) );
    }

    // No more data to get so send it back
    // First, stop listening so we can talk
    radio.stopListening();
    radio.write( &myData, sizeof(data) );


    // Tell the user what we got
    Serial.print("X Val: ");
    Serial.println(myData.joystickX);
    Serial.print("Y Val: ");
    Serial.println(myData.joystickY);
  
    //Turn the values into readable variables

    int xVal = myData.joystickX;
    int yVal = myData.joystickY;

    int yUp = map(yVal, 0, 100, 352, 680);
    int yDown = map(yVal, 0, 100, 348, 0);
    int xRight = map(xVal, 0, 100, 340, 680);
    int xLeft = map(xVal, 0, 100, 336, 0);


    if (yVal > 382){

      backLeft.setSpeed( speed(yUp) );
      backLeft.run(FORWARD);
      backRight.setSpeed( speed(yUp) );
      backRight.run(FORWARD);
      frontLeft.setSpeed( speed(yUp) );
      frontLeft.run(FORWARD);
      frontRight.setSpeed( speed(yUp) );
      frontRight.run(FORWARD);
      Serial.println("Moving Forward");
      delay(0);

    }else if (yVal < 340){

      backLeft.setSpeed( speed(yDown) );
      backLeft.run(BACKWARD);
      backRight.setSpeed( speed(yDown) );
      backRight.run(BACKWARD);
      frontLeft.setSpeed( speed(yDown) );
      frontLeft.run(BACKWARD);
      frontRight.setSpeed( speed(yDown) );
      frontRight.run(BACKWARD);
      Serial.println("Moving Backwards");
      delay(0);

    }else if (xVal > 366){

      backLeft.setSpeed( speed(xRight) );
      backLeft.run(FORWARD);
      backRight.setSpeed( speed(xRight) );
      backRight.run(BACKWARD);
      frontLeft.setSpeed( speed(xRight) );
      frontLeft.run(FORWARD);
      frontRight.setSpeed( speed(xRight) );
      frontRight.run(BACKWARD);
      Serial.println("Moving Right");
      delay(0);

    }else if (xVal < 336){
 
      backLeft.setSpeed( speed(xLeft) );
      backLeft.run(BACKWARD);
      backRight.setSpeed( speed(xLeft) );
      backRight.run(FORWARD);
      frontLeft.setSpeed( speed(xLeft) );
      frontLeft.run(BACKWARD);
      frontRight.setSpeed( speed(xLeft) );
      frontRight.run(FORWARD);
      Serial.println("Moving Left");
      delay(0);

    }else{

      stop();

    }

    // Now, resume listening so we catch the next packets.
    radio.startListening();

    delay(10);

  }


}

