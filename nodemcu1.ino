#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include<SoftwareSerial.h>
 
/* Set these to your desired credentials. */
const char *ssid = "Your username";  //ENTER YOUR WIFI SETTINGS <<<<<<<<<
const char *password = "Your password";
 
//Web address to read from
const char *host = "api.thingspeak.com";
String apiKey = "5KGZZ1E9D4OZVI8H";  //ENTER YOUR API KEY <<<<<<<<<<<
SoftwareSerial s(D6,D5);
float data;
void setup() {
s.begin(115200);
delay(1000);
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  //WiFi.mode(WIFI_AP_STA);   //Both hotspot and client are enabled
  //WiFi.mode(WIFI_AP);       //Only Access point
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");}
 //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
 
void loop() {
  s.write("s");
  if (s.available()>0)
  {
    WiFiClient client;          
  const int httpPort = 80; //Port 80 is commonly used for www
 //---------------------------------------------------------------------
 //Connect to host, host(web site) is define at top 
 if(!client.connect(host, httpPort)){
   Serial.println("Connection Failed");
   delay(300);
   return; //Keep retrying until we get connected
 }
 String ADCData;
   data=s.read(); //Read Analog value of LDR
  ADCData = String(data);   //String to interger conversion
  String Link="GET /update?api_key="+apiKey+"&field1=";  //Requeste webpage  
  Link = Link +  ADCData ;
  Link = Link + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";                
  client.print(Link);
  delay(100);
   // data=s.read();
    //Serial.println(data);
    int timeout=0;
 while((!client.available()) && (timeout < 1000))     //Wait 5 seconds for data
 {
   delay(10);  //Use this with time out
   timeout++;
 }

//---------------------------------------------------------------------
 //If data is available before time out read it.
 if(timeout < 500)
 {
     while(client.available()){
        Serial.println(client.readString()); //Response from ThingSpeak       
     }
 }
 else
 {
     Serial.println("Request timeout..");
 }

 delay(5000);
    //ThingSpeak.writeField(myChannelNumber, 1,data, myWriteAPIKey); //Update in ThingSpeak
  }
 
 
}
