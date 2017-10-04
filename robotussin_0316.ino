#include <rgb_lcd.h>
#include <Wire.h>
#include <Servo.h>

 /*
  * Notes on the pins:
  * SDA - analog pin 4
  * SCL - analog pin 5
  */

// lcd constants
rgb_lcd lcd;
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// variables for buzzer and button
int BUZZER_PIN = 2;
int tempo = 300;

int buttonPin = 6;
int buttonState = 0;

// variables for sensors
int sensorpin = 0; 
int sensorpin2 = 1;
int sensorpin3 = 2;                
int val = 0;
int val2 = 0;
int val3 = 0;                 
int led = 11;

int counter = 0;
int avg = 0;
int finalAvg = 0;
int avg2 = 0;
int finalAvg2 = 0;
int avg3 = 0;
int finalAvg3 = 0;

// Servos
Servo servoLeft;          
Servo servoRight;          

void setup() {  
  pinMode(led, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  // lcd commented out until it's fixed
  //lcd.begin(16, 2);
} 

void loop() { 
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH)
  {
    /*
     * put all robot code here
     */
    forward(5);
    turnLeft(5);
    playMusic("siren");
    
    delay(200);
  }
}

// LCD functions - disabled until lcd is fixed
int printMessage(char str[])
{
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0, 1);
  lcd.print(str);
  delay(50);
  return 0;
}

// Motion routines for forward, reverse, turns, and stop
int turnLeft(int seconds)
{
  Serial.println("turning left");
  attachWheels();
  left();
  
  readSensors(seconds, 3);

  detachWheels();
  return 0;
}

void left() {
  servoLeft.write(60);
  servoRight.write(60);
}

int turnRight(int seconds)
{
  Serial.println("turning right");
  attachWheels();
  right();
  
  readSensors(seconds, 1);

  detachWheels();
  return 0;
}

void right() {
  servoLeft.write(120);
  servoRight.write(120);
}

int reverse(int seconds)
{
  Serial.println("reversing");
  attachWheels();
  reverseRobot();
  
  readSensors(seconds, 2);

  detachWheels();
  return 0;
}

void reverseRobot() {
  servoLeft.write(60);
  servoRight.write(120);
}

int forward(int seconds)
{
  Serial.println("moving forward");
  attachWheels();
  forwardRobot();
  
  readSensors(seconds, 0);

  detachWheels();
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

void attachWheels() {
  servoLeft.attach(9);  
  servoRight.attach(10);  
}

void detachWheels() {
  servoLeft.detach();
  servoRight.detach();
  delay(200);
}

float read_gp2d12_range(byte pin)
{
  int tmp;
  tmp = analogRead(pin);
  if (tmp < 3)return -1;
  return (6787.0 /((float)tmp - 3.0)) - 4.0;
}

/* Inputs for direction:
 * 0 - forward
 * 1 - right
 * 2 - reverse
 * 3 - left
 */
void readSensors(int seconds, int direction) {
  int time = seconds * 1000;
  counter = 0;
  avg = 0;
  avg2 = 0;
  avg3 = 0;
  finalAvg = 0;
  finalAvg2 = 0;
  finalAvg3 = 0;

  while(time > 0)
  {
    val = analogRead(sensorpin);       // front sensor
    val2 = analogRead(sensorpin2);    // right sensor
    val3 = analogRead(sensorpin3);    // left sensor
    Serial.println(val);            // prints the value of the sensor to the serial monitor
    Serial.println(val2);
    Serial.println(val3);
    Serial.print("\t");
    Serial.println(time);
    if(counter < 9)
    {
      avg += val;
      avg2 += val2;
      avg3 += val3;
      counter++;
    }
    else
    {
      finalAvg = avg / 10;
      finalAvg2 = avg2 / 10;
      finalAvg3 = avg3 / 10;
      avg = 0;
      avg2 = 0;
      avg3 = 0;
      counter = 0;
    }
    time = time - 25;
    delay(25);                    
  
    if (finalAvg > 300 || finalAvg2 > 300 || finalAvg3 > 300) {
     break;
    }
    else if (finalAvg <= 300 && finalAvg2 <= 300 && finalAvg3 <= 300) {
      if (direction == 0)
      {
        forwardRobot();
      }
      else if (direction == 1)
      {
        right();
      }
      else if (direction == 2)
      {
        reverseRobot();
      }
      else if (direction == 3)
      {
        left();
      }
      else
      {
        detachWheels();
      }
    }
  }
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

void playMusic(char str[])
{
  if(str == "siren")
  {
    char notes[] = "ecec ";
    int beats[] = { 1, 1, 1, 1, 1};
    int length = 5; 
    for(int i = 0; i < length; i++) {
          if(notes[i] == ' ') {
              delay(beats[i] * tempo);
          } else {
              playNote(notes[i], beats[i] * tempo);
          }
          delay(tempo / 2);    /* delay between notes */
      }
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
