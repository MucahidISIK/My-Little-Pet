#include <ThingSpeak.h>               
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>       // bluetooth için kütüphane
#define sesPin D0

WiFiClient  client;
unsigned long counterChannelNumber = 1260979;    // sıcaklık              
const char * myCounterReadAPIKey = "YB9BTFGX8NE0ZOSK";      
const char * myCounterWriteAPIKey = "54N3PRWPMSFRTXUC";   

unsigned long counterChannelNumberV = 1265311;    // ses              
const char * myCounterReadAPIKeyV = "UG4RORHZZ9TN4086";      
const char * myCounterWriteAPIKeyV = "H2GTP4NL1BMPVGDE";  

const int sicaklikPin = A0;                                   // ESP8266 Analog Pin ADC0 = A0
int sicaklikdegeri = 0;   //Analog değeri dönüştüreceğimiz sıcaklık değeri 
float olculendeger = 0;   //Ölçeceğimiz analog değer

int gurultulu = 1;
int sessiz = 0;

SoftwareSerial bt(D9, D10);           // bluetooth inputları


void setup()
{
  Serial.begin(115200);
  bt.begin(115200);
  Serial.println();
  
  

  pinMode(sicaklikPin, INPUT);
  pinMode(sesPin, INPUT);

  WiFi.begin("ISIK_NET", "eskm5414");                 // write wifi name & password           

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop() 
{
  olculendeger = analogRead(sicaklikPin); //A0'den değeri alacak
  olculendeger = (olculendeger/1024)*5000;//değeri mV'a dönüştürecek 
  sicaklikdegeri = olculendeger /12,0; // mV'u sicakliğa dönüştürecek
  sicaklikdegeri=sicaklikdegeri-10;


  Serial.print(F("\n Sicaklik Degeri : "));
  Serial.println(sicaklikdegeri);

  ThingSpeak.writeField(counterChannelNumber , 1 ,sicaklikdegeri  , myCounterWriteAPIKey);

  int sesValue = digitalRead(sesPin);
  if(sesValue == LOW)
  {  
   // bt.write("123"); 
    Serial.println("Gürültü");
    ThingSpeak.writeField(counterChannelNumberV , 1 ,gurultulu , myCounterWriteAPIKeyV);
  }
  else if (sesValue == HIGH)
  {
    Serial.println("Sessiz");
    ThingSpeak.writeField(counterChannelNumberV , 1 ,sessiz  , myCounterWriteAPIKeyV);
  }
  /
   
}
