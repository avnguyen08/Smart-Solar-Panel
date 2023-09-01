/*
SUMMARY: Auto adjusting solar panel. Uses esp32 wroom. 
Uses pins d19, d21, d32, d35 as analog read. 
Uses D2 and D4 as output to configure solar panels.
Board on Arduino IDE is ESP32 Dev Module
*/


#include <ESP32Servo.h>
#include "Servo_Panel_Calibration\servo_move.h"
//DEBUG VARIABLES
#define SLOW 50
#define MEDIUM 25
#define FAST 12
#define SUPERFAST 5
char string[50];


//value of photoresistors
int topleft;
int topright;
int downleft;
int downright;
int left, right, top, down;
int change = 1;
uint waittime = 30;
int light_sens = 50;

//Pins reading input voltage from photoresistors
int PINA = 25;
int PINB = 33;
int PINC = 32;
int PIND = 35;

Servo myservoBot;  // create servo object to control bottom servo
Servo myservoTop;  // create servo object to control top servo
// 16 servo objects can be created on the ESP32
int minbotval = 0;
int maxbotval = 180;
int mintopval = 0;
int maxtopval = 180;
uint posTop = 0;  // variable to store the servo position
uint posBot = 0;  // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPinTop = 4;
int servoPinBot = 2;
void setup() {
  Serial.begin(115200);
  pinMode(PINA, INPUT);
  pinMode(PINB, INPUT);
  pinMode(PINC, INPUT);
  pinMode(PIND, INPUT);
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservoBot.setPeriodHertz(70);              // standard 50 hz servo
  myservoTop.setPeriodHertz(70);              // standard 50 hz servo
  myservoBot.attach(servoPinBot, 500, 2400);  // attaches the servo on pin 4 to the servo object
  myservoTop.attach(servoPinTop, 500, 2400);  // attaches the servo on pin 2 to the servo object
  Serial.println("Hello World");
  servo_move(&myservoBot, &posBot, 90, FAST);
  servo_move(&myservoTop, &posTop, 90, FAST);
  delay(2000);
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
}

void loop() {
  topleft = analogRead(PINA);
  topright = analogRead(PINB);
  downleft = analogRead(PINC);
  downright = analogRead(PIND);

  top = (topleft + topright)/2;
  down = (downleft + downright)/2;
  left = (topleft + downleft)/2;
  right = (topright + downright)/2;
  //light shines on right side more than left.  Turn counterclockwise
  if (((left - right) > light_sens) && (posBot < maxbotval)) {
    posBot = posBot + change;
  }

  //light shines on left side more than right. Turn clockwise
  if (((right - left) > light_sens) && (posBot > minbotval)) {
    posBot = posBot - change;
  }

  //light shines on down side more than top side. Turn clockwise
  if (((top - down) > light_sens) && (posTop < maxtopval)) {
    posTop = posTop + change;
  }

  //light shines on top side more than down side. Turn counterclockwise
  if (((down - top) > light_sens) && (posTop > mintopval)) {
    posTop = posTop - change;
  }


    delay(waittime);
  //debug serial print
  sprintf(string, "Bottom Servo: %u \t\t Top Servo: %u \t\t LDR: %d \t\t%d\t\t%d\t\t%d", posBot, posTop, topleft, topright, downleft, downright);
  Serial.println(string);
  myservoBot.write(posBot);  // tell servo to go to position in variable 'pos'
  myservoTop.write(posTop);  // tell servo to go to position in variable 'pos'
}
