#include "application.h"
#include "spark_disable_cloud.h"
//https://community.spark.io/t/bug-bounty-kill-the-cyan-flash-of-death/1322/201
// reference for wlan libs https://github.com/spark/core-common-lib/blob/master/CC3000_Host_Driver/wlan.h



//------------------------------------------

// GLO-216G serial OLED display
// Natalia Fargasch Norman @ theelectronicshobbyist.com/blog

// GLO-216Y/G information: http://www.seetron.com/glo216.html
// Programming reference: http://seetron.com/glo216/glo216prog.html

/*
Parameters for setPosition and rightAlign instructions:
setPosition: position + 0x40
rightAlign: '0'-'7'
*/

/*
Parameter for escape instructions:
Define lower custom character: 'D', '0'-'7'
Define upper custom character: 'd', '0'-'7'
Recall saved text: 'E', '0'
Restore custom character set: 'e', '0'
Save text since last clearScreen: 'X', '0'
Store custom character set: 'x', '0'
*/

// Moves printing position to the first character of top line
char homeCursor() {  return 0x01;}
// Cycles the font size: normal -> tall -> wide -> big -> normal -> ...
char selectSize() {  return 0x02;}
// Sets font size to default of small on two lines of 16 characters
char defaultSize() {  return 0x03;}
// Behaves like the backspace key
char backspace() {  return 0x08;}
// Moves printing position to the next multiple of 4 location
char tab() {  return 0x09;}
// Moves the printing position down a line
char linefeed() {  return 0x0A;}
// Moves the printing position up a line
char verticalTab() {  return 0x0B;}
// Clears the screen and moves printing position to 0
char clearScreen() {  return 0x0C;}
// Moves to the first printing position of the next line
char carriageReturn() {  return 0x0D;}
// Turns on the OLED driver circuitry when it has been previously turned off
char turnOn() {  return 0x0E;}
// Turns off the OLED driver circuitry to save power
char turnOff() {  return 0x0F;}
// Sets the printing position according to value of next byte
char setPosition() {  return 0x10;}
// Prints text at the righthand end of a field of defined size from 2-8
char rightAlign() {  return 0x12;}
// Sets seven segment style font for large characters
char sevenSeg() {  return 0x13;}
// Sets text style font for large characters
char textStyle() {  return 0x14;}
// Begin multi-part instruction
char escape() {  return 0x1B;}
// See http://seetron.com/glo216/bpigloupgg.html
char bpkInstr() {  return 0xFE;}

//------------------------------------------
//character formats

      char instr[] = {
        clearScreen(),
        defaultSize(),
        selectSize(), // tall
        selectSize(), // wide
        selectSize(), // big
        textStyle(),
        NULL
      };

      // defines a new custom wifi shaped character
      char instr1[] = {
        // escape sequence to receive new custom character
        clearScreen(),
        defaultSize(),
        selectSize(), // tall
        selectSize(), // wide
        selectSize(), // big
        escape(), 'D', '0',
        // pattern tool at http://seetron.com/apps/app_cceditor.html
        0x80, 0x8E, 0x91, 0x84, 0x8A, 0x80, 0x84, 0x80,
        
        textStyle(),
        NULL
      };

      char instr2[] = {
        defaultSize(),
        textStyle(),
        NULL
      };


//------------------------------------------

int y = 1;
int incomingByte = 0;
unsigned char ucResults[64];
UINT32* aiIntervalList[16];


int ledPin1 = D0;
int ledPin2 = D1;
int ledPin3 = D2;


tNetappIpconfigRetArgs ipconfig;

void setup() {

    Serial.begin(115200);
    Serial.println("press 'a' to read Network data\n");

    Serial1.begin(9600);

    pinMode(ledPin1,OUTPUT);
    pinMode(ledPin2,OUTPUT);
    pinMode(ledPin3,OUTPUT);

    digitalWrite(ledPin1,HIGH);
    digitalWrite(ledPin2,HIGH);
    digitalWrite(ledPin3,HIGH);

    Serial1.print(instr);
    const char msg[] = "YESYESNO";
    Serial1.print(msg);

    delay(1000);

    digitalWrite(ledPin1,LOW);
    digitalWrite(ledPin2,LOW);
    digitalWrite(ledPin3,LOW);


}

void loop() {

    netapp_ipconfig(&ipconfig);
    char connectedSSID[32];
    sprintf(connectedSSID, "%s", ipconfig.uaSSID);

    digitalWrite(ledPin1,HIGH);
    delay(50);
    digitalWrite(ledPin1,LOW);
    delay(50);

    // should be done once prob...
    long errParams = wlan_ioctl_set_scan_params(100,20,30,2,0x7ff,-80,0,205,aiIntervalList[16]);

    long errResults = wlan_ioctl_get_scan_results(0, ucResults);
    int _numEntry = ((uint8_t) ucResults[3] << 24) | ((uint8_t) ucResults[2] << 16) | ((uint8_t) ucResults[1] << 8) | ((uint8_t) ucResults[0]);

    if (errParams == 0 && errResults == 0 && _numEntry > 0) {


        Serial.println("num entry");
        Serial.println( _numEntry);


        digitalWrite(ledPin2,HIGH);
        delay(50);
        digitalWrite(ledPin2,LOW);

        int _stat = ((uint8_t) ucResults[7] << 24) | ((uint8_t) ucResults[6] << 16) | ((uint8_t) ucResults[5] << 8) | ((uint8_t) ucResults[4]);
        bool _valid = (uint8_t) ucResults[8]  & 0x1;
        int _rssi = (uint8_t) ucResults[8] >> 1;
        int _mode = ((uint8_t) ucResults[9] | 0xC0) & 0x3;
        int _ssidlen = (uint8_t) ucResults[9] >> 2;

        char ssid[32];
        memset(ssid, 0, 32);
        int idx = 0;
        while(idx < _ssidlen) {
            ssid[idx] = ucResults[idx+12];
            idx++;
        }
        ssid[_ssidlen] = (char) NULL;

        // while(strcmp(connectedSSID, ssid) != 0) {
        //     ssid[idx] = ucResults[idx+12];
        //     idx++;
        // }
        // ssid[_ssidlen] = (char) NULL;



        if (strcmp(connectedSSID, ssid) == 0){

            digitalWrite(ledPin3,HIGH);
            delay(100);
            digitalWrite(ledPin3,LOW);


            Serial.print("WiFi scan status: ");
            switch (_stat) {
                case 0:
                    Serial.print("aged, ");
                    break;
                case 1:
                    Serial.print("valid, ");
                    break;
                case 2:
                    Serial.print("no results, ");
                    break;
            }

            Serial.print(_numEntry);
            Serial.print(" nets found. ");
            Serial.print(ssid);

            if (_valid)
                Serial.print(" is valid, RSSI: ");
            else
                Serial.print("not valid, RSSI: ");

            Serial.print(_rssi);

          // print character 0x80, the newly defined heart
          const char msg1[] = { 0x80, NULL};
          Serial1.print(instr1);
          Serial1.print(msg1);


          const char msg2[] = " RSSI ";
          Serial1.print(instr2);
          Serial1.print(msg2);
      


            if (_rssi >= 90){ 
                const char msg3[] = {0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F};
                Serial1.print(msg3);
            }
            else if(90 > _rssi && _rssi >= 80){ 
                const char msg3[] = {0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x20};
                Serial1.print(msg3);
            }
            else if(80 > _rssi && _rssi >= 70){ 
                const char msg3[] = {0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x20,0x20};
                Serial1.print(msg3);
            }
            else if(70 > _rssi && _rssi >= 60){ 
                const char msg3[] = {0x88,0x89,0x8A,0x8B,0x8C,0x20,0x20,0x20};
                Serial1.print(msg3);
            }
            else if(60 > _rssi && _rssi >= 50){ 
                const char msg3[] = {0x88,0x89,0x8A,0x8B,0x20,0x20,0x20,0x20};
                Serial1.print(msg3);
            }
            else if(50 > _rssi && _rssi >= 40){ 
                const char msg3[] = {0x88,0x89,0x8A,0x20,0x20,0x20,0x20,0x20};
                Serial1.print(msg3);
            }
            else if(40 > _rssi && _rssi >= 30){ 
                const char msg3[] = {0x88,0x89,0x20,0x20,0x20,0x20,0x20,0x20};
                Serial1.print(msg3);
            }
            else if(30 > _rssi && _rssi >= 20){ 
                const char msg3[] = {0x88,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
                Serial1.print(msg3);
            }
            else { 
                const char msg3[] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
                Serial1.print(msg3);
            }

            Serial1.print(ssid);
            Serial1.print(" : ");
            Serial1.print(_rssi);
            

            Serial.print(", mode: ");
            switch (_mode) {
                case 0:
                    Serial.println("OPEN");
                    break;
                case 1:
                    Serial.println("WEP");
                    break;
                case 2:
                    Serial.println("WPA");
                    break;
                case 3:
                    Serial.println("WPA2");
                    break;
            }

        }

        else {
            Serial.print(connectedSSID);
            Serial.print(" >______< ");
            Serial.println(ssid);
        }

        incomingByte = Serial.read();
        //key press 'a' to get data
        if (incomingByte == 97){
            Serial.println(Network.localIP());
            Serial.println(Network.subnetMask());
            Serial.println(Network.gatewayIP());
            Serial.println(Network.SSID());
        }


    }
    else{
        Serial.println("oops,,,");
    }

}









