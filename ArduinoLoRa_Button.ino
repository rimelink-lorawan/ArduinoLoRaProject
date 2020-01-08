#include <lora.h>

lora LoRa;
const int inputPin = 2;        // the number of the input pin
const int ledPin = 13;         // the number of the output pin
const int debounceDelay = 50;  // milliseconds to wait until stable

#define DEBUG    0  // 0=disable, 1=enable

#if DEBUG
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(8, 9);  // 8=RX, 9=TX
int count;   // add this variable to store the number of presses
#endif

boolean debounce(int pin)
{
  boolean state;
  boolean previousState;
  previousState = digitalRead(pin);          // store switch state
  for (int counter=0; counter < debounceDelay; counter++)
  {
      delay(1);                  // wait for 1 millisecond
      state = digitalRead(pin);  // read the pin
      if (state != previousState)
      {
         counter = 0; // reset the counter if the state changes
         previousState = state;  // and save the current state
      }
  }
  // here when the switch state has been stable longer than the debounce period
  if (LOW == state)
    return true;
  else
    return false;
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  digitalWrite(inputPin, HIGH);  
  Serial.begin(115200);
#if DEBUG
  debugSerial.begin(9600); // add this to the setup function
#endif
}

void loop()
{
  boolean array[1];
        
  if (debounce(inputPin))
  {
    array[0] = true;
    LoRa.write(array, sizeof(array[0]));

    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
#if DEBUG
    count++;        // increment count
    debugSerial.println(count);  // display the count on the Serial Monitor
#endif
  }
}
      
    
