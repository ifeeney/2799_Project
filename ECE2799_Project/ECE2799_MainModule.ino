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
      inches = (duration/1000000) * 890 * 12;
   
      //Check if measurements are in bounds
      if((inches > 2) && (inches < 100)){
        samples ++;
        distSum += inches;
      }
     
  }

  avgDist = distSum/5;
  Serial.print("avgDist: ");
  Serial.println(avgDist);

  if(firstRun){
    prevDist = avgDist;
    firstRun = false;
  }

  Serial.print("prevDist: ");
  Serial.println(prevDist);
  
  //If this distance is 1 inch closer than previous or more, send alert
  if(avgDist < (prevDist-1)){
   
         //Process PIR Input
         if (digitalRead(PIR) == HIGH) { // check if the sensor is HIGH
           Serial.println("Motion detected!");
           delay(1000); //Delay primarily for debugging and readability purposes
           //Write to the controller at address 1
           Wire.beginTransmission(1);                          
           Wire.write(1);                      
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
  if(panicStatus == 1){
    tone(speakerPin, 3000);           // 3kHz buzzing (about 80dB @10cm)
  }else{
    noTone(speakerPin);
  }

}
