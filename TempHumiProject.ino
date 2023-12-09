#include "Wire.h"
#include "DFRobot_SHT20.h"
#include <lora.h>

lora LoRa;
DFRobot_SHT20    sht20;

#define MIN_PERIOD    10
unsigned int period = 10;

#define DEBUG    0

void setup()
{
    sht20.initSHT20(); // Init SHT20 Sensor
    delay(100);  // for SHT20 initialize itself

    Serial.begin(57600); 
}

void loop()
{
    Sample();

    ParseCmd();
    
#if DEBUG
    Serial.print("period: ");
    Serial.print(period);
    Serial.println(" seconds");
#endif
 
    delay(period * 1000);
}

void Sample() 
{
    float array[2];
  
    array[0] = sht20.readTemperature(); // Read Temperature
    array[1] = sht20.readHumidity(); // Read Humidity
  
#if DEBUG
    Serial.print("Temperature: ");
    Serial.print(array[0]);
    Serial.print("°C, Humidity: ");
    Serial.print(array[1]);
    Serial.println("%");
#else
    LoRa.write(array, sizeof(array));	
#endif
}

void ParseCmd()
{
    #define MAXINSZ    9  // like as: ST:P:xxx
    char instr[MAXINSZ];    // Input buffer
    int  len;
    
    if (LoRa.available() == 0) {
        return;
    }

    len = LoRa.read(instr, MAXINSZ);
    if (('S' == instr[0]) && ('T' == instr[1]) && (':' == instr[2]) && ('P' == instr[3]) && (':' == instr[4])) {  // match the "ST:P:"   
        period = CvtInt(instr, 5, len);
        if (period < MIN_PERIOD) {
            period = MIN_PERIOD;
        }
    }
}

// Convert ASCII digit character to numeric value by subtracting
// 0x30 ('0'), and mult by decade position (i.e. 1, 10, or 100).
// Will not convert numbers larger than 999.
int CvtInt(char *strval, int strt, int slen)
{
    int intval;

    if ((slen - strt) == 1) {
        intval = strval[strt] - '0';
    }
    else if ((slen - strt) == 2) {
        intval = ((strval[strt] - '0') * 10) + (strval[strt+1] - '0');
    }
    else if ((slen - strt) == 3) {
        intval = ((strval[strt] - '0') * 100) + ((strval[strt+1] - '0') * 10) + (strval[strt+2] - '0');
    }
    else
        intval = 0;

    return intval;
}
