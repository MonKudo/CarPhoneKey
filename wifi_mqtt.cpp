#include <Arduino.h>
#include "variables.h"
#include <WiFi.h>
#include <WebServer.h>
#include <PicoMQTT.h>
#include "wifi_mqtt.h"
#include "pins.h"
#include "config.h"

int status = WL_IDLE_STATUS;
WiFiServer server(80);
String header;
PicoMQTT::Server MqttBroker;

void WifiSetup() {
  /*
  WiFi.mode(WIFI_AP);
  WiFi.setHostname("Main Module");
  WiFi.softAP(ssid, password);
  IPAddress IP(192, 168, 1, 1);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(IP, IP, NMask);
  server.begin();

  Serial.print("Creating access point named: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(IP);
  */

  WiFi.softAP(configData.ssid, configData.password);
  delay(100);

  server.on("/", handleRoot);
  server.on("/config", handleConfig);
  server.on("/save", handleSave);
  server.on("/restart", handleRestart);
  server.on("/controls", handleControls);
  server.on("/control/:id", handleControl);

  server.begin();
  Serial.println("Access Point started");
}

void MqttSetup() {
  Serial.println("Starting MQTT broker");
  MqttBroker.subscribe("#", [](const char * topic, const char * payload) {
    Serial.printf("Received message in topic '%s': %s\n", topic, payload);
  });
  MqttBroker.begin();
}

/*
void WebInterface() {
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                           // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /0") >= 0) {
              Serial.println("Status 0");
              CarStatus = 0;
            } else if (header.indexOf("GET /1") >= 0) {
              Serial.println("Status 1");
              CarStatus = 1;
            } else if (header.indexOf("GET /2") >= 0) {
              Serial.println("Status 2");
              CarStatus = 2;
            } else if (header.indexOf("GET /3") >= 0) {
              Serial.println("Status 3");
              CarStatus = 3;
            } else if (header.indexOf("GET /4") >= 0) {
              Serial.println("Status 4");
              CarStatus = 4;
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; border-radius: 25px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>T-ROC Web Server</h1>");

            client.println("<p>Mode 0</p>");
            if (CarStatus == 0) {
              client.println("<p><a href=\"/0\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/0\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Mode 1</p>");
            if (CarStatus == 1) {
              client.println("<p><a href=\"/1\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Mode 2</p>");
            if (CarStatus == 2) {
              client.println("<p><a href=\"/2\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Mode 3</p>");
            if (CarStatus == 3) {
              client.println("<p><a href=\"/3\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/3\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Mode 4</p>");
            if (CarStatus == 4) {
              client.println("<p><a href=\"/4\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>ACC</p>");
            if (digitalRead(ACC_PIN) == HIGH) {
              client.println("<p><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>IGNITION 1</p>");
            if (digitalRead(IGNITION_1_PIN) == HIGH) {
              client.println("<p><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>IGNITION 2</p>");
            if (digitalRead(IGNITION_2_PIN) == HIGH) {
              client.println("<p><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>STARTER</p>");
            if (digitalRead(STARTER_PIN) == HIGH) {
              client.println("<p><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>UNLOCK</p>");
            if (digitalRead(UNLOCK_PIN) == HIGH) {
              client.println("<p><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>LOCK</p>");
            if (digitalRead(LOCK_PIN) == HIGH) {
              client.println("<p><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("</body></html>");
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    header = "";
    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}
*/

void MqttLoop(){
  MqttBroker.loop();
}

void handleRoot(ConfigData configData) {
  String html = "<html><body style='text-align:center;background-color:black;color:white;'>";
  html += "<h1>" + String(configData.deviceName) + "</h1>";
  html += "<br>";
  html += "<a href='/controls' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:#4CAF50; color:white; border:none; margin:10px;'>Controls</button>";
  html += "</a>";
  html += "<br>";
  html += "<a href='/config' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:#2196F3; color:white; border:none; margin:10px;'>Configuration</button>";
  html += "</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleConfig(ConfigData configData) {
  String html = "<html><body style='text-align:center;background-color:black;color:white;'>";
  html += "<h1>Configuration</h1>";
  html += "<br>";
  html += "<form action='/save'>";
  html += "<label for='ssid'>SSID:</label><br>";
  html += "<input type='text' id='ssid' name='ssid' value='" + String(configData.ssid) + "'><br><br>";
  html += "<label for='password'>Password:</label><br>";
  html += "<input type='password' id='password' name='password' value='" + String(configData.password) + "'><br><br>";
  html += "<label for='deviceName'>Device Name:</label><br>";
  html += "<input type='text' id='deviceName' name='deviceName' value='" + String(configData.deviceName) + "'><br><br>";
  html += "<input type='submit' value='Save' style='padding:10px 20px; border-radius:5px; background-color:#4CAF50; color:white; border:none; margin:10px;'>";
  html += "</form>";
  html += "<br>";
  html += "<a href='/' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:red; color:white; border:none; margin:10px;'>Back</button>";
  html += "</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleSave(ConfigData configData) {
  String ssid = server.arg("ssid");
  String password = server.arg("password");
  String deviceName = server.arg("deviceName");

  strncpy(configData.ssid, ssid.c_str(), MAX_SSID_LENGTH - 1);
  configData.ssid[MAX_SSID_LENGTH - 1] = '\0';

  strncpy(configData.password, password.c_str(), MAX_PASSWORD_LENGTH - 1);
  configData.password[MAX_PASSWORD_LENGTH - 1] = '\0';

  strncpy(configData.deviceName, deviceName.c_str(), MAX_DEVICE_NAME_LENGTH - 1);
  configData.deviceName[MAX_DEVICE_NAME_LENGTH - 1] = '\0';

  saveConfigData();

  server.sendHeader("Location", "/restart");
  server.send(303);
}

void handleRestart() {
  String html = "<html><body style='text-align:center;background-color:black;color:white;'>";
  html += "<h1>Restarting...</h1>";
  html += "<script>setTimeout(function(){ window.location.reload(); }, 2000);</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);

  delay(1000);
  ESP.restart();
}

void handleControls() {
  String html = "<html><body style='text-align:center;background-color:black;color:white;'>";
  html += "<h1>Controls</h1>";
  html += "<br>";
  html += "<div style='display:flex; flex-wrap:wrap; justify-content:center;'>";
  html += "<a href='/control/1' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:#4CAF50; color:white; border:none; margin:10px;'>1</button>";
  html += "</a>";
  html += "<a href='/control/2' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:#4CAF50; color:white; border:none; margin:10px;'>2</button>";
  html += "</a>";
  html += "<a href='/control/3' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:#4CAF50; color:white; border:none; margin:10px;'>3</button>";
  html += "</a>";
  html += "<a href='/control/4' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:#4CAF50; color:white; border:none; margin:10px;'>4</button>";
  html += "</a>";
  html += "<a href='/control/5' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:#4CAF50; color:white; border:none; margin:10px;'>5</button>";
  html += "</a>";
  html += "<a href='/control/6' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:#4CAF50; color:white; border:none; margin:10px;'>6</button>";
  html += "</a>";
  html += "</div>";
  html += "<br>";
  html += "<a href='/' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:red; color:white; border:none; margin:10px;'>Back</button>";
  html += "</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleControl() {
  String controlId = server.pathArg(0);
  String html = "<html><body style='text-align:center;background-color:black;color:white;'>";
  html += "<h1>Control " + controlId + "</h1>";
  html += "<br>";
  html += "<a href='/controls' style='text-decoration:none;'>";
  html += "<button style='padding:10px 20px; border-radius:5px; background-color:red; color:white; border:none; margin:10px;'>Back</button>";
  html += "</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleClients(){
  server.handleClient();
}
