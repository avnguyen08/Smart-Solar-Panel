/*
SUMMARY: Auto adjusting solar panel. Uses two servos (0-180 degree), 
Board: Esp32 WROOM(ESP32 Dev Module for Arduino).
Uses pins d19, d21, d32, d35 as analog read to track the LDR sensors values
Uses D2 and D4 as PWM pins to configure servo motors for solar panels.
*/
#include <ESP32Servo.h>
#include "Servo_Panel_Calibration\servo_move.h"
//DEBUG VARIABLES
#define SUPER_SLOW 400
#define SLOW 100
#define MEDIUM 50
#define FAST 12
#define SUPERFAST 5
char string[50];
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

//value of photoresistors
int topleft;
int topright;
int downleft;
int downright;
int left, right, top, down;
int change = 1;
uint waittime = 500;
int light_sens = 50;

//Pins reading input voltage from photoresistors
int PINA = 25;
int PINB = 33;
int PINC = 32;
int PIND = 35;
//Pin to control Power switch
int PIN_Power = 14;

Servo myservoBot;  // create servo object to control bottom servo
Servo myservoTop;  // create servo object to control top servo
// 16 servo objects can be created on the ESP32
int minbotval = 0;
int maxbotval = 180;
int mintopval = 45;
int maxtopval = 135;
RTC_DATA_ATTR uint posTop = 90;       // variable to store the top servo position, RTC saves after sleep
RTC_DATA_ATTR uint posBot = 90;       // variable to store the bottom servo position, RTC saves after sleep
uint tempTop = 90;  //variable to store past top position
uint tempBot = 90;  //variable to store past bot position
int startTime = 0;
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPinTop = 4;
int servoPinBot = 2;



  //Make solar panel servos and esp32 go to sleep for inputted amount of time
void solar_sleep(int slp_time) {
  digitalWrite(PIN_Power, LOW);
  Serial.print("Going into sleep mode for ");
  Serial.print(slp_time);
  Serial.println(" seconds");
  //Turn off ESP32
  esp_sleep_enable_timer_wakeup(slp_time*uS_TO_S_FACTOR); //enables wake up by timer
  esp_deep_sleep_start(); //starts deep sleep mode. RTC timer + RTC memorys burns only 10uA
}

void setup() {
  Serial.begin(115200);
  pinMode(PINA, INPUT);
  pinMode(PINB, INPUT);
  pinMode(PINC, INPUT);
  pinMode(PIND, INPUT);
  pinMode(PIN_Power, OUTPUT);
  digitalWrite(PIN_Power, HIGH);
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  //Using 70hz PWM signal. May differ based on servo
  myservoBot.setPeriodHertz(70);  // standard 50 hz servo
  myservoTop.setPeriodHertz(70);  // standard 50 hz servo
  // using min/max of 500us and 2400us. May differ based on servo
  myservoBot.attach(servoPinBot, 500, 2400);  // attaches the servo on pin 2 to the servo object
  myservoTop.attach(servoPinTop, 500, 2400);  // attaches the servo on pin 4 to the servo object
  // servo_move(&myservoBot, &posBot, 90, FAST);
  // servo_move(&myservoTop, &posTop, 90, FAST);
  Serial.println("Servo Setup Finished!");
  delay(1500);
}

void loop() {
  topleft = analogRead(PINA);
  topright = analogRead(PINB);
  downleft = analogRead(PINC);
  downright = analogRead(PIND);
  top = (topleft + topright) / 2;
  down = (downleft + downright) / 2;
  left = (topleft + downleft) / 2;
  right = (topright + downright) / 2;

  //edge case: sensors in darkness, no point in moving servos because it may be a cloud. ESP32 max value is 4096 for analog read
  if (top > 4050 && down > 4050) {
    // servo_move(&myservoTop, &posTop, 90, FAST);
    // servo_move(&myservoBot, &posBot, 90, FAST);
  } else {
    int currentTime = millis();  // current time
    //If last position of servos has been the same for a x amount of time, activate sleep mode
    if (currentTime - startTime >= 3000) {
      if (tempTop == posTop && tempBot == posBot) {
        tempTop = posTop;
        tempBot = posBot;
        solar_sleep(3); //sleep for x amount of seconds
      } else {
        startTime = currentTime;
        tempTop = posTop;
        tempBot = posBot;
      }
    } else
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
  }
  delay(MEDIUM);
  //debug string meant to display servos' degrees and LDR sensor readings
  sprintf(string, "Bottom Servo: %u \t\t Top Servo: %u \t\t LDR: %d \t\t%d\t\t%d\t\t%d", posBot, posTop, topleft, topright, downleft, downright);
  Serial.println(string);
  myservoBot.write(posBot);  // tell servo to go to position in variable 'pos'
  myservoTop.write(posTop);  // tell servo to go to position in variable 'pos'
}
