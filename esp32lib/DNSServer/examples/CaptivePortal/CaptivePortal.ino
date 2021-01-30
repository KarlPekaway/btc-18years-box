#include <ESP8266WiFi.h>
#include <DNSServer.h>


#define STATUS_PIN LED_BUILTIN
#define DCF_PIN 2
 
int HIGH_Start = 0;
int HIGH_Ende = 0;
int HIGH_Zeit = 0;
int LOW_Start = 0;
int LOW_Ende = 0;
int LOW_Zeit = 0;
 
bool Signal = false;
bool neueMinute = false;
int BIT = -1;
int ZEIT[65];
int ZEIT_STUNDE;
int ZEIT_MINUTE;
int ZEIT_TAG;
int ZEIT_MONAT;
int ZEIT_JAHR;
int ZEIT_WOCHENTAG;
int PAR_STUNDE;
int PAR_MINUTE;
int PAR_BEGINN;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WiFiServer server(80);

String responseHTML = ""
  "<!DOCTYPE html><html><head><title>Happy Birthday!</title></head><body>"
  "<h1>Hello World!</h1><p> Hier für dich ein Wallet mit bisschen Geld drauf, also vielleicht :D "
  "Puplic Key: sdjfnsiefsdfskjdfsebfisdfj         Private-Key: sdfbshbfsjd sjh </p></body></html>";

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Happy Birthday Gustav");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  server.begin();

    Serial.begin(115200);
  pinMode(DCF_PIN, INPUT);
  pinMode(STATUS_PIN, OUTPUT);
  Serial.println("Syncronisierung");
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
            client.print(responseHTML);
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

   if (BIT > 60) {neueMinute = false;}
  int DCF_SIGNAL = digitalRead(DCF_PIN);
  
  if (DCF_SIGNAL == HIGH && Signal == false) {
    Signal = true; 
    HIGH_Start = millis(); 
    LOW_Ende = HIGH_Start;  
    LOW_Zeit = LOW_Ende - LOW_Start; 
    
    if (neueMinute == true) {
      PrintBeschreibung(BIT);
      //Serial.print("Bit"); 
      //Serial.print (BIT); 
      //Serial.print (": "); 
      ZEIT[BIT] = (BIT_Zeit(LOW_Zeit));
      Serial.print (ZEIT[BIT]);
      //Serial.println ();
    }
    else {
      Serial.print(".");
    }
  }
 
  if (DCF_SIGNAL == LOW && Signal == true) {
    Signal = false; 
    HIGH_Ende = millis();  
    LOW_Start = HIGH_Ende; 
    HIGH_Zeit = HIGH_Ende - HIGH_Start; 
 
    NEUMINUTE(LOW_Zeit);
  }
}



int BIT_Zeit (int LOW_Zeit) {
   if (LOW_Zeit >= 851 && LOW_Zeit <= 950) {return 0;} 
   if (LOW_Zeit >= 750 && LOW_Zeit <= 850) {return 1;}
   if (LOW_Zeit <= 350) {BIT-=1;return 0;}
}

void NEUMINUTE (int LOW_Zeit) {
  if (LOW_Zeit >= 1700) {
    BIT = 0;
    neueMinute = true;
    ZEIT_STUNDE = ZEIT[29]*1+ZEIT[30]*2+ZEIT[31]*4+ZEIT[32]*8+ZEIT[33]*10+ZEIT[34]*20;
    ZEIT_MINUTE = ZEIT[21]*1+ZEIT[22]*2+ZEIT[23]*4+ZEIT[24]*8+ZEIT[25]*10+ZEIT[26]*20+ZEIT[27]*40;
    PAR_STUNDE = ZEIT[35];
    PAR_MINUTE = ZEIT[28];
    ZEIT_TAG = ZEIT[36]*1+ZEIT[37]*2+ZEIT[38]*4+ZEIT[39]*8+ZEIT[40]*10+ZEIT[41]*20;
    ZEIT_MONAT = ZEIT[45]*1+ZEIT[46]*2+ZEIT[47]*4+ZEIT[48]*8+ZEIT[49]*10;
    ZEIT_JAHR = 2000+ZEIT[50]*1+ZEIT[51]*2+ZEIT[52]*4+ZEIT[53]*8+ZEIT[54]*10+ZEIT[55]*20+ZEIT[56]*40+ZEIT[57]*80;
    PAR_BEGINN = ZEIT[20];

    
    
    Serial.println();
    Serial.println("*****************************");
    Serial.print ("Uhrzeit: ");
    Serial.println();
    Serial.print (ZEIT_STUNDE);
    Serial.print (":");
    Serial.print (ZEIT_MINUTE);
    Serial.println();
    Serial.println();
    Serial.print ("Datum: ");
    Serial.println();
    Serial.print (ZEIT_TAG);
    Serial.print (".");
    Serial.print (ZEIT_MONAT);
    Serial.print (".");
    Serial.print (ZEIT_JAHR);
    Serial.println();
    Serial.println("*****************************");


    if (ZEIT_JAHR == 2020 && ZEIT_MONAT == 7 && ZEIT_TAG == 25)
    Serial.println("BOX OFFEN!");
    
  } else {BIT++;}
}
 
void PrintBeschreibung(int BitNummer) {
  switch (BitNummer) {
    case  0: Serial.println("\n# START MINUTE (IMMER 0)"); break;
    case  1: Serial.println("\n# WETTERDATEN"); break;
    case 15: Serial.println("\n# RUFBIT"); break;
    case 16: Serial.println("\n# MEZ/MESZ"); break;
    case 17: Serial.println("\n# MESZ"); break;
    case 18: Serial.println("\n# MEZ"); break;
    case 19: Serial.println("\n# SCHALTSEKUNDE"); break;
    case 20: Serial.println("\n# BEGIN ZEITINFORMATION (IMMER 1)"); break;
    case 21: Serial.println("\n# MINUTEN"); break;
    case 28: Serial.println("\n# PARITAET MINUTE"); break;
    case 29: Serial.println("\n# STUNDE");break;
    case 35: Serial.println("\n# PARITAET STUNDE"); break;
    case 36: Serial.println("\n# TAG"); break;
    case 42: Serial.println("\n# WOCHENTAG"); break;
    case 45: Serial.println("\n# MONAT"); break;
    case 50: Serial.println("\n# JAHR"); break;
    case 58: Serial.println("\n# PARITAET DATUM"); break;
  }
}
