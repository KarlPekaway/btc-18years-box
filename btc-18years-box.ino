#include <WiFi.h>
#include <DNSServer.h>
// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const byte DNS_PORT = 53;
IPAddress apIP(8,8,4,4); // The default android DNS
DNSServer dnsServer;
WiFiServer server(80);

String rtctime;
bool birthday = false;

//website after date
String responseHTML = ""
  "<!DOCTYPE html><html><head><title><center>TITEL</title></head><body>"
  "<h1><center>congratulations</h1>
  "<p><center>your text here" 
    "<p><center>your text here"   
    "<h1><center>your BTC ADRESS</h1><p><center>"
      "<h1><center>your Private Key</h1><p><center> "
  "<center> best regards </p>";

  //generate for example at https://www.bitaddress.org/

//website time not reached 
String responseHTML2  = ""
  "<!DOCTYPE html><html><head><title><center>WAIT</title></head><body>"
  "<h1><center>your text here</h1><p><center>"
  "<h1><center>your text here</h1><p><center>"
  "<center>best regards</p>";  



void setup() { 


  Serial.begin(57600);
Serial.println("START");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    delay(1000);
    Serial.flush();
    abort();
  }

  rtc.start();
  

  DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.println();
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

  
  rtctime = "<p><center>";
  rtctime += "TIME :";
  rtctime += now.year();
  rtctime += "/";
   rtctime += now.month();
  rtctime += "/";
   rtctime += now.day();
  rtctime += "/";
   rtctime += now.hour();
  rtctime += ":";
   rtctime += now.minute();
   rtctime += "</p></body></html>";


responseHTML += rtctime; 
responseHTML2 += rtctime;

  if(now.year() == 2038 && now.month() == 6  && now.day() == 18)  
  birthday = true; 

  
  if(now.year() > 2038)
  birthday = true; 

  if(now.year() == 2038 && now.month() >= 6 && now.day() >= 18)
  birthday = true; 
  
  
  Serial.print(birthday);
  
  if(birthday == true)
  { 
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Your WIfi Birthday NAME HERE");
  }
  else
  {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Your WAIT Wifi Name");
  }
  
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  
  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);



  server.begin();
  
}

void loop() {
  dnsServer.processNextRequest();
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            if(birthday == true)
            {
            client.print(responseHTML);
            }
            else
            { 
            client.print(responseHTML2);
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}
