#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

const char *ssid = "your-ssid";
const char *password = "your-password";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Handle WebSocket events
  ws.onEvent(onWsEvent);
  
  // Serve HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<html><body><h1>Hello from ESP32!</h1></body></html>");
  });

  // Start server
  server.begin();
}

void loop() {
  // Nothing to do here
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.println("WebSocket client connected");
      break;
    case WS_EVT_DISCONNECT:
      Serial.println("WebSocket client disconnected");
      break;
    case WS_EVT_DATA:
      handleWsData(client, data, len);
      break;
  }
}

void handleWsData(AsyncWebSocketClient *client, uint8_t *data, size_t len){
  // Process WebSocket data here
  // For example, you can send data back to the client
  client->text("Received: " + String((char*)data));
}
