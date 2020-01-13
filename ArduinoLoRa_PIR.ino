/*
   PIR sketch
   a Passive Infrared motion sensor connected to pin 2
   lights the LED on pin 13
*/
#include <lora.h>

lora LoRa;

const int ledPin = 13;            // choose the pin for the LED
const int inputPin = 2;           // choose the input pin (for the PIR sensor)

void setup() 
{
  pinMode(ledPin, OUTPUT);         // declare LED as output
  pinMode(inputPin, INPUT);        // declare pushbutton as input
  Serial.begin(115200);            // for LoRa Node
}

void loop()
{
  boolean array[1];
    
  int val = digitalRead(inputPin);  // read input value
  if (HIGH == val)                  // check if the input is HIGH
  {
    array[0] = true;
    LoRa.write(array, sizeof(array[0]));  // report to LoRa Server that a motion detected

    digitalWrite(ledPin, HIGH);     // turn LED on if motion detected
    delay(3000);
    digitalWrite(ledPin, LOW);      // turn LED off
  }
}
