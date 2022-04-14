#include <Arduino.h>

#include <string.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32QRCodeReader.h>
#include <vector>

#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define serverName "https//your-url:8080/endpoint"
using namespace std;


ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);
struct QRCodeData qrCodeData;
bool isConnected = false;

bool connectWifi()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return true;
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int maxRetries = 10;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    maxRetries--;
    if (maxRetries <= 0)
    {
      return false;
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  return true;
}
String httpGETRequest() {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}"; 

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void setup()
{
  Serial.begin(9600);


 
}

void loop()
{
  
  
 Serial.println("Hello");

}