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
//   Serial.print(" ");
  if (client.connected()) {
    // echo all available bytes back to the client
    while (client.available()) {
        
      Serial.println();
      int val = client.read();   
      Serial.println(val);  
      
      
      char strength[10];
      sprintf(strength, "%i", Network.RSSI());
    //   int streLen = strlen(strength);
      
      char ssid[30];
      sprintf(ssid, "&i", Network.SSID());
      
      
      Serial.print(strength);
    //   Serial.print(" / ");
    //   Serial.print(streLen);
      Serial.print(" / ");
    //   Serial.println(Network.SSID());
      Serial.println(ssid);
      
      
      //only write once
      server.write(strength);
      server.write(" / ");
    //   server.write(Network.SSID());
      server.write(ssid);
      server.write("\n");
    //   delay(5000);
    }
  } else {
//     // if no client is yet connected, check for a new connection
    client = server.available();
  }
}