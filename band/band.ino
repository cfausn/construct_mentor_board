#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>

const char* ssid = "RIT-Legacy";//type your ssid


WiFiServer server(80);//Service Port

#define STATUS_PIN_UP D0
#define STATUS_PIN_DOWN D1
#define NAME_PIN_UP D2
#define NAME_PIN_DOWN D3
#define LED D4


boolean wasTripped = true;
int currentStatus = 0;
int oldButtonState = LOW;
int currentStatusUP = LOW;
int currentStatusDOWN = LOW;
int currentNameUP = LOW;
int currentNameDOWN = LOW;

 
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(LED, OUTPUT);
  pinMode(STATUS_PIN_UP, INPUT);
  pinMode(STATUS_PIN_DOWN, INPUT);
  pinMode(NAME_PIN_UP, INPUT);
  pinMode(NAME_PIN_DOWN, INPUT);
  digitalWrite(LED, LOW);
   
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}
 
void loop() {
  
  int statusUp = digitalRead(STATUS_PIN_UP);
  int statusDown = digitalRead(STATUS_PIN_DOWN);
  int nameUp = digitalRead(NAME_PIN_UP);
  int nameDown = digitalRead(NAME_PIN_DOWN);


  if(statusUp == HIGH) Serial.println("Status Up");
  if(statusDown == HIGH) Serial.println("Status Down");
  if(nameUp == HIGH) Serial.println("Name Up");
  if(nameDown == HIGH) Serial.println("Name Down");

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
  // Match the request
 
  //Set ledPin according to the request
  //digitalWrite(ledPin, value);
   
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print(currentStatus);
  
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
