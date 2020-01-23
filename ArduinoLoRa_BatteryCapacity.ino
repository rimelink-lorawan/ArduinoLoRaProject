#include <lora.h>

lora LoRa;
const int ledPin = 13;  // choose the pin for the LED

#define DEBUG    0

#if DEBUG
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(8, 9);  // 8=RX, 9=TX
#endif

/* results are Vcc * 100, So for example, 3V would be 30 */
int getVccVoltage() 
{
  /* Adjust this value to your board's specific internal BG voltage */
  static const long InternalReferenceVoltage = 1115;  // 1.115V measured on the AREF pin 

 /* REFS0: Selects AVcc external reference
  * MUX3 MUX2 MUX1: Selects 1.1V (VBG)
  */
  bitSet(ADMUX, REFS0);
  bitSet(ADMUX, MUX3);
  bitSet(ADMUX, MUX2);
  bitSet(ADMUX, MUX1);

 /* START and WAIT for ADC conversion to complete
  *  
  * ADC start conversion when set the ADSC to 1
  * ADSC will read as 1 as long as a conversion in progress
  * when the conversion is complete, the ADSC returns to 0
  * 
  * add dead time to avoid ENDLESS LOOP if the ADC failed
  * ADC conversion is 115us in normal, so set MAX is 200us
  */
  unsigned long duration = 0;
  unsigned long microseconds = micros();
  bitSet(ADCSRA, ADSC);  // start conversion
  while(bitRead(ADCSRA, ADSC) && (duration < 200))  
  {
    duration = micros() - microseconds;
  }
  int results = (((InternalReferenceVoltage * 1024) / ADC) + 5) / 10; 

  return results;
}

/* argument are Vcc * 100, So for example, 3V would be 300 */
int getBatteryCapacity(int VccVoltage)
{
  /* Adjust this value to your board's specific battery voltage */
  #define BATTERY_MAX_VOLTAGE    300  // 3.0V
  #define BATTERY_MIN_VOLTAGE    180  // 1.8V

  int capacity;

  capacity = (VccVoltage - BATTERY_MIN_VOLTAGE) * 100L / (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE);
  capacity = constrain(capacity, 0, 100);  // constrained between 0 and 100

  return capacity;
}

void setup() 
{
  pinMode(ledPin, OUTPUT);  // declare LED as output
  Serial.begin(115200);  // for LoRa Node
#if DEBUG
  debugSerial.begin(115200); // start software serial port at 115200-8-N-1
#endif  
}

void loop()
{
  int voltage, capacity;
  int array[2];  // voltage, capacity

  voltage = getVccVoltage();
  capacity = getBatteryCapacity(voltage);

#if DEBUG
  debugSerial.print("Vcc: ");
  debugSerial.print(voltage);
  debugSerial.print("V, Battery Capacity: ");
  debugSerial.print(capacity);
  debugSerial.println("%");
#endif  

  array[0] = voltage;
  array[1] = capacity;
  LoRa.write(array, sizeof(array));

  digitalWrite(ledPin, HIGH);  // indicate that have send a LoRa message
  delay(100);
  digitalWrite(ledPin, LOW);

  delay(60000);  // 60 seconds
}
