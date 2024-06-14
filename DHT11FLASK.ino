#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char *ssid = "MAN1";
const char *password = "majesa1234";
const char *serverName = "http://192.168.60.214:5000/submit";

DHT dht(13, DHT11);

void setup(void) {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(void) {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    float temperature = readDHTTemperature();
    float humidity = readDHTHumidity();

    if (temperature != -1 && humidity != -1) {
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String jsonPayload = "{\"temp\":";
      jsonPayload += temperature;
      jsonPayload += ",\"hum\":";
      jsonPayload += humidity;
      jsonPayload += "}";

      int httpResponseCode = http.POST(jsonPayload);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }

    delay(60000); // Send data every minute
  }
}

float readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  } else {
    return t;
  }
}

float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  } else {
    return h;
  }
}
