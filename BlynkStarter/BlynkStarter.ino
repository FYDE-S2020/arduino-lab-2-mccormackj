#include <Blynk.h>

/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */

//SHOULD BE REAL LED


#define BLYNK_PRINT Serial
#define LED 0

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//V0=button
//V1=switch
//V3=Terminal
//V4=Display

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "CFOu1e3EYI7ynXzjeZD4Hc6EpAiHaNiU";

char ssid[32] = "Jo_iPhone";
char pass[32] = "tutoring";


BlynkTimer timer;         //good
int dutyCycle = 0;        //good
boolean ledState=false;   //good
int time_count = 0;

char character;
String content = "";


void myTimerEvent(){
  if(time_count %100 ==0){
    Blynk.virtualWrite(V4, millis()/1000);
    time_count = 0;
  }
  while(Serial.available()){
    character=Serial.read();
    content.concat(character);
  }
  if (content != ""){
    Blynk.virtualWrite(V3, content);
    content = "";
  }
  time_count +=1;
}

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  //configure LED PWM functionality
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);
  timer.setInterval(10L, myTimerEvent);
  pinMode(0, OUTPUT);
}

void loop()
{
  Blynk.run();
  timer.run();
  //Turn the LED on and off with the button
  //Change the LED brightness with the slider (note the button interaction)
  //Display a message typed on the Arduino Serial Monitor on the Blynk terminal
  //Display the uptime of the ESP32 in seconds
}


BLYNK_WRITE(V0)
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int pinValue = param.asInt(); // assigning incoming value from pin V0 to a variable

    // Because V0 is a button, pinValue will be a 0 or a 1.
    if (pinValue == 0) {
         ledcWrite(ledChannel, 0);
         ledState=false;
    }
    else {
         ledcWrite(ledChannel, dutyCycle);
         ledState=true;
    }
}

BLYNK_WRITE(V1){
  if (ledState==true){
  dutyCycle = param.asInt();
  ledcWrite(ledChannel, dutyCycle);
  }
}
