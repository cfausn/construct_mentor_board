#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>

const char* ssid = "RIT-Legacy";//type your ssid


WiFiServer server(80);//Service Port

#define STATUS_PIN_UP D0
#define STATUS_PIN_DOWN D1
#define NAME_PIN_UP D2
#define NAME_PIN_DOWN D3
#define LED D4

#define NUM_STATUSES 3
#define NUM_MENTORS 7
String mentors[NUM_MENTORS]  = {"Andrew","Colin","Esther","Mark","Matt","Michelle","Myles"};

int status_index = 1;
int mentor_index = 0;


boolean wasTripped = true;
int currentStatus = 0;
int oldButtonState = LOW;
int lastStatusUp = LOW;
int lastStatusDown = LOW;
int lastNameUp = LOW;
int lastNameDown = LOW;

 
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


  if(statusUp == HIGH) lastStatusUp = HIGH;   
  else if(lastStatusUp == HIGH){
    lastStatusUp = LOW;
    
    if(status_index < NUM_STATUSES - 1) status_index++;
    else status_index = 0;
    
    Serial.println("Status Up");
  }
  else lastStatusUp = LOW;

  
  if(statusDown == HIGH) lastStatusDown = HIGH;
  else if(lastStatusDown == HIGH){
    lastStatusDown = LOW;
    
    if(status_index > 0) status_index--;
    else status_index = 2;
        
    Serial.println("Status Down");
  }
  else lastStatusDown = LOW;
  
  if(nameUp == HIGH) lastNameUp = HIGH;
  else if(lastNameUp == HIGH){
    lastNameUp = LOW;
    if(mentor_index < NUM_MENTORS -1) mentor_index++;
    else mentor_index = 0;
    Serial.println("Name Up");
  }
  else lastNameUp = LOW;
  
  if(nameDown == HIGH) lastNameDown = HIGH;
  else if(lastNameDown == HIGH){
    
    if(mentor_index > 0) mentor_index--;
    else mentor_index = 6;
    
    lastNameDown = LOW;
    Serial.println("Name Down");
  }
  else lastNameDown = LOW;

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
  client.print(mentors[mentor_index]);
  client.print(",");
  client.print(status_index);
  
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
