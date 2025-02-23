#include <WiFi.h>
#include <ESPAsyncWebServer.h>  // Async server library

const char* ssid = "IMARI";  // Replace with your Wi-Fi network name
const char* password = "S3R0J4Y4";  // Replace with your Wi-Fi password

#define LED_PIN 2  // Pin for built-in LED (GPIO 2)

// Variable to track the LED state
bool ledState = false;  // false = OFF, true = ON

AsyncWebServer server(80);  // Create a server that listens on port 80 (HTTP)

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);  // Set the LED pin as an output

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Print the ESP32's IP address
  Serial.println(WiFi.localIP());

  // Serve the main HTML page to the client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h1>ESP32 Web Server</h1>";
    
    // Display the current LED state and the appropriate button
    if (ledState) {
      html += "<p>LED is ON</p>";
      html += "<p><a href=\"/toggle\"><button>Turn Off LED</button></a></p>";
    } else {
      html += "<p>LED is OFF</p>";
      html += "<p><a href=\"/toggle\"><button>Turn On LED</button></a></p>";
    }
    
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  // Toggle LED state on "/toggle" route
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){
    // Toggle the LED state
    ledState = !ledState;
    
    // Set the LED based on the new state
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    
    // Redirect back to the main page to show the new state
    request->redirect("/");
  });

  // Start the server
  server.begin();
}

void loop() {
  // The server runs asynchronously, so nothing is needed here
}
