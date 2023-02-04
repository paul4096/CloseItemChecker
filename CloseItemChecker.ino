// Complete Instructions: https://RandomNerdTutorials.com/esp8266-nodemcu-digital-inputs-outputs-arduino/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

const char* ssid = "NETGEAR";
const char* password = "kalamandaros122";
const int REPEAT_INTERVAL = 120;
//Your Domain name with URL path or IP address with path
String serverName = "https://chanciest-chamois-6999.dataplicity.io/api/v1/doorstate";

// set pin numbers
const int buttonPin = 5;     // the number of the pushbutton pin
const int ledPin =  14;       // the number of the LED pin

// variable for storing the pushbutton status
int buttonState = 0;
int prevbuttonState = 0;
ESP8266WiFiMulti WiFiMulti;
bool sendToSever = true;
int repeatIntervalCounter = REPEAT_INTERVAL;

#include <ESP8266HTTPClient.h>
void setup() {
  sendToSever = true;
  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
   pinMode(ledPin-2, OUTPUT);

  Serial.begin(115200); 


  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SSID", "password");
  // WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

}

void loop() {
  // read the state of the pushbutton value
  prevbuttonState = buttonState;
  buttonState = digitalRead(buttonPin);
  if(buttonState!=prevbuttonState){
    sendToSever = true;
  }
  Serial.println("STARTING");
  //if it is, the buttonState is HIGH
  if (buttonState == HIGH) {
    // turn LED on
    Serial.println("Input state HIGH");
    digitalWrite(ledPin, LOW);
  
  } else {
    // turn LED off
    Serial.println("Input state LOW");
    digitalWrite(ledPin, HIGH);
 
  }

  if(!sendToSever){
    delay(500);
    repeatIntervalCounter--;
    if(repeatIntervalCounter<0){
      sendToSever = true;
      repeatIntervalCounter = REPEAT_INTERVAL;
    }
    return;
  }
  if ((WiFiMulti.run() == WL_CONNECTED)) {
        // WiFiClient client;
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

        client->setInsecure();
        HTTPClient http;
        // Your Domain name with URL path or IP address with path
        http.begin(*client, serverName);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
        int httpResponseCode = http.POST("doorState="+String(buttonState));
        if(httpResponseCode == 200){
          sendToSever = false;
        }
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
            
          // Free resources
          http.end();
  }
  delay(10000);

}
