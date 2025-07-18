#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP-CONTROL-HUB";
const char* password = "12345678";

WebServer server(80);
String cam_ip = "192.168.4.2";

// Serial2 pins for RX/TX
#define RXD2 18
#define TXD2 17

// Sensor values
String ldrStatus = "N/A";
String distanceStatus = "N/A";
String obstacleStatus = "N/A";
String headingStatus = "N/A";
String motionStatus = "N/A";
String micStatus = "N/A";

void sendCommandToArduino(const String& cmd) {
  Serial2.println(cmd);
  Serial.println("[DEBUG] Command sent to Arduino: " + cmd);
}

void handleRoot() {
  Serial.println("[DEBUG] Root page requested by client");
  String html = R"rawliteral(
    <html>
    <head>
      <title>Robot Control & Camera</title>
      <style>
        body { font-family: Arial; background: #f0f0f0; text-align: center; }
        .section {
          background: #fff; border: 1px solid #ccc; border-radius: 8px;
          padding: 20px; margin: 20px auto; max-width: 400px;
          box-shadow: 2px 2px 8px rgba(0,0,0,0.1);
        }
        button {
          width: 80px; height: 80px; font-size: 18px; margin: 5px;
        }
        .button2 {
          width: 120px; height: 80px; font-size: 18px; margin: 5px;
        }
      </style>
    </head>
    <body>
      <h1>AmphiBot Control Interface</h1>
      <p>Use the control pad to move the robot. Sensor data updates every 2 seconds.</p>

      <div class="section">
        <h2>Live Camera Stream</h2>
        <button class="button2" onclick="window.open('http://192.168.4.2', '_blank')">Open Camera Interface</button>
      </div>

      <div class="section">
        <h2>Motor Controls</h2>
        <table style="margin:auto;">
          <tr><td></td><td><button style="text-align:center;" onclick="fetch('/forward')">Front</button></td><td></td></tr>
          <tr>
            <td><button style="text-align:center;" onclick="fetch('/left')">Left</button></td>
            <td><button style="text-align:center;" onclick="fetch('/stop')">Stop All</button></td>
            <td><button style="text-align:center;" onclick="fetch('/right')">Right</button></td>
          </tr>
          <tr><td></td><td><button onclick="fetch('/backward')">Back</button></td><td></td></tr>
        </table>
      </div>

      <div class="section" id="statusBox">
        <h2>Sensor Status</h2>
        <p><b>Brightness:</b> <span id="ldr">Loading...</span></p>
        <p><b>Distance:</b> <span id="dist">Loading...</span></p>
        <p><b>Obstacle:</b> <span id="obs">Loading...</span></p>
        <p><b>Heading:</b> <span id="head">Loading...</span></p>
        <p><b>Motion:</b> <span id="motion">Loading...</span></p>
        <p><b>Microphone:</b> <span id="mic">Loading...</span></p>
      </div>

      <script>
        setInterval(() => {
          fetch('/status')
            .then(res => res.json())
            .then(data => {
              document.getElementById('ldr').innerText = data.ldr;
              document.getElementById('dist').innerText = data.dist;
              document.getElementById('obs').innerText = data.obs;
              document.getElementById('head').innerText = data.head;
              document.getElementById('motion').innerText = data.motion;
              document.getElementById('mic').innerText = data.mic;
            });
        }, 2000);
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleStatus() {
  String json = "{";
  json += "\"ldr\":\"" + ldrStatus + "\",";
  json += "\"dist\":\"" + distanceStatus + "\",";
  json += "\"obs\":\"" + obstacleStatus + "\",";
  json += "\"head\":\"" + headingStatus + "\",";
  json += "\"motion\":\"" + motionStatus + "\",";
  json += "\"mic\":\"" + micStatus + "\"";
  json += "}";
  Serial.println("[DEBUG] Sending sensor status JSON");
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  Serial.println("[DEBUG] Serial started");

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("[DEBUG] Serial2 (to Arduino) initialized");

  WiFi.softAP(ssid, password);
  Serial.println("[DEBUG] WiFi AP started with SSID: " + String(ssid));
  Serial.println("AP IP address: " + WiFi.softAPIP().toString());

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/forward", []() { sendCommandToArduino("F"); server.send(200, "text/plain", "OK"); });
  server.on("/backward", []() { sendCommandToArduino("B"); server.send(200, "text/plain", "OK"); });
  server.on("/left", []() { sendCommandToArduino("L"); server.send(200, "text/plain", "OK"); });
  server.on("/right", []() { sendCommandToArduino("R"); server.send(200, "text/plain", "OK"); });
  server.on("/stop", []() { sendCommandToArduino("S"); server.send(200, "text/plain", "OK"); });

  server.begin();
  Serial.println("[DEBUG] Web server started");
}

void loop() {
  server.handleClient();

  if (Serial2.available()) {
    String line = Serial2.readStringUntil('\n');
    line.trim();
    Serial.println("[DEBUG] Received from Arduino: " + line);

    int ldrIdx = line.indexOf("LDR:");
    int distIdx = line.indexOf("DIST:");
    int obsIdx = line.indexOf("OBS:");
    int headIdx = line.indexOf("HEAD:");
    int motionIdx = line.indexOf("MOTION:");
    int micIdx = line.indexOf("MIC:");

    if (ldrIdx != -1 && distIdx != -1 && obsIdx != -1 && headIdx != -1 && motionIdx != -1 && micIdx != -1) {
      ldrStatus = line.substring(ldrIdx + 4, line.indexOf(',', ldrIdx));
      distanceStatus = line.substring(distIdx + 5, line.indexOf(',', distIdx));
      obstacleStatus = line.substring(obsIdx + 4, line.indexOf(',', obsIdx));
      headingStatus = line.substring(headIdx + 5, line.indexOf(',', headIdx));
      motionStatus = line.substring(motionIdx + 7, line.indexOf(',', motionIdx));
      micStatus = line.substring(micIdx + 4);
      Serial.println("[DEBUG] Parsed values → Brightness: " + ldrStatus + ", Distance: " + distanceStatus +
                     ", Obstacle: " + obstacleStatus + ", Heading: " + headingStatus +
                     ", Motion: " + motionStatus + ", MIC: " + micStatus);
    }
  }
}
