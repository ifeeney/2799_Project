//ECE 2799 Team 6
//Main Module Code

#include <Wire.h> 
int PIR = 3;              // the pin that the PIR is attached to
int pingPin = 2;          // the pin that the ultrasonic sensor is attached to
int speakerPin = 4;       // the pin that the speaker is attached to 
bool firstRun = true;

void setup() {
 pinMode(PIR, INPUT);         // initialize sensor as an input
 pinMode(speakerPin, OUTPUT); // initalize speaker as an output
 Serial.begin(9600);          // initialize serial
 Wire.begin();                // initalize i2c communication 
}


void loop(){
  
  //Process Ultrasonic Sensor 
  float duration, inches;
  float distSum = 0;
  
  //Take 5 samples
  int samples = 0;
  while(samples<5){

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
      inches = (duration/1000000) * 890;
    
      //Check if measurements are in bounds
      if((inches < 2) && (inches > 100)){
        samples ++;
        distSum += inches; 
      }
      
  }

  if(firstRun){
    float prevDist = avgDist;
    firstRun = false;
  }

  float avgDist = distSum/5;

  //If this distance is closer than previous, send alert 
  if(avgDist < prevDist){
    
         //Process PIR Input
         if (digitalRead(PIR) == HIGH) { // check if the sensor is HIGH
           Serial.println("Motion detected!"); 
           
           //Write to the controller at address 1
           Wire.beginTransmission(1);                          
           Wire.write(MasterSend);                       
           Wire.endTransmission();   
         } 
         
         else {
           Serial.println("Motion stopped!");
         }
  }

  prevDist = avgDist; 

  //Read in controller
  wire.requestFrom(contorller, 1);
  byte panicStatus = wire.read();
  if(PanicStatus == 1){
    tone(SpeakerPin, 3000);           // 3kHz buzzing (about 80dB @10cm)
  }

}



