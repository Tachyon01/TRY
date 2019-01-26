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

////To connect to IFTTT
//String MakerIFTTT_Key ;
//String MakerIFTTT_Event;
//char *append_str(char *here, String s) {  int i=0; while (*here++ = s[i]){i++;};return here-1;}
//char *append_ul(char *here, unsigned long u) { char buf[20]; return append_str(here, ultoa(u, buf, 10));}
//char post_rqst[256];char *p;char *content_length_here;char *json_start;int compi;

//To connect to Adafruit server
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe led = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led");

char* x;

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
//  pinMode(Bell,INPUT);        //To read pressing of bell
//  t=millis();                 //Get current time for board 
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
  x=((char *)led.lastread);
  Serial.println(x);
  if(strcmp(x,"OF"))
  {
  digitalWrite(BUILTIN_LED,HIGH);
  }
  else
  {
    digitalWrite(BUILTIN_LED,LOW);
  }}
  }
// 
//  if(digitalRead(Bell)==0 && b_on+2000<millis())  //Read if bell pressed and time between two press is atleast 2 sec
//  {
//     b_on=millis();                               //Update time when bell was pressed
//     Serial.println("Bell pressed");
//     Bell_notification();                          //Call function to send notification
//  }
  

