#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include<ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClient.h>

WiFiClient wifiClient;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ledPin =  2;

// Wifi Credintials
const char* SSID = "SSID";
const char* Password = "Password";




void connectWifiNetwork(){
  WiFi.begin(SSID, Password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to Your Wifi ...");
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 15);
    display.println("Connectingto wifi...");
    display.display();
  }
}

void requestAPI(){
  // Declare http client reference
  HTTPClient http;

  // Create HTTP Request
  http.begin(wifiClient,"your_api_link");

  // get status code
  int httpCode = http.GET();

  // check if httpCode == 200
  if(httpCode == HTTP_CODE_OK){
    Serial.print("HTTP Status Code: ");
    Serial.print(httpCode);
    Serial.println();

    //print response
    Serial.println(http.getString());
    parseJSON(http.getString());

    
  }

  //close connection
  http.end();
}

void parseJSON(String json){
  
  //declare the buffer
  DynamicJsonDocument buffer(1024);

  //Deserialize the string => object
  deserializeJson(buffer, json);

  //get Message field
  const char* message = buffer["message"];
  int ledStatus = buffer["data"]["status"];

  //Print Response
  Serial.println();
  Serial.print("Response is : ");
  Serial.print(message);
  Serial.println();
  Serial.print("Led 1 Status is: ");
  Serial.print(ledStatus);

  if(ledStatus == 0){
    digitalWrite(ledPin, LOW);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setFont(NULL);
    display.setCursor(0, 16);
    display.println("Connected!Led1: OFF");
    display.display();
  }else{
    digitalWrite(ledPin, HIGH);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setFont(NULL);
    display.setCursor(0, 16);
    display.println("Connected!Led1: ON");
    display.display();
  }

}


void setup() {

  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);

  //welcome screen delay 3s
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setFont(NULL);
  display.setCursor(0, 9);
  display.println("WELCOME TOHOME IOT  AUTOMATION");
  display.display();
  delay(3000);

  //setup wifi
  connectWifiNetwork();

}

void loop() {
  
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Successfully Connected to Your Wifi Network.");

    requestAPI();
    delay(2000);
  }

}
