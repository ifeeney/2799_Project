//ECE 2799 Team 6
//Main Module Code

#include <Wire.h>
int PIR = 3;              // the pin that the PIR is attached to
int pingPin = 2;          // the pin that the ultrasonic sensor is attached to
int speakerPin = 4;       // the pin that the speaker is attached to
bool firstRun = true;
byte panicStatus;
float prevDist;
float avgDist;
enum State_enum {SENSE, ALERT};
int state = 0;           //IDK what data type this should be
float takeSamples();

void setup() {
  pinMode(PIR, INPUT);         // initialize sensor as an input
  pinMode(speakerPin, OUTPUT); // initalize speaker as an output
  Serial.begin(9600);          // initialize serial
  Wire.begin();                // initalize i2c communication
}


void loop() {
  
  switch (state)
  {
    case SENSE:
        avgDist = takeSamples();
        Serial.print("avgDist: ");
        Serial.println(avgDist);

        if (firstRun) {
          prevDist = avgDist;
          firstRun = false;
        }

        //Serial.print("prevDist: ");
        //Serial.println(prevDist);

        //If this distance is 1 inch closer than previous or more, send alert
        if (avgDist < (prevDist - 1)) {

          //Process PIR Input
          if (digitalRead(PIR) == HIGH) { // check if the sensor is HIGH
            Serial.println("Motion detected!");
            delay(1000);
            //Write to the controller at address 1
            Wire.beginTransmission(1);
            Wire.write(1);
            Wire.endTransmission();
          }
        }

        prevDist = avgDist;

        //Read in controller
        Wire.requestFrom(1, 1);
        panicStatus = Wire.read();
        if (panicStatus == 1) {
          tone(speakerPin, 3000);           // 3kHz buzzing (about 80dB @10cm)
          state = ALERT;
        }
        break; 

      case ALERT:
        while (panicStatus == 1) {
          Wire.requestFrom(1, 1);
          panicStatus = Wire.read();
        }
        noTone(speakerPin);
        state = SENSE;
        break;
      }
  }



float takeSamples() {
  //Process Ultrasonic Sensor
  float duration, inches;
  float distSum = 0;

  //Take 5 samples
  int samples = 0;
  while (samples < 5) {

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
    inches = (duration / 1000000) * 890 * 12;

    //Check if measurements are in bounds
    if ((inches > 2) && (inches < 100)) {
      samples ++;
      distSum += inches;
    }
  }
  return (distSum / 5);
}
