//ECE 2799 Team 6
//Main Module Code

#include<Wire.h>                          //Library for I2C Communication functions
#define buttonPin 2
#define motorPin 4      
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int motorON;

void setup() {
  // put your setup code here, to run once:
  pinMode(MotorPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600); 
  Wire.begin(1);                          //Begins I2C communication with Slave Address as 8 at pin (A4,A5)
  Wire.onReceive(receiveEvent);           //Function call when Slave receives value from master
  Wire.onRequest(requestEvent);           //Function call when Master request value from Slave

}

void loop() {
  // put your main code here, to run repeatedly:
int reading = digitalRead(buttonPin);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
    }

    if (buttonState == 1) {
      if (motorON) {
        digitalWrite(motorPin, LOW);
      }
    }
  }
  lastButtonState = reading;
        
}

void receiveEvent (int howMany)                    //This Function is called when Slave receives value from master
{
    byte alarm = Wire.read();                      //Used to read value received from master and store in variable alarm
    if (alarm) {
      digitalWrite(motorPin, HIGH);
      motorON = 1;
      Serial.println("Motor ON");  
    }
}

void requestEvent()                                //This Function is called when Master wants value from slave
{
  Wire.write(lastButtonState);                          // sends one byte converted POT value to master
}







