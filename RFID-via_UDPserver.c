#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const char* ssid = "realme";
const char* password = "password";
//const char* udpServerIP = "192.168.150.194";
const char* udpServerIP = "192.168.150.195";
const int udpServerPort = 1943;
//const int udpServerPort = 1946;  // The port the UDP server is listening on
WiFiUDP udp;
String S1="";
void setup() {
  Serial.begin(9600);
  delay(10);
  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Initialize UDP
  udp.begin(udpServerPort);
}
void loop()
{
  udp.beginPacket(udpServerIP, udpServerPort);
  if (Serial.available())
  {
    S1 = Serial.readStringUntil('\n');
    Serial.println(S1);
    if(S1!=" " && S1!="\n")
    {
      udp.write(S1.c_str());
      udp.endPacket();
    }
    //Serial.println("Message sent via UDP: ");
     delay(5000);  // Send every 5 seconds
  }
}