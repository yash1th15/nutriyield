
#include <SoftwareSerial.h>
#define SSID "Pocoboi"        // "WiFiname"  replace with your WiFi name
#define PASS "abcdefghijk"      // "Password"  replace with your WiFi password
String msg = "GET /update?key=GER2CHDUNKZVMPZE"; //change it with your channel Write key find this in ThingSpeak
int Time_S = 15;   //Change this to your prefered sampling interval in seconds, exampe 2 min is 120 seconds
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IP  "184.106.153.149" // thingspeak.com
// ip (do not change) 

SoftwareSerial mySerial(2, 3); // RX, TX


int pH = A3;    // select the analog input pin for the pH sensor


float pH_Value = 0;     // variable to store the value coming from pH sensor

float ad7 = 603.0;     // change this value to the one on serial monitor when in pH7 buffer
float ad4 = 703.0;     // change this value to the one on serial monitor when in pH4 buffer

void setup() 
{
  Serial.begin(57600);  // also set your serial monitor to this baud rate
  
  Serial.println("typhoonbox");  // Test ESP8266
  delay(5000);                  
  if(Serial.find("OK"))  
  {
    connectWiFi();
  }
}

void loop() 
{
  
  Get_pH(); 
  Send();
  delay(15*Time_S);   // time to wait before loop repeats
}
///////////////////////////////////////////

void Get_pH()
{
  int sensorValue =0;
  int currentValue = 0;
  
  for (int i = 0; i < 10; i++) 
  {
    currentValue += analogRead(pH);
    delay(100);
  }
sensorValue =(currentValue / 10);
Serial.println(sensorValue);
float m =(-3.0/(ad4-ad7));    
float c = 7-(m*ad7);
float a =((m*sensorValue)+c);
Serial.print("PH = ");
Serial.println(a);
pH_Value = a;  
delay(500); 
}

/////////////////////////////////////////WiFi//////////////////////////////////////////////////
void Send()
{
String cmd = "AT+CIPSTART=\"TCP\",\"";
cmd += "IP";
cmd += "\",80";
Serial.println(cmd);
delay(2500);  
cmd = msg ;
cmd += "&field1=";  //field 
cmd += pH_Value;

cmd += "\r\n";
cmd += "\r\n";
Serial.print("AT+CIPSEND=");
Serial.println(cmd.length());
if(Serial.find(">")){
Serial.print(cmd);
}
 else
 {
  Serial.println("AT+CIPCLOSE");
  }
}
///////////////////////////WiFi connect////////////////////////////////////////
boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")){
    return true;
  }
  else{
    return false;
  }
}
