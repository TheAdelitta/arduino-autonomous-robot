// -------------------------------------------------------------------------------------- //
//Code obtained from
//https://create.arduino.cc/projecthub/tarantula3/using-an-ldr-sensor-with-arduino-807b1c
//LED turns off when the surrounding light is bright. 
//To simulate darkness, hold down the photocell with your hands to turn on the LED. 
// Using Arduino IDE 1.8.13
// Using Photocell
// Tested Oct. 13th, 2021
// -------------------------------------------------------------------------------------- //

const int ledPin = 13;
const int photocellPin = A0;

void setup() 
{
Serial.begin(9600);
pinMode(ledPin, OUTPUT);
pinMode(photocellPin, INPUT);
}

void loop() 
{
int photocellStatus = analogRead(photocellPin);
if (photocellStatus <= 100) 
{
digitalWrite(ledPin, HIGH);
Serial.print("Its DARK, Turn on the LED : ");
Serial.println(photocellStatus);

} else 
{
digitalWrite(ledPin, LOW);
Serial.print("Its BRIGHT, Turn off the LED : ");
Serial.println(photocellStatus);
}
}
