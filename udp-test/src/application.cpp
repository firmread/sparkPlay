//UDP-TEST

#include "application.h"
#include "spark_disable_cloud.h"


// local port to listen on
unsigned int localPort = 8888;

const int PACKET_SIZE = 12;
byte  packetBuffer[PACKET_SIZE];

unsigned long lastsec;
int delaysec = 100;

// An UDP instance to let us send and receive packets over UDP
UDP Udp;

void setup()
{

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


  // start the UDP
  Udp.begin(localPort);
  Serial.begin(9600);


  Serial.println(Network.localIP());
  Serial.println(Network.subnetMask());
  Serial.println(Network.gatewayIP());
  Serial.println(Network.SSID());
}

void loop()
{

    Serial.println("i saw sparks");


    digitalWrite(D7,HIGH);
    delay(500);
    digitalWrite(D7,LOW);
    delay(500);


    unsigned long nowsec = millis();
    if (nowsec >= (lastsec + delaysec)) {
        lastsec = nowsec;

        digitalWrite(D4,HIGH);
        delay(100);
        digitalWrite(D4,LOW);

        if (int nbytes = Udp.parsePacket()) {

            digitalWrite(D0,HIGH);
            delay(100);
            digitalWrite(D0,LOW);

            Udp.read(packetBuffer,nbytes);
            for(int i=0;i<nbytes;i++) {
                char c = (char)packetBuffer[i];
                Serial.print(c);
                // Serial.print(c>>4,HEX);
                // Serial.print(c&0x0f,HEX);
            }
            Serial.println();
            Serial.println(Udp.remoteIP());
        }
        Udp.stop();
        delay(1);
        Udp.begin(localPort);

    }

}
