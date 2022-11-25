#include <Servo.h>
Servo leftServo;    //Starts up Servos
Servo rightServo;
const int leftStop = 84;    
const int rightStop = 78;
const int PBL = 4;    //Defines button value
const int PBR = 2;
const int IR = 3;   
#define leftSensor A2   //Defines light sensors
#define middleSensor A1
#define rightSensor A0
bool turnRight = false;   //Direction after reading barcode
bool turnLeft = false;
int codeLines = 0;    //Amount of lines on the barcode

const int rightWhite = 467;   //Sets the lighting values
const int rightBlack = 212;
const int rightGrey = 336;
const int leftWhite = 454;    
const int leftBlack = 260;
const int leftGrey = 364;
const int middleWhite = 360;    
const int middleBlack = 180;
const int middleGrey = 317;



void stop() {   //Runs stop values
  leftServo.write(leftStop);
  rightServo.write(rightStop);
}

int setSpeed(int a, int b) {    //Takes speed values
  int leftServoVal = leftStop + a;    //Adds speed to Servo
  int rightServoVal = rightStop - b - 1;
  leftServo.write(leftServoVal);
  rightServo.write(rightServoVal);
}

void moveStraight(int dist) {
  setSpeed(10, 10);
  delay(dist * 207.32);   //Time taken to move 1cm multiplied by requested distance
  stop();
}

void turnAngle(int angle) {
  if (angle < 0) {    //Checks which direction the robot will turn
    setSpeed(-10, 10);    //Turns left
    angle = abs(angle);   //makes value positive
  }
  else {
    setSpeed(10, -10);    //Turns right
  }
  delay(28.72 * angle);   //Time taken to turn 1° multiplied by requested angle
  stop();
}

void waitKey (int pin) {    //Waits for button to be let go
  while (digitalRead(pin) == HIGH) {}
  delay(40);
  while (digitalRead(pin) == LOW) {}
  delay(40);
  return;
}

void turn(int value1, int value2) {   //Takes in which sensors to use
  if (analogRead(rightSensor) < value1 + 20 and analogRead(rightSensor) > value1 - 20) {    //Creates light sensor range 
    turnAngle(10);
  }

  else if (analogRead(leftSensor) < value2 + 20 and analogRead(leftSensor) > value2 - 20) {
    turnAngle(-10);
  }
}

void obstacleAvoid(){
      turnAngle(90);    //Moves robot right and forward
    moveStraight(30);
    turnAngle(-90);   //Checks to see if there is still an object
    int d = 30;   //Distance to travel back
    while (digitalRead(2) == LOW) {
      turnAngle(90);    //Continues until there is no object
      moveStraight(30);
      turnAngle(-90);
      d = d + 30;   //Adds to the travel back distance
    }
    moveStraight(40);
    turnAngle(-90);
    while (digitalRead(2) == LOW) {   //Moves along the outside of the object
      turnAngle(90);
      moveStraight(40);
      turnAngle(-90);
    }
    moveStraight(d);    //Moves back to the line
    turnAngle(90);
}

void setup() {
  Serial.begin(9600);   //Sets up Serial
  leftServo.attach(6);    //Attaches servos
  rightServo.attach(5);
  pinMode(PBL, INPUT);    //Sets pins to be inputs
  pinMode(PBR, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(middleSensor, INPUT);
  pinMode(leftSensor, INPUT);
  pinMode(IR, OUTPUT);
  tone(3, 38000);
  setSpeed(0, 0);   //Sets speed to 0 at start
  waitKey(PBL);   //Waits for the button to be pressed before running the loop
  Serial.println("Start");
}

void loop() {

  setSpeed(10, 10);

  if (digitalRead(2) == LOW) {    //Checks front sensor
    Serial.println("Obstacle detected");
    obstacleAvoid();

  }

  if ((analogRead(middleSensor) > middleWhite - 20) and (turnRight == false)) {   //Checks if the middle sensor is touching white
    turn(rightBlack, leftBlack);
  }

  if ((analogRead(middleSensor) > middleWhite - 20) and (turnRight == true) and (turnLeft == false)) {    //Checks if the middle sensor is touching white and turning right
    turn(rightGrey, leftGrey);
  }

  if ((analogRead(middleSensor) > middleGrey - 50) and (turnRight == false) and (turnLeft == true)) {   //Checks if the middle sensor is touching grey and turning left
    turn(rightBlack, leftBlack);
  }
  
  //Checks if the outer light sensors are both on black
  if ((analogRead(rightSensor) < rightBlack + 40 and analogRead(rightSensor) > rightBlack - 40) && (analogRead(leftSensor) < leftBlack + 40 and analogRead(leftSensor) > leftBlack - 40)) {
    unsigned long fixedTime = millis() + 8000;    //Makes 8 second delay
    while (millis() < fixedTime) {    //Loops for 8 seconds
      //Checks if the outer light sensors are both on black
      if ((analogRead(rightSensor) < rightBlack + 40 and analogRead(rightSensor) > rightBlack - 40) && (analogRead(leftSensor) < leftBlack + 40 and analogRead(leftSensor) > leftBlack - 40)) {        
        Serial.print("Plus 1");
        codeLines += 1;   //Adds 1 to number of barcode lines read
        delay(1000);    //Waits 1 second to get off each line
      }
    }
    if (codeLines == 2) {
      Serial.print("Right");
      turnRight = true;   //Set to turn right
    }
    else if (codeLines == 3) {
      Serial.print("Left");
      turnLeft = true;    //Set to turn left
    }
  } 
}
