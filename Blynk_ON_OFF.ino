#define BLYNK_PRINT Serial //IT SHOULD BE PRINTED RELATED TO THE BLYNK LIBRARY(NOdeMcu-client, blynk-server)
#include <ESP8266WiFi.h>   //INBUILT LIBRARY FOR NODEMCU
#include <BlynkSimpleEsp8266.h>  //LIBRARY USED TO COMBINE BLYNK AND ESP8266
char auth[] = " OHy7n8GPeZBcXRzIKnXZUDu4g4HGhPHE"; //API
char ssid[] = "Aarib's Moto";//SSID
char pass[] = "persie123";//PASSWORD

void setup()
{
 
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);//YOUR CODE STARTS HERE(NODE MCU CONNECTS TO BLYNK APP)
}                                //function and parameters begin(auth,ssid,pass)

void loop()
{
  Blynk.run();//THE PLACE WHERE U RUN THE CODE
}
