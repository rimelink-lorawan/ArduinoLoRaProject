#include <lora.h>
#include <Wire.h>              // Wire library for I2C communication
#include <Adafruit_MLX90614.h> // MLX90614 library from Adafruit

lora LoRa;
Adafruit_MLX90614 MLX90614 = Adafruit_MLX90614();

const int buttonPin = 2;
const int speakerPin = 13;
const int debounceDelay = 50;  // milliseconds to wait until stable

#define DEBUG    0  // 0=disable, 1=enable

#if DEBUG
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(8, 9);  // 8=RX, 9=TX
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
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);  // pull up that would read LOW if pressed
  Serial.begin(115200);  // for LoRa Node
  MLX90614.begin();
#if DEBUG
  debugSerial.begin(9600); // add this to the setup function
#endif
}

void loop()
{
  double array[1];
        
  if (debounce(buttonPin))
  {
    array[0] = MLX90614.readObjectTempC();  // get temperature from MLX90614
    LoRa.write(array, sizeof(array[0]));

    digitalWrite(speakerPin, HIGH);
    delay(100);
    digitalWrite(speakerPin, LOW);
#if DEBUG
    debugSerial.println(array[0]);
#endif
  }
}
