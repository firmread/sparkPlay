#include "application.h"
#include "spark_disable_cloud.h"
//https://community.spark.io/t/bug-bounty-kill-the-cyan-flash-of-death/1322/201


int y = 1;
int incomingByte = 0;
unsigned char ucResults[64];


tNetappIpconfigRetArgs ipconfig;

void setup() {
    
    pinMode(D7,OUTPUT);
    pinMode(D0,OUTPUT);
    pinMode(D4,OUTPUT);
    
    digitalWrite(D7,HIGH);
    digitalWrite(D4,HIGH);
    digitalWrite(D0,HIGH);
    delay(1000);
    
    digitalWrite(D7,LOW);
    digitalWrite(D4,LOW);
    digitalWrite(D0,LOW);
    
    Serial.begin(115200);
    while(!Serial.available());
    
    // Serial.println(Network.localIP());
    // Serial.println(Network.subnetMask());
    // Serial.println(Network.gatewayIP());
    // Serial.println(Network.SSID());
    Serial.println("press 'a' to read Network data\n");
}

void loop() {
    
    netapp_ipconfig(&ipconfig);
    char connectedSSID[32]; 
    sprintf(connectedSSID, "%s", ipconfig.uaSSID);
    
    
    
    digitalWrite(D7,HIGH);
    delay(500);
    digitalWrite(D7,LOW);
    delay(500);
    
    long err = wlan_ioctl_get_scan_results(0, ucResults);
    int _numEntry = ((uint8_t) ucResults[3] << 24) | ((uint8_t) ucResults[2] << 16) | ((uint8_t) ucResults[1] << 8) | ((uint8_t) ucResults[0]);
    
    if (err == 0 && _numEntry > 0) {
        
        
        digitalWrite(D4,HIGH);
        delay(100);
        digitalWrite(D4,LOW);
        
    	int _stat = ((uint8_t) ucResults[7] << 24) | ((uint8_t) ucResults[6] << 16) | ((uint8_t) ucResults[5] << 8) | ((uint8_t) ucResults[4]);
    	bool _valid = (uint8_t) ucResults[8]  & 0x1;
    	int _rssi = (uint8_t) ucResults[8] >> 1;
    	int _mode = ((uint8_t) ucResults[9] | 0xC0) & 0x3;
    	int _ssidlen = (uint8_t) ucResults[9] >> 2;
    
    	char ssid[32];
    	int idx = 0;
    	while(idx < _ssidlen) { 
    		ssid[idx] = ucResults[idx+12];
    		idx++;
    	}
    	ssid[_ssidlen] = (char) NULL;  
    
        if (strcmp(connectedSSID, ssid) == 0){
            
            digitalWrite(D0,HIGH);
            delay(100);
            digitalWrite(D0,LOW);
            
            
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
        
        incomingByte = Serial.read();
        //key press 'a' to get data
        if (incomingByte == 97){
            Serial.print("local ip   : ");
            Serial.println(Network.localIP());
            Serial.print("subnet mask: ");
            Serial.println(Network.subnetMask());
            Serial.print("gateway ip : ");
            Serial.println(Network.gatewayIP());
            Serial.print("ssid       : ");
            Serial.println(Network.SSID());
            Serial.println();
        }
        
        Serial.print(connectedSSID);
        Serial.print( " ");
        Serial.println(ssid);

        
    }

}
