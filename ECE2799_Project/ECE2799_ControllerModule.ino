//ECE 2799 Team 6
//Main Module Code

#include<Wire.h>                          //Library for I2C Communication functions
#define MotorPin 4
const int buttonPin = 12;     // the number of the pushbutton pin
int buttonState = 0;    
int panicAlarm = 0; 

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

    buttonState = digitalRead(buttonPin); //MAYBE USE INTERRUPT? NOT SURE HOW TIMING WILL WORK OUT

  if(panicAlarm == 1){
    digitalWrite(MotorPin, HIGH);   // turn on vibrating motor
  }
  else{
    digitalWrite(MotorPin, LOW);    // turn off vibrating motor
  }
        
}

void receiveEvent (int howMany)                    //This Function is called when Slave receives value from master
{
   panicAlarm = Wire.read();                    //Used to read value received from master and store in variable SlaveReceived
}

void requestEvent()                                //This Function is called when Master wants value from slave
{
  Wire.write(buttonState);                          // sends one byte converted POT value to master
}
