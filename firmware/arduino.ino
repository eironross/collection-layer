
#include "thingProperties.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

unsigned long lastPrintTime = 0;
CloudString jsonPayload;
void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  if (millis() - lastPrintTime >= 10000) {
    if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    http.begin("<localhost>/api/sensors");
    http.addHeader("Content-Type", "application/json");
    
    Serial.println("Hello World");

    JsonDocument doc;

    doc["node_id"] = "NFT_A";
    doc["zone_id"] = "ZONE-1";
    doc["status"] = "active";

    JsonObject sensors = doc.createNestedObject("sensors");
    sensors["temp"] = random(200, 350) / 10.0;
    sensors["humidity"] = random(40, 80);

    String requestBody; 
    // empty string added since the doc is currently an Object the the serialize convert the object to Text thus including the requestBody
    serializeJson(doc, requestBody);

    jsonPayload = requestBody;

    Serial.print("Sending the payload to the cloud: ");
    Serial.println(jsonPayload);
    Serial.println(requestBody);  

    int httpResponseCode = http.POST(requestBody);

    // Check the connection
    if (httpResponseCode > 0) {
      Serial.print("HTTP Responde Code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    
    http.end();
    
    lastPrintTime = millis(); // Reset the timer
    }
  }
}
