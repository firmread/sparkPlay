#include "application.h"
#include "spark_disable_cloud.h"
//https://community.spark.io/t/bug-bounty-kill-the-cyan-flash-of-death/1322/201
// reference for wlan libs https://github.com/spark/core-common-lib/blob/master/CC3000_Host_Driver/wlan.h

int y = 1;
int incomingByte = 0;
unsigned char ucResults[64];
UINT32* aiIntervalList[16];

int ledPin1 = D0;
int ledPin2 = D1;
int ledPin3 = D2;


tNetappIpconfigRetArgs ipconfig;

void setup() {

    pinMode(ledPin1,OUTPUT);
    pinMode(ledPin2,OUTPUT);
    pinMode(ledPin3,OUTPUT);

    digitalWrite(ledPin1,HIGH);
    digitalWrite(ledPin2,HIGH);
    digitalWrite(ledPin3,HIGH);
    delay(1000);

    digitalWrite(ledPin1,LOW);
    digitalWrite(ledPin2,LOW);
    digitalWrite(ledPin3,LOW);

    Serial.begin(9600);
    // while(!Serial.available());
    Serial1.begin(9600);

    Serial.println(Network.localIP());
    Serial.println(Network.subnetMask());
    Serial.println(Network.gatewayIP());
    Serial.println(Network.SSID());

}

void loop() {

    netapp_ipconfig(&ipconfig);
    char connectedSSID[32];
    sprintf(connectedSSID, "%s", ipconfig.uaSSID);



    digitalWrite(ledPin1,HIGH);
    delay(50);
    digitalWrite(ledPin1,LOW);
    delay(50);

    long errParams = wlan_ioctl_set_scan_params(1000, 20, 30, 2, 0x7ff, -80, 0, 205, aiIntervalList[16]);
    long errResults = wlan_ioctl_get_scan_results(0, ucResults);
    int _numEntry = ((uint8_t) ucResults[3] << 24) | ((uint8_t) ucResults[2] << 16) | ((uint8_t) ucResults[1] << 8) | ((uint8_t) ucResults[0]);

    if (errParams == 0 && errResults == 0 && _numEntry > 0) {


        digitalWrite(ledPin2,HIGH);
        delay(50);
        digitalWrite(ledPin2,LOW);

    	int _stat = ((uint8_t) ucResults[7] << 24) | ((uint8_t) ucResults[6] << 16) | ((uint8_t) ucResults[5] << 8) | ((uint8_t) ucResults[4]);
    	bool _valid = (uint8_t) ucResults[8]  & 0x1;
    	int _rssi = (uint8_t) ucResults[8] >> 1;
    	int _mode = ((uint8_t) ucResults[9] | 0xC0) & 0x3;
    	int _ssidlen = (uint8_t) ucResults[9] >> 2;

    	char ssid[32];
    	int idx = 0;
    	while(strcmp(connectedSSID, ssid) != 0) {
    		ssid[idx] = ucResults[idx+12];
    		idx++;
        	ssid[_ssidlen] = (char) NULL;
    	}

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
        Serial1.println(ssid);

        if (_valid){
    		Serial.print(" is valid, RSSI: ");
        }
    	else
    		Serial.print("not valid, RSSI: ");

        Serial.print(_rssi);
        Serial1.println(_rssi);

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



        // else {
        //     Serial.print(connectedSSID);
        //     Serial.print(" >______< ");
        //     Serial.println(ssid);
        // }

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









