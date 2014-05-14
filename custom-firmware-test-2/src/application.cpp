#include "application.h"
#include "spark_disable_cloud.h"

//interfacing with Mac via Coolterm
//press "a" to increase A0 output
//press "s" to decrease A0 output


int incomingByte = 0;   // for incoming serial data
int val = 100;


void setup() {

    Serial.begin(9600);   // open serial over USB
    Serial.println("Hello Computer");
    
    pinMode(A0, OUTPUT); // sets the pin as output

}

void loop() {

    // Serial.println("Hello Computer");
    
    // send data only when you receive data:
    if (Serial.available() > 0) {
            // read the incoming byte:
            incomingByte = Serial.read();

            // say what you got:
            Serial.print("I received: ");
            Serial.println(incomingByte, DEC);
            
            if (incomingByte == 97 && val < 255){
                val+=5;
                Serial.print("val = ");
                Serial.println(val, DEC);
            }
            
            if (incomingByte == 115 && val > 0){
                val-=5;
                Serial.print("val = ");
                Serial.println(val, DEC);
            }
            
    }
    
    analogWrite(A0, val);
}
