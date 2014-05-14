//interfacing with Mac via Coolterm
//take keyboard input to change value,,,


int incomingByte = 0;   // for incoming serial data
int val = 0;

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
            val = incomingByte;
    }
    
    analogWrite(A0, val);
}