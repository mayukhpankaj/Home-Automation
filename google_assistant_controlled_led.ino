//message queue telemetry transport
//used IFTTT, adafruit.io nodeMCU

//this uses a ONOFF feed named "feed1"

/* there are two actions in IFTTT one turns feed1 data to 1 for Phrase like "turn on the lights" in google Assistant.Like wise second one turns feed1 value 2 for phraselike "turn off light" */


#include <ESP8266WiFi.h>//inbult lib for esp8266
#include "Adafruit_MQTT.h"//adafruit acts as server 
#include "Adafruit_MQTT_Client.h"//adafruit feeds ,dashboard or webpage  and nodeMcu is client

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pwd"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883       //1883 is mqtt port number            // use 8883 for SSL
#define AIO_USERNAME  "mayukh"
#define AIO_KEY       "aio_VYgf52two4qjU3ZhrWZ2G3cXEmXj"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect NodeMcu to the MQTT server.
WiFiClient client;//wifi client is NodeMcu

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// library           class           instances of the class-above line
/****************************** Feeds ***************************************/

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/feed1");
//class                 object           function                     parameters
/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();//function call

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(D0,OUTPUT);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);//mqtt is a class which is subscribing to on off button which is an object
}

uint32_t x=0;//unsigned int of 32 bits 

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
      uint16_t state=atoi((char*)onoffbutton.lastread);//normally we get string from server and we need int bcoz (1/0)is used
      digitalWrite(D0,state);
    }
  }

 
  
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

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
