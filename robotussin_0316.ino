#include <rgb_lcd.h>
#include <Wire.h>
#include <Servo.h>

 /*
  * Notes on the pins:
  * SDA - analog pin 4
  * SCL - analog pin 5
  */

// LCD constants
rgb_lcd lcd;
int colorR = 255;
int colorG = 0;
int colorB = 0;

// Variables for buzzer and button
int BUZZER_PIN = 2;
int tempo = 300;

int buttonPin = 6;
int buttonState = 0;

// Variables for sensors
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
  // A lot of setting up so we get the right connections to the pins 
  pinMode(led, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
} 

void loop() { 
  // The buttonState variable and the following if statement makes it so that the robot
  // only moves once the button is pressed
  
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH)
  {
    /*
     * put all robot code here
     */
     /*
    forward(3);
    printMessage("hello WECE!");
    turnLeft(3);
    char notes[] = "cg ";
    int beats[] = {1, 1, 1};
    playMusic(notes, beats);
    */
    //playMusic("siren");

    forward(5);
    setLcdColor(47, 34, 229);
    printMessage("Go Gators!");
    delay(3000);
    setLcdColor(255, 119, 0);
    printMessage("Go Gators!");
    delay(3000); 
    turnLeft(5);
    char notes[] = "gC ";
    int beats[] = {1, 1, 1};
    playMusic(notes, beats);
    
    
    delay(200);
  }
}

/***************************************************************************
 * ALL THE FUNCTIONS
 **************************************************************************/

/******************************************************
 * LCD Functions
 *****************************************************/

/*
 * setLcdColor
 * What it does: Changes the background color of the LCD to the inputted RGB value 
 * Inputs: R value, G value, B value
 */ 
int setLcdColor(int r, int g, int b)
{
  colorR = r;
  colorG = g;
  colorB = b;
  return 0;
}

/*
 * printMessage
 * What it does: Prints the inputted message to the LCD 
 * Inputs: the message
 */
int printMessage(char str[])
{
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0, 1);
  lcd.print(str);
  delay(50);
  return 0;
}

/******************************************************
 * Motion Functions
 *****************************************************/
 
/*
 * turnLeft
 * What it does: the robot turns left for the inputted time 
 * Inputs: time (in seconds)
 */
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
  
    if (finalAvg > 400 || finalAvg2 > 400 || finalAvg3 > 400) {
      Serial.print("final avgs: ");
      Serial.print(finalAvg);
      Serial.print(" ");
      Serial.print(finalAvg2);
      Serial.print(" ");
      Serial.println(finalAvg3);
     break;
    }
    else if (finalAvg <= 400 && finalAvg2 <= 400 && finalAvg3 <= 400) {
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
void playMusic(char notes[], int beats[])
{
  if(sizeof(notes) != sizeof(beats))
  {
    Serial.println("notes and beats don't match!");
    return;
  }

  int length = sizeof(notes);
  
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
    int tones[] = { 523, 587, 659, 698, 784, 880, 988, 1047 };

    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}
