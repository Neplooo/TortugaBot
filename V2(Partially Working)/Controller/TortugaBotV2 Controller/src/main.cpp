#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);

byte addresses[][6] = {"1Node","2Node"};

const int joystickReadX = A0;
const int joystickReadY = A1;

struct data {

  int joystickX;
  int joystickY;

};

data myData;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  Serial.println("Transmitter Begin");

  radio.begin();

  radio.setPALevel(RF24_PA_LOW);

  radio.setDataRate(RF24_2MBPS);

  radio.setChannel(124);

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

}

void loop() {
  // put your main code here, to run repeatedly:

  myData.joystickX = analogRead(joystickReadX);
  myData.joystickY = analogRead(joystickReadY);

  radio.stopListening();

  if(!radio.write( &myData, sizeof(myData))){
    Serial.println("No ack");
  }

  radio.startListening();

  unsigned long startedWaitingAt = millis();

  while(! radio.available()) {

    if (millis() - startedWaitingAt > 200 ) {
      Serial.println("No response");
      return;
    }

  }

  unsigned char dataRx;
  radio.read( &dataRx, sizeof(unsigned char) );


  Serial.print("Sent: ");
  Serial.print(myData.joystickX);
  Serial.print("\t");
  Serial.print(myData.joystickY);
  Serial.print(", received: ");
  Serial.println(dataRx);

  delay(10);

}