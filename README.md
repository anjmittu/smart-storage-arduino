# smart-storage-arduino

This repo contains the Ardunio files for the Smart Storage system.

## smart-storage
The smart-storage file is the main program for this application.  The schematic below shows how the Ardiuno should be
wired for the system to run.

Requirements:
* Arduino
* ESP8266 wifi module
* Two A4988stepper motor drivers
* Two stepper motors

![Alt text](images/Smart Storage_bb.png?raw=true "schematic")

Before running the program the ESP8266 needs to be configured to work with your wifi.  Run the test_esp8266
program and send the following commands in the console.

1.Verify that the ESP8266 is connected properly.
  
  Command to send: AT
  
  Expected response: OK
  
2. Change the mode.
   
   Command to send: AT+CWMODE=3
   
   Expected response: OK
   
3. See the list of all the nearby WiFi Networks

    Command to send: AT+CWLAP

4. Connect to your router (Make sure to replace YOUR_SSID and YOUR_WIFI_PASSWORD).
   
   Command to send: AT+CWJAP=”YOUR_SSID”,”YOUR_WIFI_PASSWORD”
   
   Expected response:
   
   WIFI CONNECTED
   
   WIFI GOT IP
   
   OK
   
5. Set baud rate to 9600.
   
   Command to send: AT+UART=9600,8,1,0,0
   
   Expected response: OK
   
6. Verify that the ESP8266 is communicating with baud rate of 9600.
   
   Command to send: AT
   
   Expected response: OK

## test_esp8266
This program lets you send commands to the ESP8266 chip.  This is useful to check that the chip
is wired correctly and to configure it for use.

## testmotor
This program will spin the motor so that you can check that the chips are wired correctly.