#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>

WiFiClient wifiClient;

SoftwareSerial esp(D2,D3); //TX,RX
SoftwareSerial esp2(D5,D6); //TX,RX

const char* ssid = "CSLAB2GHZ";
const char* password = "cslabm606"; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  esp.begin(9600);
  esp2.begin(9600);

  delay(100);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status( ) != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP( ));
}

void loop() {
  // put your main code here, to run repeatedly:
  int soju = Serial.read();
  int kkk = esp.read();
  int result = kkk;
  if(soju != -1) {
    result = 1 + kkk;  
  }
  Serial.println(kkk);
  Serial.println(soju);
  Serial.println();
  delay(100);
   
   if (kkk != -1 && WiFi.status() == WL_CONNECTED) {    // WiFi가 연결되어 있을 경우 HTTP 접속
    HTTPClient http;  // HTTPClient 클래스의 오브섹트 선언
    http.begin(wifiClient, "http://220.69.209.111:8008/submit");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST("{\"helmet\":" + String(result) + "}");  // POST 실행

    if (httpCode > 0) { // 정상적으로 요청이 된 경우
      String payload = http.getString();   // 응답을 수신
      //Serial.println(payload);            // 수신된 응답 내용을 시리얼 통신을 통해 출력
    }
    http.end(); // HTTP 접속 종료
  }
  delay(100);
}
