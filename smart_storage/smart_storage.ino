#include <WiFiEspAT.h>
//#define USE_WIFI_NINA 0
//#include <WiFiWebServer.h>
//#include <AccelStepper.h>

// Emulate Serial1 on pins 6/7 if not present
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

// Create a new instance of the AccelStepper class:
//AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

WiFiServer server(80);

void setup() {

  Serial.begin(9600);
  while (!Serial);

  ESPserial.begin(9600);
  WiFi.init(ESPserial);

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

//  server.begin();

  IPAddress ip = WiFi.localIP();
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

//  server.on("/open", handleOpen);

//  server.onNotFound(handleNotFound);

  server.begin();
  ESPserial.write("+IPD,0,32:");
  Serial.println("HTTP server started");

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
  if (ESPserial.available()) {
    String ESPMessage = "";
    while (ESPserial.available()) {
      ESPMessage = ESPserial.readString();
//      Serial.write(ESPMessage.c_str());
    }

    int hostPos = ESPMessage.indexOf("Host");

    if (hostPos > 0) {
      disconnect_from_client();
      
      int pos = ESPMessage.indexOf("open");
  
      if (pos > 0) {
        // This api call is to open the box
        int box_id = ESPMessage.substring(pos+5, pos+6).toInt();
        if (open_bin == box_id) {
          Serial.println("Box is already open");
        } else if (open_bin == 0) {
          Serial.print("Opening box ");
          Serial.println(box_id);
          handleOpen(box_id);
          open_bin = box_id;
        } 
      } else {
        pos = ESPMessage.indexOf("close");
        if (pos > 0) {
          if (open_bin == 0) {
            Serial.println("There is no open bin");
          } else {
            Serial.print("Closing box ");
            Serial.println(open_bin);
            handleClose(open_bin);
            open_bin = 0;
          }
        } else {
          Serial.println("There was an error");
        }
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
