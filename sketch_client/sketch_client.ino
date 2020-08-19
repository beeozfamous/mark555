#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "mark-555.firebaseio.com"
#define FIREBASE_AUTH "HHqscC3tm4NwQWxYpX8pjadJbBGMbfZ64RrHFMZW"
#define WIFI_SSID "Midori Cafe"
#define WIFI_PASSWORD "midoricafe"


// Structure example to receive data
// Must match the sender structure
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
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].AcX = myData.AcX;
  boardsStruct[myData.id-1].AcY = myData.AcY;
  boardsStruct[myData.id-1].AcZ = myData.AcZ;
  boardsStruct[myData.id-1].GyX = myData.GyX;
  boardsStruct[myData.id-1].GyY = myData.GyY;
  boardsStruct[myData.id-1].GyZ = myData.GyZ;
  Serial.printf("AcX value: %d \n", boardsStruct[myData.id-1].AcX);
  Serial.printf("AcY value: %d \n", boardsStruct[myData.id-1].AcY);
  Serial.printf("AcZ value: %d \n", boardsStruct[myData.id-1].AcZ);
  Serial.printf("GyX value: %d \n", boardsStruct[myData.id-1].GyX);
  Serial.printf("GyY value: %d \n", boardsStruct[myData.id-1].GyY);
  Serial.printf("GyZ value: %d \n", boardsStruct[myData.id-1].GyZ);
  Serial.printf(boardsStruct[myData.id-1].AcX + "/AcX");
  //Firebase.setString( myData.id-1+"/AcX",boardsStruct[myData.id-1].AcX);
//  Firebase.setString(String(boardsStruct[myData.id-1].AcY).concat("/AcY"),boardsStruct[myData.id-1].AcY);
//  Firebase.setString(String(boardsStruct[myData.id-1].AcZ).concat("/AcZ"),boardsStruct[myData.id-1].AcZ);
//  Firebase.setString(String(boardsStruct[myData.id-1].GyX).concat("/GyX"),boardsStruct[myData.id-1].GyX);
//  Firebase.setString(String(boardsStruct[myData.id-1].GyY).concat("/GyY"),boardsStruct[myData.id-1].GyY);
//  Firebase.setString(String(boardsStruct[myData.id-1].GyZ).concat("/GyZ"),boardsStruct[myData.id-1].GyZ);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  //SEND DATA
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
      {
    Serial.print(".");
    delay(500);
      }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("0/Test","55as9876");
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
    Serial.println("\nStart to get:");
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){
  // Access the variables for each board
//  int bAcX = boardsStruct[0].AcX;
//  int bAcY = boardsStruct[0].AcY;
//  int bAcZ = boardsStruct[0].AcZ;
//  int bGyX = boardsStruct[0].GyX;
//  int bGyY = boardsStruct[0].GyY;
//  int bGyZ = boardsStruct[0].GyZ;
//  Firebase.setString("2/Test","777");
//  if (Firebase.failed()) {
//      Serial.println(Firebase.error());  
//      return;
//  }
//  delay(1000);
}
