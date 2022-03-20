#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <stdio.h>

const char* ssid     = "This ain't a hotspot";
const char* password = "henry123";

// Temporary host for development purposes.
const char* host = "http://d4e9-128-189-94-49.ngrok.io";
const char* jsonStructure =\
  "{\"SessionID\":\"%s\",\"Coord\":{\"lat\":\"%s\",\"lon\":\"%s\"},\"NumPlayers\":\"%s\",\"Win\":\"%s\"}";

//--------------------------Helper Functions-------------------------//

// Connect the esp8266 to the WiFi specified in the constants above
void connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}

// Fill our JSON message with the relevant paramters (see README)
String constructJSON(String sessionID, String lat, String lon, String numPlayers, String win) {
  char buffer[50];
  sprintf(buffer, 
          jsonStructure,
          sessionID.c_str(), lat.c_str(), lon.c_str(), numPlayers.c_str(), win.c_str());
  return String(buffer);
}

// Read one of our data strings from the serial buffer, ending with a delimiter (see README)
inline String getSerialString(char delimiter) {
  while (!Serial.available());
  String result = "";
  char c;
  while ((c = Serial.read()) != delimiter) {
    result += String(c);
    while(!Serial.available());
  }
  return result;
}

// Extract data from incoming serial input
String getSerialData() {
  String sessionID, lat, N_S, lon, E_W, numPlayers, win;
  Serial.flush();
  while (!Serial.available());
  while ((Serial.read()) != '$') // Loop until start of data string
    while (!Serial.available());
  char c;
  Serial.println("Getting session ID");
  sessionID = getSerialString(',');
  Serial.println(sessionID);
  lat = getSerialString(',');
  Serial.println(lat);
  N_S = getSerialString(',');
  if (N_S == "S")
    lat = "-" + lat;
  lon = getSerialString(',');
  Serial.println(lon);
  E_W = getSerialString(',');
  if (E_W == "W")
    lon = "-" + lon;
  numPlayers = getSerialString(',');
  Serial.println(numPlayers);
  win = getSerialString('*'); // Last delimiter is '*', not ','
  Serial.println(win);
  return constructJSON(sessionID, lat, lon, numPlayers, win);
}

//---------------------End of Helper Functions---------------------//

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println();
  Serial.println();
  connectWiFi();
}

void loop() {
  delay(5000);
  if (WiFi.status() != WL_CONNECTED)
    connectWiFi();
  WiFiClient client;
  HTTPClient http;
  while (WiFi.status() == WL_CONNECTED) {
    http.begin(client, host);
    http.addHeader("Content-Type", "application/json");
    Serial.println("Reading Serial for data");
    String data = getSerialData();
    Serial.print("Sending POST request: ");
    Serial.println(data);
    int statusCode = http.POST(data);
    if (statusCode > 0) {
      if (statusCode = HTTP_CODE_OK) { // 200
        Serial.println("POST successful!");
      } else if (statusCode = HTTP_CODE_BAD_REQUEST) { // 400
        Serial.println("POST failed with code 400!");
      } else {
        Serial.println("POST failed with unknown status code!");
      }
    } else {
      // HTTPClient error
      Serial.println("POST failed with error: " + http.errorToString(statusCode));
    }
    http.end();
  }
}
