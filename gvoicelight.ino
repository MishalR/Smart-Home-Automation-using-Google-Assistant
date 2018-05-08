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

//Set up MQTT and WiFi clients
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//Set up the feed you're subscribing to
Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/onoff");


void setup()
{
  Serial.begin(9600);

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("OK!");

  //Subscribe to the onoff feed
  mqtt.subscribe(&onoff);

  pinMode(PIN1, OUTPUT);
 analogWrite(PIN1, ledp);
}

void loop()
{
  MQTT_connect();
  
  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &onoff)
    {
      //Print the new value to the serial monitor
      Serial.print("onoff: ");
      Serial.println((char*) onoff.lastread);
      
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
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
        //Active low logic
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
        //Active low logic
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

  // ping the server to keep the mqtt connection alive
  if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}


/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

void MQTT_connect() 
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


