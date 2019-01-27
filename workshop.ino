//Header Files to be used
#include<ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//Adafruit.io details
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "Tachyon01"
#define AIO_KEY  "19d6e75a354a423a8a64d198eef9cc76"

WiFiClient client;

//To connect to Adafruit server
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe led = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led");

String x;

void Wifi()           //Connect to Wifi
{
  WiFi.disconnect();
  delay(1000);
  Serial.println("START");
  WiFi.begin("Ni milega","bepa8139");     //Change to your credentials
  while(!(WiFi.status() == WL_CONNECTED))
  {
    delay(300);
    Serial.print("...");
  }
  Serial.println("Connected");
  Serial.println("Your IP is ");
  Serial.print(WiFi.localIP().toString());
}

void MQTT_connect()
{
    if (mqtt.connected())
    {
      return;
    }
    int8_t ret;
    Serial.println("Coneecting to MQTT");
    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");    
}

void setup() 
{
  Serial.begin(9600);
  Wifi();     //Connect to Wifi
  pinMode(D4,OUTPUT);        
  mqtt.subscribe(&led);     //Subscribe to Light feed to control colour
  MQTT_connect();             //Connect to MQTT server on of adafruit
}



void loop()
{
  if(mqtt.connected()==0)     //If mqtt connection failed, reconnect
  {
    MQTT_connect();
  }
  Adafruit_MQTT_Subscribe *subscription_name;
  while (subscription_name = mqtt.readSubscription(5000)){
  x=((char *  )led.lastread);
  Serial.println(x);
  if(x[1]=='F')
  {
  digitalWrite(D4,HIGH);
  }
  else
  {
    digitalWrite(D4,LOW);
  }}
}
