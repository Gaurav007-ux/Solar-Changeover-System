#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Solar-Controller";
const char* password = "12345678";

WebServer server(80);

const int relayPin = 23;
bool relayState = false;

String page() {
  String stateText = relayState ? "ON" : "OFF";
  String stateClass = relayState ? "on" : "off";

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Solar Changeover Controller</title>
<style>
*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:Segoe UI,sans-serif;
}
body{
background:#0f172a;
display:flex;
justify-content:center;
align-items:center;
height:100vh;
color:white;
}
.card{
width:90%;
max-width:420px;
background:#1e293b;
padding:30px;
border-radius:20px;
box-shadow:0 10px 30px rgba(0,0,0,0.4);
text-align:center;
}
h1{
margin-bottom:25px;
font-size:28px;
}
.status{
font-size:22px;
margin-bottom:25px;
}
.on{
color:#22c55e;
font-weight:bold;
}
.off{
color:#ef4444;
font-weight:bold;
}
button{
width:100%;
padding:15px;
font-size:18px;
border:none;
border-radius:12px;
cursor:pointer;
background:#2563eb;
color:white;
transition:0.3s;
}
button:hover{
background:#1d4ed8;
}
.footer{
margin-top:20px;
font-size:13px;
color:#94a3b8;
}
</style>
</head>
<body>
<div class="card">
<h1>Solar Changeover</h1>
<div class="status">
Relay Status:
<span class=")rawliteral";

  html += stateClass;

  html += R"rawliteral(">
)rawliteral";

  html += stateText;

  html += R"rawliteral(
</span>
</div>

<form action="/toggle" method="GET">
<button type="submit">Toggle Relay</button>
</form>

<div class="footer">
ESP32 Control Dashboard
</div>
</div>
</body>
</html>
)rawliteral";

  return html;
}

void handleRoot() {
  server.send(200, "text/html", page());
}

void handleToggle() {
  relayState = !relayState;

  if(relayState)
    digitalWrite(relayPin, LOW);
  else
    digitalWrite(relayPin, HIGH);

  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);

  server.begin();
}

void loop() {
  server.handleClient();
}
