#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Ping.h>

const char* ssid = "IMARI";  // Replace with your Wi-Fi network name
const char* password = "S3R0J4Y4";  // Replace with your Wi-Fi password

AsyncWebServer server(80);

String getSystemStatus() {
    String status = "{";
    status += "\"wifi_status\":\"" + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected") + "\",";
    status += "\"ip_address\":\"" + WiFi.localIP().toString() + "\",";
    status += "\"uptime\":\"" + String(millis() / 1000) + " seconds\",";
    status += "\"heap_memory\":\"" + String(ESP.getFreeHeap()) + " bytes\",";
    status += "\"cpu_usage\":\"" + String(esp_get_minimum_free_heap_size()) + " bytes\",";
    status += "\"directory_status\":\"" + String(SPIFFS.begin(true) ? "Mounted" : "Not Mounted") + "\"}";
    return status;
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 System Status</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }
        h1 { color: #007bff; }
        .status-box { display: inline-block; padding: 15px; border: 1px solid #ddd; border-radius: 10px; box-shadow: 2px 2px 10px #ccc; }
    </style>
    <script>
        function updateStatus() {
            fetch('/status')
            .then(response => response.json())
            .then(data => {
                document.getElementById('wifi').innerText = data.wifi_status;
                document.getElementById('ip').innerText = data.ip_address;
                document.getElementById('uptime').innerText = data.uptime;
                document.getElementById('heap').innerText = data.heap_memory;
                document.getElementById('cpu').innerText = data.cpu_usage;
                document.getElementById('directory').innerText = data.directory_status;
            });
        }
        setInterval(updateStatus, 5000);
    </script>
</head>
<body>
    <h1>ESP32 System Status</h1>
    <div class="status-box">
        <p><strong>WiFi Status:</strong> <span id="wifi">Loading...</span></p>
        <p><strong>IP Address:</strong> <span id="ip">Loading...</span></p>
        <p><strong>Uptime:</strong> <span id="uptime">Loading...</span></p>
        <p><strong>Heap Memory:</strong> <span id="heap">Loading...</span></p>
        <p><strong>CPU Usage:</strong> <span id="cpu">Loading...</span></p>
        <p><strong>Directory Status:</strong> <span id="directory">Loading...</span></p>
    </div>
</body>
</html>
)rawliteral";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected.");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html);
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", getSystemStatus());
    });

    server.begin();
}

void loop() {
}
