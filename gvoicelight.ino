#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "Iserver"
#define WIFI_PASS "jkdw34"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "Mishal56"
#define MQTT_PASS "f28208ca63dc42fabc436470f9d8635c"
#define PIN1 D5

int ledp = 500;


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);


Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/onoff");


void setup()
{
  Serial.begin(9600);

  
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("OK!");

  
  mqtt.subscribe(&onoff);

  pinMode(PIN1, OUTPUT);
 analogWrite(PIN1, ledp);
}

void loop()
{
  MQTT_connect();
  
  
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    
    if (subscription == &onoff)
    {
      
      Serial.print("onoff: ");
      Serial.println((char*) onoff.lastread);
      
      
     if (!strcmp((char*) onoff.lastread, "ON"))
     { ledp=500;
      analogWrite(PIN1, 500);
      }
     if (!strcmp((char*) onoff.lastread, "OFF"))
     { ledp=0;
       analogWrite(PIN1, 0);
      }


      if (!strcmp((char*) onoff.lastread, "MAX"))
     { ledp=1023;
        analogWrite(PIN1, 1023);
      }
     if (!strcmp((char*) onoff.lastread, "MIN"))
     { ledp=5;
       analogWrite(PIN1, 5);
      }


      
      if (!strcmp((char*) onoff.lastread, "IN"))
      {
        
       while(ledp!=1023)
        
        { 
          if(ledp==500)
          {ledp=1023;}
        else 
          {
        ledp=ledp*10;
          }
          
          analogWrite(PIN1, ledp);
          delay(5000);
            mqtt.readSubscription(5000);
        if (!strcmp((char*) onoff.lastread, "STOP"))
          {  Serial.println((char*) onoff.lastread);
            break;
          }
        
          

        
        }
        
      }

      if (!strcmp((char*) onoff.lastread, "DE"))
      {
        
       while(ledp!=5)
        
        {  
          
          if(ledp==1023)
          {ledp=500;}
        else 
          {
        ledp=ledp/10;
          }
          
          analogWrite(PIN1, ledp);
          delay(5000);
            mqtt.readSubscription(5000);
        if (!strcmp((char*) onoff.lastread, "STOP"))
          {  Serial.println((char*) onoff.lastread);
            break;
          }
        

        
        }
        
      }
      
    }
  }

  
  if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}




void MQTT_connect() 
{
  int8_t ret;

  
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) 
  { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  
       retries--;
       if (retries == 0) 
       {
         
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


