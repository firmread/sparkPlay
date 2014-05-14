// telnet defaults to port 23

TCPServer server = TCPServer(23);
TCPClient client;

void setup()
{
    // start listening for clients
    server.begin();

    Serial.begin(9600);

    delay(1000);

    Serial.println(Network.localIP());
    Serial.println(Network.subnetMask());
    Serial.println(Network.gatewayIP());
    Serial.println(Network.SSID());
}

void loop()
{
  Serial.println("I'm not blocked!!!");
  if (client.connected()) {
    // echo all available bytes back to the client
    while (client.available()) {
        
      int val = client.read();        
      server.write(val);
      Serial.println(val);
    }
  } else {
    // if no client is yet connected, check for a new connection
    client = server.available();
  }
}