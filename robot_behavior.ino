// ---------------------------------------------------------------- // 
// Arduino Origami Bot 
// Using Arduino IDE 1.8.19
// ---------------------------------------------------------------- // 

#include <LiquidCrystal_I2C.h> // liquid crystal library

LiquidCrystal_I2C lcd(0x27, 16, 2); // specifices how to refrence the LCD and it's characteristics


// define varibales

//Strings are padded to 16 chars to fully overwrite the LCD line
char array1[] = "Robot State:    ";
char array2[] = "STOPPED         ";
char array3[] = "BACKWARD        ";
char array4[] = "FORWARD         ";
char array5[] = "OUT OF RANGE    ";

//Ultrasonic pins
const echoPin = 8; // attach pin D8 to pin Echo of HC-SR04
const trigPin = 7; // attach pin D7 to pin Trig of HC-SR04

//L298N motor pins
const in1 = 4; // variable names for the L298N
const in2 = 5; // variable names for the L298N
const in3 = 9; // variable names for the L298N
const in4 = 10; // variable names for the L298N

//L298N enable pins for PWM speed control
const LCDprev = 99; // helps to update the LCD state tracks previous
const LCDcurr = 100; // helps to update the LCD state tracks current

//LED
const ledpin = 13; //specify the LED pin to pin 13

//Distance variables
long duration = 0; // duration of sound wave travel
int distance = 0; // distance measurement

//LCD state tracking
int LCDprev=99;
int LCDcurr=100;

//Speed settings 0..255
int speedLeft=170;
int speedRight=170;


void setup() {
  lcd.init(); // initializes LCD
  lcd.backlight(); // turn on backlight

  pinMode (trigPin, OUTPUT) ; // sonar digital pin mode for trig
  pinMode (echoPin, INPUT); // sonar digital pin mode for echo
  
  pinMode (in1, OUTPUT); // all L298N digital pins are outputs
  pinMode (in2, OUTPUT); // all L298N digital pins are outputs
  pinMode (in3, OUTPUT); // all L298N digital pins are outputs
  pinMode (in4, OUTPUT); // all L298N digital pins are outputs
  pinMode (ledpin, OUTPUT) ; // configure LCD pin as an output

  Serial.begin (9600) ; // use default of 9600 baud rate for serial communication

  lcd.setCursor(0, 0); // set cursor at top left

  lcd.print (array1); //prints the first line "Robot State:

 //Seed random for turn choice
  randomSeed(analogRead(A0));

  //Initial stop
  stopMotors();
  writeLCD(array2);
  
}


void loop() {
  
/*  
  NOTE: it is important only to update the LCD screen when a change of state has occured
  Otherwise, the program become inefficient. We only want to write to the LCD when 
  there is a change in what the robot is doing. The LCDprey and LCDcurr variables 
   help us to do this
*/
  LCDprev = LCDcurr;

  // Clears the trigPin condition
  digitalWrite (trigPin, LOW);//starts trig pin low
  delayMicroseconds (2); //waits for hardware to respond
  digitalWrite (trigPin, HIGH); //makes trig pin high
  delayMicroseconds (10) ; //continues high trig pin state for 10 microseconds
  digitalWrite (trigPin, LOW); //after 10 microseconds trig pin is brought low

  
//Read echo and compute distance in cm
  duration=pulseIn(echoPin,HIGH,30000);
  if(duration==0){
    distance=400;
  }else{
    distance=duration*0.034/2;
  }

  // Displays the distance on the serial Monitor
  Serial.print ("Distance: ");
  Serial.print (distance);
  Serial.println(" cm");
  
  //Simple hysteresis band to reduce flip-flop near thresholds
  const int nearThresh=15;
  const int stopThresh=30;
  const int goThresh=45;

  if(distance<=nearThresh){
    LCDcurr=1;
    digitalWrite(ledpin,HIGH);
    //Back up briefly
    driveBackward(speedLeft,speedRight);
    writeLCD(array3);
    delay(400);
    //Random turn
    if(random(0,2)==0){
      LCDcurr=6;
      turnLeft(180);
      writeLCD(array6);
    }else{
      LCDcurr=7;
      turnRight(180);
      writeLCD(array7);
    }
  }else if(distance<=stopThresh){
    LCDcurr=2;
    digitalWrite(ledpin,LOW);
    stopMotors();
    writeLCD(array2);
  }else if(distance<=goThresh){
    LCDcurr=3;
    digitalWrite(ledpin,HIGH);
    driveForward(speedLeft,speedRight);
    writeLCD(array4);
  }else{
    LCDcurr=4;
    digitalWrite(ledpin,LOW);
    driveForward(speedLeft,speedRight);
    writeLCD(array5);
  }

  delay(20);
}

//Helper to update second LCD line only if state changed
void writeLCD(const char* msg){
  if(LCDprev!=LCDcurr){
    lcd.setCursor(0,1);
    lcd.print(msg);
  }
}

//Motor primitives
void driveForward(int ls,int rs){
  analogWrite(enA,constrain(ls,0,255));
  analogWrite(enB,constrain(rs,0,255));

  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);

  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void driveBackward(int ls,int rs){
  analogWrite(enA,constrain(ls,0,255));
  analogWrite(enB,constrain(rs,0,255));

  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);

  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void stopMotors(){
  analogWrite(enA,0);
  analogWrite(enB,0);

  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void turnLeft(int ms){
  //left motor slow or reverse, right motor forward
  analogWrite(enA,120);
  analogWrite(enB,190);

  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);

  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);

  delay(ms);
}

void turnRight(int ms){
  //right motor slow or reverse, left motor forward
  analogWrite(enA,190);
  analogWrite(enB,120);

  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);

  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);

  delay(ms);
}
