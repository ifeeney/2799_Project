//ECE 2799 Team 6
//Main Module Code

#include <Wire.h> 
int PIR = 3;              // the pin that the PIR is attached to
int pingPin = 2;          // the pin that the ultrasonic sensor is attached to
int speakerPin = 4;       // the pin that the speaker is attached to 
bool firstRun = true;
float prevDist;
float avgDist;

void setup() {
 pinMode(PIR, INPUT);         // initialize sensor as an input
 pinMode(speakerPin, OUTPUT); // initalize speaker as an output
 Serial.begin(500000);          // initialize serial
 Wire.begin();                // initalize i2c communication 
}


void loop(){
  
  //Process Ultrasonic Sensor 
  float duration, inches;
  float thisSample = 0;
  Serial.println("BEGIN");
  int samples = 0;
  while(samples<200){ //take 200 samples when averaging 5
      thisSample = takeSamples();
      Serial.println(thisSample); //print averaged sample
      samples++;
  }

  while(1){ //stop taking samples after enough are collected
  }

}

float takeSamples() {
  //Process Ultrasonic Sensor
  float duration, cm;
  float distSum = 0;

  //Take 5 samples
  int samples = 0;
  while (samples < 5) { //take 3 or 5 samples
  
//trigger PING pulse
      pinMode(pingPin, OUTPUT);
      digitalWrite(pingPin, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin, HIGH);
      delayMicroseconds(5);
      digitalWrite(pingPin, LOW);

     // Read in signal from PING pin
      pinMode(pingPin, INPUT);
      duration = pulseIn(pingPin, HIGH);

    // convert the time into a distance using conversions found on datasheet
    cm = duration/29/2;

    //Check if measurements are in bounds
    if ((cm > 10) && (cm < 300)) {
      Serial.println("sample added"); //necessary print line
      samples ++;
      distSum += cm;
    }
  }
  return (distSum / 5);
}
