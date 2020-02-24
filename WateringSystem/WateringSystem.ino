/*********************************************************
* ES1 Week 6 and 7 Assignment
*
* Course : Embedded Systems 1 (ES1)
* Assignment: ES1 Final Project Assignment
* Date : 1-04-2019
* Student(s) : Sam Jarvis (3734811)
*              Alhazmi Yasser (3529495)
*
*********************************************************/
#include <Servo.h>

/*** Function declarations ***/

//Turn the machine on or off based on the boolean machineIsOn
void turnMachine();

//Button debounce that takes the button pin number and debounce time in milliseconds as arguments
boolean buttonIsPressed(int, int);

//Rotate the servo and with every rotation invoke detectPlant()
void rotateServo();

//Use PING to detect if a plant exist
void detectPlant();

//Check if the user changed the watering time interval and set the new interval if necessary
void checkTimeInterval(int);

//Convert the potentiometer mapped value to milliseconds
long int hourToMilliSeconds(int);

//Change the RGB LED color based on passed value
void changeRgbColorByValue(int, int, int)

/*** PINS and Program Constants ***/
//Potentiometer PIN
const int POTENTIOMETERPIN = A0;

//Button pin
const int BUTTONPIN = 2;

// Blue channel pin (on the LED)
const int BLUEPIN = 6;

// Green channel pin (on the LED)
const int GREENPIN = 5;

// Red channel pin (on the LED)
const int REDPIN = 3;

//LED Pins
const int LEDPINHIGH = 10;
const int WATERINGLED = 7;

//Servo pin
const int SERVOPIN = 12;

//Ping ECHO pin
const int ECHOPIN = 8;

//Ping TRIGGER pin
const int TRIGPIN = 9;

//debounce time for the debounce functions
const int debounceTime = 70;

/*** Global variables ***/

//To determine the machine state
boolean machineIsOn;

//Time to wait after one watering round
int timeInterval;

//Servo object to control the servo
Servo myservo; 


void setup() {
  //LEDs, button, and potentiometer are set to the appropriate pinMode
  pinMode(BUTTONPIN, INPUT); 
  pinMode(LEDPINHIGH, OUTPUT);
  pinMode(WATERINGLED, OUTPUT);
  pinMode(POTENTIOMETERPIN, INPUT);

  //PING sensor
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);

  //RGB LED channels
  pinMode(BLUEPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);

  //Sevo
  myservo.attach(SERVOPIN);  // attaches the servo on pin 9 to the servo object

  //Serial
  Serial.begin(9600);
}

void loop() {

  // Turn machine on if button is pressed
  if (buttonIsPressed(BUTTONPIN, debounceTime)) { //Turn the machine ON
    turnMachine();
  }

  delay(50);

  //If machine is turned on, start looking for plants to water
  while (machineIsOn) {
    rotateServo();
    if (buttonIsPressed(BUTTONPIN, debounceTime)) {
      //If button is cliked and machine is on, turn it off
      if (machineIsOn) {
        turnMachine();
        return;
      }
    }

}
}// end loop()



//Check if the user changed the watering time interval and set the new interval if necessary
void checkTimeInterval(int oldTimeInterval) {
  int checkTimeInterval = map (analogRead(POTENTIOMETERPIN), 0, 1023, 1, 12); //obtain a new read and map it
  if (checkTimeInterval != oldTimeInterval) { //If the user changed watering interval
    timeInterval = checkTimeInterval;   //Update the watering interval
    Serial.print("New read: ");   //Print the new value
    Serial.println(checkTimeInterval);  //Print the new value
  } else {
    Serial.println("Potentiometer value not changed");
  }
  if (buttonIsPressed(BUTTONPIN, debounceTime)) {
    if (machineIsOn) {
      turnMachine();
      return;
    }
  }
  delay(100);
}


//Rotate the servo and with every rotation invoke detectPlant()
void rotateServo() {
  static int i = 0; // The current angle of the servo
  static boolean TurnOpposite = false; //Turn to the opposite direction if true
  while (machineIsOn) {
    if (i >= 180) {
      TurnOpposite = true;
      delayWatering(); //we have watered all the plants, now we wait for the interval the user specified before watering again
    }
    else if (i == 0) {
      TurnOpposite = false;
      delayWatering(); //we have watered all the plants, now we wait for the interval the user specified before watering again
    }

    if (TurnOpposite == true) {
      i -= 20; //Set the degrees by which the servo will move 
    }
    else if (TurnOpposite == false) {
      i += 20; //Set the degrees by which the servo will move 
    }
    myservo.write(i); //Move the servo
    if (buttonIsPressed(BUTTONPIN, debounceTime)) {
      if (machineIsOn) {
        turnMachine();
        return;
      }
    }
    //servo is moved, let's see if we can find a plant
    checkTimeInterval(timeInterval);
    detectPlant();
  }
}

void detectPlant() {

  int MindistanceToPlant = 4, distance, MaxDistanceToPlant = 6; //define minimun and maximum distances in cms
  
  digitalWrite(TRIGPIN, LOW);                   // Set the trigger pin to low for 2uS
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);                  // Send a 10uS high to trigger ranging
  //Calibrate this for better detection
  delay(1000);
  digitalWrite(TRIGPIN, LOW);                     // Send pin low again

  distance = pulseIn(ECHOPIN, HIGH);        // Read in times pulse
  distance = distance / 58;                     // Calculate distance from time of pulse
  Serial.println(distance);

  if (distance >= MindistanceToPlant && distance <= MaxDistanceToPlant ) {
    //Watering. Let's turn the internal LED on for now
    digitalWrite(WATERINGLED, HIGH);
    //digitalWrite(WATERINGLOW, LOW);
    delay(2000);
    digitalWrite(WATERINGLED, LOW);
  } else {
    digitalWrite(WATERINGLED, LOW);
  }
  if (buttonIsPressed(BUTTONPIN, debounceTime)) {
    if (machineIsOn) {
      turnMachine();
      return;
    }
  }
}


void turnMachine() {
  if (!machineIsOn) {
    timeInterval = map(analogRead(POTENTIOMETERPIN), 0, 1023, 1, 12); //Initial read (from previous setup) of the timing interval
    Serial.println("Machine state: ON");
    machineIsOn = true;
    digitalWrite(LEDPINHIGH, HIGH); //Turning green LED on
    delay(200);
    return;
  }
  //The machine should be turned off
  Serial.println("Machine state: OFF");
  machineIsOn = false;
  //Turn all LEDs off
  changeRgbColorByValue(LOW, LOW, LOW);  
  digitalWrite(LEDPINHIGH, LOW);
  digitalWrite(WATERINGLED, LOW);
  delay(200);
  
}

long int hourToMilliSeconds(int hour) { //Hour is the mapped potentiometer value
  long int convertedTime = hour * 60 * 60 * 1000;
  Serial.print("Time in hours is: ");
  Serial.print(hour);
  Serial.print(". This is ");
  Serial.print(convertedTime);

  return convertedTime;
}

//Watering will be delayed by the given time interval
void delayWatering()
{
  int red = 0;
  int blue = 0;
  int green = 255;
  int counter = 0;
  long int delayTime = hourToMilliSeconds(timeInterval / 255); //Time to wait is divided by the RGB color spectrum to take x time to move from green to red

  while (counter <= 255)
  {
    counter++;
    green -= 1;
    red += 1;
    changeRgbColorByValue(red, green, blue);
    delay(delayTime);
    if (buttonIsPressed(BUTTONPIN, debounceTime)) {
      turnMachine();
      return;
    }
  }
}

//Change the RGB LED color based on passed value
void changeRgbColorByValue(int rValue, int gValue, int bValue)
{
  analogWrite(REDPIN, rValue);
  analogWrite(BLUEPIN, bValue);
  analogWrite(GREENPIN, gValue);
  delay(50);
}


boolean buttonIsPressed(int BUTTONPIN, int debounceCheckTimeMilliSeconds)
{
  if (digitalRead(BUTTONPIN) == LOW)
  {
    return LOW;
  }
  delay(debounceCheckTimeMilliSeconds);
  if (digitalRead(BUTTONPIN) == HIGH)
  {
    return HIGH;
  }
  return LOW;
}
