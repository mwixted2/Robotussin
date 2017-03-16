#include <Servo.h>
#include <LiquidCrystal.h>

/*
 * The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */
 LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int BUZZER_PIN = 6;
int tempo = 300;

int buttonPin = 2;
int buttonState = 0;

int sensorpin = 0;                 
int val = 0;                 
int led = 11;

int counter = 0;
int avg = 0;
int finalAvg = 0;

Servo servoLeft;          
Servo servoRight;          

void setup() { 
  servoLeft.attach(9);  
  servoRight.attach(10);  
  pinMode(led, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
} 

bool running = true;
void loop() { 
  if(running)
  {
    forward(15);
    running = false;
  }
}

/*int main()
{
  //setup();
  //forward(5);
  return 0;
}
*/
void robotMovement()
{
  //add functions for robot movement
  
  /*
   * functions for students to use:
   * 
   * printMessage( char inputString[] )
   * ledOn( int seconds )
   * turnLeft( int seconds )
   * turnRights( int seconds )
   * reverse( int secodns )
   * forward( int seconds )
   * playMusic( char notes[], int beats[], int length )
   * buttonMusic( char notes[], int beats[], int length )
   * buttonLed()
   */

   forward(5);
}

//LCD functions
int printMessage(char str[])
{
  lcd.setCursor(0, 1);
  lcd.print(str);
  delay(50);
  return 0;
}

//LED functions
int ledOn(int seconds)
{
  analogWrite(led, 200);
  digitalWrite(6, HIGH);
  delay(seconds * 1000);
  ledOff();
  return 0;
}

void ledOff()
{
  analogWrite(led, 0);
  digitalWrite(6, LOW);
  delay(50);
}

// Motion routines for forward, reverse, turns, and stop
float read_gp2d12_range(byte pin)
{
  int tmp;
  tmp = analogRead(pin);
  if (tmp < 3)return -1;
  return (6787.0 /((float)tmp - 3.0)) - 4.0;
}

int turnLeft(int seconds)
{
  int time = seconds * 1000;
  counter = 0;
  avg = 0;
  finalAvg = 0;
  val = analogRead(sensorpin);       // reads the value of the sharp sensor
  Serial.println(val);            // prints the value of the sensor to the serial monitor

  while(time > 0)
  {
    if(counter < 9)
    {
      avg += val;
      counter++;
    }
    else
    {
      finalAvg = avg / 10;
      avg = 0;
      counter = 0;
    }
    time = time - 25;
    delay(25);                    
  
    if (finalAvg > 300) {
     stopRobot();
     return 0;
    }
    else if (finalAvg <= 300) {
      left();
    }
  }

  stopRobot();
  delay(50);
  return 0;
}

void left() {
  servoLeft.write(60);
  servoRight.write(60);
}

int turnRight(int seconds)
{
  int time = seconds * 1000;
  counter = 0;
  avg = 0;
  finalAvg = 0;
  val = analogRead(sensorpin);       // reads the value of the sharp sensor
  Serial.println(val);            // prints the value of the sensor to the serial monitor

  while(time > 0)
  {
    if(counter < 9)
    {
      avg += val;
      counter++;
    }
    else
    {
      finalAvg = avg / 10;
      avg = 0;
      counter = 0;
    }
    time = time - 25;
    delay(25);                    
  
    if (finalAvg > 300) {
     stopRobot();
     return 0;
    }
    else if (finalAvg <= 300) {
      right();
    }
  }

  stopRobot();
  delay(50);
  return 0;
}

void right() {
  servoLeft.write(120);
  servoRight.write(120);
}

int reverse(int seconds)
{
  int time = seconds * 1000;
  counter = 0;
  avg = 0;
  finalAvg = 0;
  val = analogRead(sensorpin);       // reads the value of the sharp sensor
  Serial.println(val);            // prints the value of the sensor to the serial monitor

  while(time > 0)
  {
    if(counter < 9)
    {
      avg += val;
      counter++;
    }
    else
    {
      finalAvg = avg / 10;
      avg = 0;
      counter = 0;
    }
    time = time - 25;
    delay(25);                    
  
    if (finalAvg > 300) {
     stopRobot();
     return 0;
    }
    else if (finalAvg <= 300) {
      reverseRobot();
    }
  }

  stopRobot();
  delay(50);
  return 0;
}

void reverseRobot() {
  servoLeft.write(60);
  servoRight.write(120);
}

int forward(int seconds)
{
  int time = seconds * 1000;
  counter = 0;
  avg = 0;
  finalAvg = 0;
  val = analogRead(sensorpin);       // reads the value of the sharp sensor
  //Serial.println("ir sensor: " + val);            // prints the value of the sensor to the serial monitor

  while(time > 0)
  {
    Serial.println(time);
    if(counter < 9)
    {
      avg += val;
      counter++;
    }
    else
    {
      finalAvg = avg / 10;
      avg = 0;
      counter = 0;
    }
    time = time - 25;
    delay(25);                    
  
    if (finalAvg > 300) {
     stopRobot();
     return 0;
    }
    else if (finalAvg <= 300) {
      forwardRobot();
    }
  }

  stopRobot();
  delay(50);
  return 0;
}

void forwardRobot() {
  servoLeft.write(120);
  servoRight.write(60);
}

void stopRobot() {
  servoLeft.write(90);
  servoRight.write(90);
}

//Music functions
void playMusic(char notes[], int beats[], int length)
{
  for(int i = 0; i < length; i++) {
        if(notes[i] == ' ') {
            delay(beats[i] * tempo);
        } else {
            playNote(notes[i], beats[i] * tempo);
        }
        delay(tempo / 2);    /* delay between notes */
    }
}

/* play tone */
void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(tone);
        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}

//Button code
int buttonMusic(char notes[], int beats[], int length)
{
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH)
  {
    playMusic(notes, beats, length);
    return 0;
  }
  else
  {
    digitalWrite(6, LOW);
    return 0;
  }
}

int buttonLed()
{
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH)
  {
    ledOn(5);
    return 0;
  }
  else if(buttonState == LOW)
  {
    ledOff();
    return 0;
  }
}

