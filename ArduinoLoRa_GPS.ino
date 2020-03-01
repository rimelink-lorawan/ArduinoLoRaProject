#include <lora.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define GPS_RX_PIN  6  // Arduino_PIN_6 <-> GPS_TX
#define GPS_TX_PIN  7  // Arduino_PIN_7 <-> GPS_RX

lora LoRa;
TinyGPSPlus GPS;
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);


void setup()
{
  Serial.begin(115200);  // for LoRa Node
  gpsSerial.begin(9600);  // default Baud Rate of NEO-6M GPS module is 9600 
}

void loop()
{
  double latitude, longitude;
  double array[2];  // latitude, longitude
  
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
  {
    GPS.encode(gpsSerial.read());
    if (GPS.location.isUpdated())
    {
      latitude = GPS.location.lat();
      longitude = GPS.location.lng();

      array[0] = latitude;
      array[1] = longitude;
      LoRa.write(array, sizeof(array));
    
      delay(10000);  // 10 seconds        
    }//if
  }//while
}
