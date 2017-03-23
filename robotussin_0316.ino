#include <rgb_lcd.h>
#include <Wire.h>
#include <Servo.h>

 /*
  * SDA - analog pin 4
  * SCL - analog pin 5
  */
 rgb_lcd lcd;
 const int colorR = 255;
 const int colorG = 0;
 const int colorB = 0;

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

bool running = true;

/* testing buzzer */
char notes[] = "ceg ";
int beats[] = { 1, 1, 1, 1};
int length = 4;

Servo servoLeft;          
Servo servoRight;          

void setup() { 
  //servoLeft.attach(9);  
  //servoRight.attach(10);  
  pinMode(led, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
} 

void loop() { 
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH)
  {
    /*
     * put all robot code here
     */
    
    forward(3);
    reverse(3);
    char str[ ] = "Hello WECE!";
    printMessage(str);
    playMusic();
    delay(200);
  }
}

//LCD functions
int printMessage(char str[])
{
  lcd.setRGB(colorR, colorG, colorB);
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
  servoLeft.attach(9);  // Set left servo to digital pin 10
  servoRight.attach(10);  // Set right servo to digital pin 9
  left();
  
  int time = seconds * 1000;
  counter = 0;
  avg = 0;
  finalAvg = 0;

  while(time > 0)
  {
    val = analogRead(sensorpin);       // reads the value of the sharp sensor
    Serial.println(val);            // prints the value of the sensor to the serial monitor
    Serial.print("\t");
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
     break;
    }
    else if (finalAvg <= 300) {
      left();
    }
  }

  servoLeft.detach();
  servoRight.detach();
  delay(200);
  return 0;
}

void left() {
  servoLeft.write(60);
  servoRight.write(60);
}

int turnRight(int seconds)
{
  servoLeft.attach(9);  // Set left servo to digital pin 10
  servoRight.attach(10);  // Set right servo to digital pin 9
  right();
  
  int time = seconds * 1000;
  counter = 0;
  avg = 0;
  finalAvg = 0;

  while(time > 0)
  {
    val = analogRead(sensorpin);       // reads the value of the sharp sensor
    Serial.println(val);            // prints the value of the sensor to the serial monitor
    Serial.print("\t");
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
     break;
    }
    else if (finalAvg <= 300) {
      right();
    }
  }

  servoLeft.detach();
  servoRight.detach();
  delay(200);
  return 0;
}

void right() {
  servoLeft.write(120);
  servoRight.write(120);
}

int reverse(int seconds)
{
  servoLeft.attach(9);  // Set left servo to digital pin 10
  servoRight.attach(10);  // Set right servo to digital pin 9
  reverseRobot();
  
  int time = seconds * 1000;
  counter = 0;
  avg = 0;
  finalAvg = 0;

  while(time > 0)
  {
    val = analogRead(sensorpin);       // reads the value of the sharp sensor
    Serial.println(val);            // prints the value of the sensor to the serial monitor
    Serial.print("\t");
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
     break;
    }
    else if (finalAvg <= 300) {
      reverseRobot();
    }
  }

  servoLeft.detach();
  servoRight.detach();
  delay(200);
  return 0;
}

void reverseRobot() {
  servoLeft.write(60);
  servoRight.write(120);
}

int forward(int seconds)
{
  servoLeft.attach(9);  // Set left servo to digital pin 10
  servoRight.attach(10);  // Set right servo to digital pin 9
  forwardRobot();
  
  int time = seconds * 1000;
  counter = 0;
  avg = 0;
  finalAvg = 0;

  while(time > 0)
  {
    val = analogRead(sensorpin);       // reads the value of the sharp sensor
    Serial.println(val);            // prints the value of the sensor to the serial monitor
    Serial.print("\t");
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
     break;
    }
    else if (finalAvg <= 300) {
      forwardRobot();
    }
  }

  servoLeft.detach();
  servoRight.detach();
  delay(200);
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
void playMusic()
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
/*
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
*/
