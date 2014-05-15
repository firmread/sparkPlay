//SIGNAL-STRENGTH-TCP
#include "application.h"
#include "spark_disable_cloud.h"

TCPServer server = TCPServer(23);
TCPClient client;

int y = 1;
int incomingByte = 0;
unsigned char ucResults[64];

tNetappIpconfigRetArgs ipconfig;

void setup() {


  server.begin();

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
  Serial.println("press 'a' to read Network data\n");

}

void loop() {



  if (client.connected()) {

    digitalWrite(D4,HIGH);
    digitalWrite(D0,HIGH);
    delay(200);
    digitalWrite(D4,LOW);
    digitalWrite(D0,LOW);

    while (client.available()) {

      digitalWrite(D4,HIGH);
      digitalWrite(D7,HIGH);
      delay(200);
      digitalWrite(D4,LOW);
      digitalWrite(D7,LOW);



      Serial.println();
      int val = client.read();
      Serial.println(val);

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
          server.write("WiFi scan status: ");
          switch (_stat) {
            case 0:
            Serial.print("aged, ");
            server.write("aged, ");
            break;
            case 1:
            Serial.print("valid, ");
            server.write("valid, ");
            break;
            case 2:
            Serial.print("no results, ");
            server.write("no results, ");
            break;
          }

          Serial.print(_numEntry);
          Serial.print(" nets found. ");
          Serial.print(ssid);

          server.write(_numEntry);
          server.write(" nets found. ");
          server.write(ssid);

          if (_valid){
            Serial.print(" is valid, RSSI: ");
            server.write(" is valid, RSSI: ");
          }
          else{
            Serial.print("not valid, RSSI: ");
            server.write("not valid, RSSI: ");
          }

          Serial.print(_rssi);
          Serial.print(", mode: ");

          server.write(_rssi);
          server.write(", mode: ");

          switch (_mode) {
            case 0:
            Serial.println("OPEN");
            server.write("OPEN\n");
            break;
            case 1:
            Serial.println("WEP");
            server.write("WEP\n");
            break;
            case 2:
            Serial.println("WPA");
            server.write("WPA\n");
            break;
            case 3:
            Serial.println("WPA2");
            server.write("WPA2\n");
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

        server.write(connectedSSID);
        server.write( " ");
        server.write(ssid);
        server.write("\n");

      } else {
        //     // if no client is yet connected, check for a new connection
        client = server.available();
      }
    }

  }
}
