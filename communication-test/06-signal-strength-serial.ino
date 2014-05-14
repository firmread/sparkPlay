// #include "spark_wiring_network.h"

void setup() {

    Serial.begin(9600);

    delay(1000);

    Serial.println(Network.localIP());
    Serial.println(Network.subnetMask());
    Serial.println(Network.gatewayIP());
    Serial.println(Network.SSID());
}

void loop() {
    
    Serial.print(Network.RSSI());
    Serial.print(" / ");
    Serial.println(Network.SSID());
    delay(1000);
}