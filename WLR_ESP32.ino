#https://github.com/Tisha768/My-Wheel-Legged-Robot/tree/main

#include <WiFi.h>
#include <ESP32Servo.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

Servo servoTopLeft;
Servo servoTopRight;
Servo servoBottomLeft;
Servo servoBottomRight;
Servo servoBack;

int enA = 13; 
int enB = 12; 
int in1 = 14;
int in2 = 27;
int in3 = 26;
int in4 = 25;

int posTL;
int posTR;
int posBL;
int posBR;
int posB;

AsyncWebServer server(80);

const char* ssid = "TALKTALK03DAA2";
const char* password = "ND6RBXY6";

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    input1: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input2: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input3: <input type="text" name="input3">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";



void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String inputMessage;

void setup()
{
  servoTopLeft.attach(33);
  servoTopRight.attach(32);
  servoBottomLeft.attach(19);
  servoBottomRight.attach(18);
  servoBack.attach(21);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

    Serial.begin(115200);
    pinMode(13, OUTPUT);      // set the LED pin mode

    delay(10);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    servoTopLeft.write(75);
    servoTopRight.write(105);
    servoBottomLeft.write(30);
    servoBottomRight.write(150);
    servoBack.write(60);

    WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();

}

void loop() {
  delay(500);
  if (inputMessage.length() > 0) { // check if inputMessage is not empty
    int intInputMessage = inputMessage.toInt();
    if (intInputMessage != 0) { // check if inputMessage contains a valid integer value
      if (intInputMessage > 1000) {
        if (servoTopLeft.read() == 115) {
          Serial.println("already on 115");
        } else {
          for (posTL = servoTopLeft.read(); posTL <= 115; posTL += 1) {
            servoTopLeft.write(posTL); 
            delay(100);         
          }
          for (posTR = servoTopRight.read(); posTR >= 71; posTR -= 1) {
            servoTopRight.write(posTR);  
            delay(100);         
          }
        }
        
      } else {
        Serial.println("no");
      }
      if (intInputMessage < 100) {
          for (posTL = servoTopLeft.read(); posTL >= 75; posTL -= 1) {
            servoTopLeft.write(posTL); 
            delay(100);         
          }
          for (posTR = servoTopRight.read(); posTR <= 105; posTR += 1) {
            servoTopRight.write(posTR);  
            delay(100);         
          }
        
      } else {
        Serial.println("no");
      }
      
    } else {
      Serial.println("no");
    }
  }
   if (inputMessage == "backwards") {
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
          Serial.println("backwards");
          delay(800);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          delay(4500);  
  } if (inputMessage == "right") {
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          Serial.println("right");
          delay(800);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          delay(4500); 
  } if (inputMessage == "left") {
          digitalWrite(in1, HIGH);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          Serial.println("left");
          delay(800);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          delay(4500); 
  } if (inputMessage == "forwards") {
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          Serial.println("forwards");
          delay(800);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          delay(4500);       
  }
}

