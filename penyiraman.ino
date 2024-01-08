#define BLYNK_TEMPLATE_ID "TMPL680tdK9in"
#define BLYNK_TEMPLATE_NAME "Penyiraman"
#define BLYNK_AUTH_TOKEN "eVx5_FsHD-kMHc6fqhF2mtHKcDWu1zpo"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

char ssid[] = "monitor";
char pass[] = "monitor2021";

int lcdColumns = 16;
int lcdRows = 2;

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

const int SensorPin = A0;
const int PompaPin  = D5;

int soilMoistureValue = 0;
int soilmoisturepercent=0;
const int AirValue = 657;   
const int WaterValue = 260; 
boolean statusAuto=true;

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  if(pinValue){
    statusAuto=true;   
  }else{
    statusAuto=false;       
  }
}

BLYNK_WRITE(V2)
{
  if(!statusAuto){  
  int pinValue = param.asInt();
  if(pinValue){
      digitalWrite(PompaPin,LOW);
  }else{
      digitalWrite(PompaPin,HIGH);
  }
  }
}
void myTimerEvent()
{
  soilMoistureValue = analogRead(SensorPin);
  Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  if(soilmoisturepercent > 100)   soilmoisturepercent=100;
  else if(soilmoisturepercent <0) soilmoisturepercent=0;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Soil  Moisture ");

  lcd.setCursor(5, 1);
  lcd.print(soilmoisturepercent);
  lcd.print("%");

  if(statusAuto){  
    if(soilmoisturepercent<50){
      
      digitalWrite(PompaPin,LOW);  
      Blynk.virtualWrite(V2,true);
    
    }else{
      
      digitalWrite(PompaPin,HIGH); 
      Blynk.virtualWrite(V2,false);
     
    }
  }

  Blynk.virtualWrite(V1,soilmoisturepercent);
}

void setup()
{

  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("  Waiting....   ");

  lcd.setCursor(0, 1);
  lcd.print("  Connect Wifi  ");

  pinMode(PompaPin,OUTPUT);
  digitalWrite(PompaPin,HIGH);
    
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
  
  Blynk.virtualWrite(V0,true);
  Blynk.virtualWrite(V2,false);
  
}

void loop()
{
  Blynk.run();
  timer.run();
}
