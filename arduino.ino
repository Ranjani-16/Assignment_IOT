#include <ACS712.h>
#include <SoftwareSerial.h>
char watt[5];
SoftwareSerial s(5,6);//to connect with Node MCU
ACS712 sensor(ACS712_30A,A0); 
unsigned long last_time =0;
unsigned long current_time =0;
float Wh =0 ;  
void setup() {
s.begin(115200);
sensor.calibrate();
}
 
void loop() {
float V= 230;
  float I = sensor.getCurrentAC();//to get the consumed power
  float P = V * I;
  last_time = current_time;
  current_time = millis();    
  Wh = Wh+  P *(( current_time -last_time) /3600000.0) ; //calculating the watt
  //Serial.println(Wh);
if(s.available()>0)
{
  char c=s.read();
  if(c=='s'){
 s.write(Wh);
 delay(3000);}
}
}
