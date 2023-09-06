/* 

 Aaron Nguyen
 Summary: Calibration Test for Servo. Top Servo should get 180 Degrees range of motion (note motor has 270 degree capability)

 *Pin 2 is for Bottom Servo movement
 *Pin 4 is for Top Servo movement
 */

#include <ESP32Servo.h>
#include "servo_move.h"

#define GetName (Variable) (#Variable);
#define SLOW 50
#define MEDIUM 25
#define FAST 12
#define SUPERFAST 5
Servo myservotop;         // create servo object to control top servo
Servo myservobot;         // create servo object to control top servo
// 16 servo objects can be created on the ESP32

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPinTop = 4;
int servoPinBot = 2;
uint top_degrees = 0;
uint bot_degrees = 0;
void setup() {
  Serial.begin(115200);
  delay(1000);
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservotop.setPeriodHertz(70);              // standard 70 hz servo
  myservobot.setPeriodHertz(70);              // standard 50 hz servo
  myservotop.attach(servoPinTop, 500, 2400);  // attaches the servo on pin 18 to the servo object
  myservobot.attach(servoPinBot, 500, 2400);  // attaches the servo on pin 18 to the servo object
                                              // using default min/max of 1000us and 2000us
                                              // different servos may require different min/max settings
                                              // for an accurate 0 to 180 sweep
  delay(3000);
}

void loop() {
  //moves servo to set degrees at a slow speed
  servo_move(&myservobot, &bot_degrees, 0, FAST);
  servo_move(&myservotop, &top_degrees, 0, FAST);
  delay(5000);
  //moves servo to set degrees at a slow speed
  servo_move(&myservobot, &bot_degrees, 180, FAST);
  servo_move(&myservotop, &top_degrees, 180, FAST);
  delay(5000);
  // servo_move(&myservotop, &top_degrees, 225, MEDIUM);
  // delay(2500);
  // myservotop.write(map(top_degrees, 0, 270, 0, 180));    // tell servo to go to position in variable 'top_degrees'
  // delay(5000);             // waits 15ms for the servo to reach the position
  // myservobot.write(map(bot_degrees, 0, 270, 0, 180));    // tell servo to go to position in variable 'bot_degrees'
  // delay(5000);             // waits for the servo to reach the position
  // myservotop.write(map(225, 0, 270, 0, 180));    // tell servo to go to position in variable 'pos'
  // delay(5000);             // waits 15ms for the servo to reach the position
}