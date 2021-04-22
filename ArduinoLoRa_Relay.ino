#include <lora.h>

lora LoRa;
const int wakeupPin = 2;  // LoRaNode LOW wakeup connected to pin 2
const int solenoidPin = 5;  // Solenoid connected to pin 5

#define DEBUG    1

#if DEBUG
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(8, 9);  // 8=RX, 9=TX
#endif

void setup()
{
  Serial.begin(57600);  // for LoRa node
  pinMode(wakeupPin, INPUT);
  pinMode(solenoidPin, OUTPUT);
  
#if DEBUG
  pinMode(LED_BUILTIN, OUTPUT);
  debugSerial.begin(9600); // start software serial port at 9600-8-N-1
  debugSerial.println("debug begin");
#endif

  for (int i = 0; i < 2; ++i) {
    LoRa.write("hi", 2);
    delay(5000);
  }
}

void loop()
{
  #define MAXINSZ    20  // like as: RY:x
  char instr[MAXINSZ];  // Input buffer
  int  len;
  int  val;

  do {
    val = digitalRead(wakeupPin);
  } while (HIGH == val);  // check looply until the pin is LOW

  delay(10);  // waiting until the LoRa message have been received

  if (LoRa.available()) {
    len = LoRa.read(instr, MAXINSZ);
    if (('R' == instr[0]) && ('Y' == instr[1]) && (':' == instr[2]))  // match the "RY:0" or "RY:1"
    {      
      if ('1' == instr[3])  // turn ON
      {
        digitalWrite(solenoidPin, HIGH); // activates the solenoid
      }
      else if ('0' == instr[3])  // turn OFF
      {
        digitalWrite(solenoidPin, LOW);  // deactivates the solenoid
      }
      else  // do nothing for the noise
      {
      }
    }

  #if DEBUG
    digitalWrite(LED_BUILTIN, HIGH);
    debugSerial.write(instr, len);
    digitalWrite(LED_BUILTIN, LOW);
  #endif
  }
}
