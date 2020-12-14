#include <WiFiEspAT.h>

#include <SoftwareSerial.h>
SoftwareSerial ESPserial(2, 3); // RX, TX

#define AT_BAUD_RATE 9600
#define dirPin1 5
#define stepPin1 4
#define dirPin2 7
#define stepPin2 6
#define enablePin1 8
#define enablePin2 9
#define motorInterfaceType 1

int open_bin = 0;
WiFiServer server(80);

void setup() {

  // Begin the serial data transmission
  Serial.begin(9600);
  while (!Serial);

  // Begin transmission to the ESP8266 chip
  ESPserial.begin(9600);
  WiFi.init(ESPserial);

  // Connect to WIFI
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // waiting for connection to Wifi network set with the SetupWiFiConnection sketch
  Serial.println("Waiting for connection to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println();

  // Print out info about the wifi connection
  IPAddress ip = WiFi.localIP();
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Begin the HTTP server and send the command to ESP8266 to wait to recieve data
  server.begin();
  ESPserial.write("+IPD,0,32:");
  Serial.println("HTTP server started");

  // Set the needed pins for output and set inital values
  pinMode(stepPin1,OUTPUT); 
  pinMode(dirPin1,OUTPUT);
  pinMode(stepPin2,OUTPUT); 
  pinMode(dirPin2,OUTPUT);
  pinMode(enablePin1,OUTPUT); 
  pinMode(enablePin2,OUTPUT);
  digitalWrite(dirPin1,HIGH);
  digitalWrite(dirPin2,HIGH);
  digitalWrite(enablePin1,HIGH);
  digitalWrite(enablePin2,HIGH);
}

void loop() {
  // Wait until there is data avalible on the ESP8266 chip
  if (ESPserial.available()) {
    String ESPMessage = "";
    
    // Read the avalible data
    while (ESPserial.available()) {
      ESPMessage = ESPserial.readString();
    }

    // Get the position of the string Host in the message
    int hostPos = ESPMessage.indexOf("Host");

    // If Host appears in the message, we want to process this message
    if (hostPos > 0) {
      // We already have all the data so we can disconnect from the client
      disconnect_from_client();

      // Get the position of th string open in the message
      int openPos = ESPMessage.indexOf("open");

      // Get the position of th string close in the message
      int closePos = ESPMessage.indexOf("close");

      // If open appears in the message, we can process as an open command
      if (openPos > 0) {
        processOpen(ESPMessage, openPos);
      } else if (closePos > 0) {
        // If close appears in the message, we can process as an open command
        processClose();
      } else {
        // This was not an open or close request
        Serial.println("There was an error");
      }
      Serial.println();
    }
  }
}

void disconnect_from_client() {
  ESPserial.println("AT+CIPCLOSE=0");
  while (ESPserial.available()) {
    ESPserial.readString();
  }
  ESPserial.write("+IPD,0,32:");
  Serial.println("client disconnected");
}

void processOpen(String ESPMessage, int pos) {
  int box_id = ESPMessage.substring(pos+5, pos+6).toInt();
  if (open_bin == box_id) {
    Serial.println("Box is already open");
  } else if (open_bin == 0) {
    Serial.print("Opening box ");
    Serial.println(box_id);
    handleOpen(box_id);
    open_bin = box_id;
  } 
}

void processClose() {
  if (open_bin == 0) {
    Serial.println("There is no open bin");
  } else {
    Serial.print("Closing box ");
    Serial.println(open_bin);
    handleClose(open_bin);
    open_bin = 0;
  }
}

void handleOpen(int boxNum) {
  if (boxNum == 1) {
    StepMotor(dirPin1, stepPin1, enablePin1, 1);
    Serial.println("opened box 1");
  } else if (boxNum == 2) {
    StepMotor(dirPin2, stepPin2, enablePin2, 1);
    Serial.println("opened box 2");
  } else {
    Serial.println("box does not exist");
  }
}

void handleClose(int boxNum) {
  if (boxNum == 1) {
    StepMotor(dirPin1, stepPin1, enablePin1, 2);
    Serial.println("closed box 1");
  } else if (boxNum == 2) {
    StepMotor(dirPin2, stepPin2, enablePin2, 2);
    Serial.println("closed box 2");
  } else {
    Serial.println("box does not exist");
  }
}

void StepMotor(int dirP, int stepP, int enablePin, int direct)
{
  if (direct == 1)
    digitalWrite(dirP, HIGH); // Open Box
  else
    digitalWrite(dirP, LOW); // Close box
    
  digitalWrite(enablePin,LOW);
  for(int x= 0; x<350; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stepP,HIGH); //Trigger one step forward
    delay(5);
    digitalWrite(stepP,LOW); //Pull step pin low so it can be triggered again
    delay(5);
  }
  digitalWrite(enablePin,HIGH);
  Serial.println("Stopped stepping");
}
