#include <ESP8266WiFi.h>
#include <WiFiClient.h> 

const char* ssid = "TALKTALK03DAA2";
const char* password = "ND6RBXY6";
const char* host = "192.168.1.3";

#define POTENTIOMETER_PIN A0
int analogValue;

int btnFrwd = 0;
int btnBkwd = 4;
int btnRght = 2;
int btnLeft = 5;

int btnLeftRead = 0;
int btnBkwdRead = 0;
int btnFrwdRead = 0;
int btnRghtRead = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(btnFrwd, INPUT_PULLUP);
  pinMode(btnBkwd, INPUT_PULLUP);
  pinMode(btnRght, INPUT_PULLUP);
  pinMode(btnLeft, INPUT_PULLUP);
}

void loop() {
  btnFrwdRead = digitalRead(btnFrwd);
  btnBkwdRead = digitalRead(btnBkwd);
  btnRghtRead = digitalRead(btnRght);
  btnLeftRead = digitalRead(btnLeft);

  analogValue = analogRead(POTENTIOMETER_PIN);
  Serial.printf("Analog value: %04d\n", analogValue);

  String urlA = "/get?input1=" + String(analogValue);
  Serial.print("Requesting URL: ");
  Serial.println(urlA);

  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("Connection failed");
    return;
  }
   if (btnBkwdRead == LOW) {
    String urlB = "/get?input1=backwards";
    Serial.print("Requesting URL: ");
    Serial.println(urlB);
    client.print(String("GET ") + urlB + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    Serial.println("Request backwards sent");
  } if (btnRghtRead == LOW) {
    String urlB = "/get?input1=right";
    Serial.print("Requesting URL: ");
    Serial.println(urlB);
    client.print(String("GET ") + urlB + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    Serial.println("Request right sent");
  } if (btnLeftRead == LOW) {
    String urlB = "/get?input1=left";
    Serial.print("Requesting URL: ");
    Serial.println(urlB);
    client.print(String("GET ") + urlB + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    Serial.println("Request left sent");
  } if (btnFrwdRead == LOW) {
    String urlB = "/get?input1=forwards";
    Serial.print("Requesting URL: ");
    Serial.println(urlB);
    client.print(String("GET ") + urlB + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    Serial.println("Request forwards sent");
  }
  

  

  client.print(String("GET ") + urlA + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("Request analog sent");

  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }
  String response = client.readStringUntil('\n');
  Serial.println("Response: " + response);

  delay(1000);
}
