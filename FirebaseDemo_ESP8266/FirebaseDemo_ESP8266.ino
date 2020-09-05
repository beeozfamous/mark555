//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 

// Set these to run example.
#define FIREBASE_HOST "mark-555.firebaseio.com"
#define FIREBASE_AUTH "HHqscC3tm4NwQWxYpX8pjadJbBGMbfZ64RrHFMZW"
#define WIFI_SSID "Hung"
#define WIFI_PASSWORD "hung1234"


// Structure example to receive data
// Must match the sender structure
int n = 0;
int slave_num=4;

typedef struct struct_message {
    int id;
    int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;
struct_message board4;
struct_message board5;
struct_message board6;

// Create an array with all the structures
struct_message boardsStruct[6] = {board1,board2,board3,board4,board5,board6};

// Callback function that will be executed when data is received

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  //first parameter is name of access point, second is the password
  WiFiManager wifiManager;
  wifiManager.autoConnect("[M-A-R-K-<555>]", "danchoithuthiethiphopbatdiet");

  // connect to wifi.
//  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  Serial.print("connecting");
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
//    delay(500);
//  }
//  Serial.println();
//  Serial.print("connected: ");
//  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
    
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  //Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.println(macStr);
  //Serial.print(",");
  memcpy(&myData, incomingData, sizeof(myData));
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].AcX = myData.AcX;
  boardsStruct[myData.id-1].AcY = myData.AcY;
  boardsStruct[myData.id-1].AcZ = myData.AcZ;
  boardsStruct[myData.id-1].GyX = myData.GyX;
  boardsStruct[myData.id-1].GyY = myData.GyY;
  boardsStruct[myData.id-1].GyZ = myData.GyZ;
  //Serial.print(millis()/1000);
  //Serial.print(",");

  Serial.printf("%d",myData.id);
  Serial.print(",");
  Serial.printf("%d",boardsStruct[myData.id-1].AcX);
  Serial.print(",");
  Serial.printf("%d",boardsStruct[myData.id-1].AcY);
  Serial.print(",");
  Serial.printf("%d",boardsStruct[myData.id-1].AcZ);
  Serial.print(",");
  Serial.printf("%d",boardsStruct[myData.id-1].GyX);
  Serial.print(",");
  Serial.printf("%d",boardsStruct[myData.id-1].GyY);
  Serial.print(",");
  Serial.printf("%d",boardsStruct[myData.id-1].GyZ);
  Serial.print('\n');

  
//  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
//  Serial.print(",");
//  Serial.printf("AcX value: %d \n", boardsStruct[myData.id-1].AcX);
//  Serial.print(",");
//  Serial.printf("AcY value: %d \n", boardsStruct[myData.id-1].AcY);
//  Serial.print(",");
//  Serial.printf("AcZ value: %d \n", boardsStruct[myData.id-1].AcZ);
//  Serial.print(",");
//  Serial.printf("GyX value: %d \n", boardsStruct[myData.id-1].GyX);
//  Serial.print(",");
//  Serial.printf("GyY value: %d \n", boardsStruct[myData.id-1].GyY);
//  Serial.print(",");
//  Serial.printf("GyZ value: %d \n", boardsStruct[myData.id-1].GyZ);
//  Serial.printf(boardsStruct[myData.id-1].AcX + "/AcX");
//  //Firebase.setString( myData.id-1+"/AcX",boardsStruct[myData.id-1].AcX);
//  Firebase.setFloat(String(boardsStruct[myData.id-1].AcY+"/AcY"),boardsStruct[myData.id-1].AcY);
//  Firebase.setFloat(String(boardsStruct[myData.id-1].AcZ+"/AcZ"),boardsStruct[myData.id-1].AcZ);
//  Firebase.setFloat(String(boardsStruct[myData.id-1].GyX+"/GyX"),boardsStruct[myData.id-1].GyX);
//  Firebase.setFloat(String(boardsStruct[myData.id-1].GyY+"/GyY"),boardsStruct[myData.id-1].GyY);
//  Firebase.setFloat(String(boardsStruct[myData.id-1].GyZ+"/GyZ"),boardsStruct[myData.id-1].GyZ);
//  if (Firebase.failed()) {
//      Serial.println("Firebase error");  
//      Serial.println(Firebase.error());  
//      return;
//  }
  
}

void loop() {
//  //Serial.print(millis()/1000);
//  for(int count = 0 ; count<slave_num;count++){
////    Serial.print(",");
////    Serial.print("ID"+count);
//    if(boardsStruct[count].AcX!=0){
//      Firebase.setFloat(String("landmark"+String(count)+"/AcX"),boardsStruct[count].AcX);
////      Serial.print(",");
////      Serial.print(boardsStruct[count].AcX);
//    }
//    if(boardsStruct[count].AcY!=0){
//      Firebase.setFloat(String("landmark"+String(count)+"/AcY"),boardsStruct[count].AcY);
////      Serial.print(",");
////      Serial.print(boardsStruct[count].AcY);
//    }
//    if(boardsStruct[count].AcZ!=0){
//      Firebase.setFloat(String("landmark"+String(count)+"/AcZ"),boardsStruct[count].AcZ);
////      Serial.print(",");
////      Serial.print(boardsStruct[count].AcZ);
//    }
//    if(boardsStruct[count].GyX!=0){
//      Firebase.setFloat(String("landmark"+String(count)+"/GyX"),boardsStruct[count].GyX);
////      Serial.print(",");
////      Serial.print(boardsStruct[count].GyX);
//    }
//    if(boardsStruct[count].GyY!=0){
//      Firebase.setFloat(String("landmark"+String(count)+"/GyY"),boardsStruct[count].GyY);
////      Serial.print(",");
////      Serial.print(boardsStruct[count].GyY);
//    }
//    if(boardsStruct[count].GyZ!=0){
//      Firebase.setFloat(String("landmark"+String(count)+"/GyZ"),boardsStruct[count].GyZ);
////      Serial.print(",");
////      Serial.print(boardsStruct[count].GyZ);
//    }
//  }
}
