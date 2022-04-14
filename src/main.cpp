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
  reader.setup();
  //reader.setDebug(true);
  Serial.println("Setup QRCode Reader");

  reader.begin();
  Serial.println("Begin QR Code reader");

  delay(1000);
}

void loop()
{
  bool connected = connectWifi();
  if (isConnected != connected)
  {
    isConnected = connected;
  }

    char* shoppingInput = (char *)httpGETRequest().c_str();
    vector<String> shoppingList;
    // Send HTTP GET request
    char* ptr;
    ptr = strtok(shoppingInput, " ");
    shoppingList.push_back(ptr);
    while(ptr != NULL){
        ptr = strtok(NULL, " ");
        shoppingList.push_back(ptr);
    }
  if (reader.receiveQrCode(&qrCodeData, 100))
  {
    Serial.println("Found QRCode");
    if (find(shoppingList.begin(),shoppingList.end(),(String)(const char *)qrCodeData.payload)!=shoppingList.end())
    {
      shoppingList.erase(find(shoppingList.begin(),shoppingList.end(),(String)(const char *)qrCodeData.payload));
      Serial.print("Payload: ");
      Serial.println((String)(const char *)qrCodeData.payload);


    }
    else
    {
      Serial.print("Invalid: ");
      Serial.println((const char *)qrCodeData.payload);
    }
    
  }  
  


}