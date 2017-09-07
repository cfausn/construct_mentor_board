#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>

const char* ssid = "RIT-Legacy";//type your ssid


WiFiServer server(80);//Service Port

#define LED D2
#define BUTTON D1
#define RS D3
#define ENABLE D4
#define DB0 D5
#define DB1 D6
#define DB2 D7
#define DB3 D8

boolean wasTripped = true;
int oldButtonState = LOW;
int currentStatus = 0;

LiquidCrystal lcd(RS,ENABLE,DB0,DB1,DB2,DB3);
 
void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("TEST");
  Serial.begin(115200);
  delay(10);

  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
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
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
  
  int buttonState = digitalRead(BUTTON);

  
  
  if(buttonState == HIGH && oldButtonState == LOW){
    if(!wasTripped){
      
      if(currentStatus == 0) currentStatus++;
      else if(currentStatus == 1) currentStatus++;
      else currentStatus = 0;
      
      digitalWrite(LED,HIGH);
      wasTripped = true;  
    }
    else{
      digitalWrite(LED,LOW);
      wasTripped = false;  
    }
    
  }
  oldButtonState = buttonState;

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
