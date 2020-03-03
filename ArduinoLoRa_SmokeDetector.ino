#include <lora.h>

lora LoRa;

const int buzzerPin = 13;
const int photodiodePin = 0;  // A0
const int smokeThreshold = 80;  // 0 <= no smoke < 80, 80 < thin smoke < 200, 200 < thick smoke <= 1023 

#define DEBUG    0  // 0=disable, 1=enable

#if DEBUG
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(8, 9);  // 8=RX, 9=TX
#endif

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);  // for LoRa Node
#if DEBUG
  debugSerial.begin(115200); // for debug
#endif
}

void loop()
{
  // perfect interval for Arduino watchdog timer is: 15, 30, 60, 120, 250, 500, 1000, 2000, 4000, 8000
  #define FAST_SAMPLE_INTERVAL      120   // 120ms
  #define NORMAL_SAMPLE_INTERVAL    1000  // 1000ms
  #define SAMPLE_NUMBER             16
  
  int val, sum;
  int array[1];
  
  val = analogRead(photodiodePin);
#if DEBUG  
  debugSerial.println(val);
#endif  
  if (smokeThreshold < val)  // sentry observation
  {
    // Check fire danger after sentry alert
    sum = 0;
    for (int count = 0; count < SAMPLE_NUMBER; ++count)
    {
      val = analogRead(photodiodePin);
      sum += val;
      delay(FAST_SAMPLE_INTERVAL);
    }

    val = sum / SAMPLE_NUMBER;  // get the average value
    if (smokeThreshold < val)
    {
      array[0] = val;
      LoRa.write(array, sizeof(array));
      
      digitalWrite(buzzerPin, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);

    #if DEBUG
      debugSerial.print("get fired， val = ");
      debugSerial.println(val);
    #endif
    }
  }
  
  delay(NORMAL_SAMPLE_INTERVAL);
}
