/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald

 modified for the ESP32 on March 2017
 by John Bennett

 see http://www.arduino.cc/en/Tutorial/Sweep for a description of the original code

 * Different servos require different pulse widths to vary servo angle, but the range is 
 * an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
 * sweep 180 degrees, so the lowest number in the published range for a particular servo
 * represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
 * of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
 * 1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
 * degrees.
 * 
 * Circuit: (using an ESP32 Thing from Sparkfun)
 * Servo motors have three wires: power, ground, and signal. The power wire is typically red,
 * the ground wire is typically black or brown, and the signal wire is typically yellow,
 * orange or white. Since the ESP32 can supply limited current at only 3.3V, and servos draw
 * considerable power, we will connect servo power to the VBat pin of the ESP32 (located
 * near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS. 
 * 
 * We could also connect servo power to a separate external
 * power source (as long as we connect all of the grounds (ESP32, servo, and external power).
 * In this example, we just connect ESP32 ground to servo ground. The servo signal pins
 * connect to any available GPIO pins on the ESP32 (in this example, we use pin 18.
 * 
 * In this example, we assume a Tower Pro MG995 large servo connected to an external power source.
 * The published min and max for this servo is 1000 and 2000, respectively, so the defaults are fine.
 * These values actually drive the servos a little past 0 and 180, so
 * if you are particular, adjust the min and max values to match your needs.
 *

 Aaron Edits
 Summary: Calibration Test for Servo. Top Servo should get 180 Degrees range of motion (note motor has 270 degree capability)

 *Pin 2 is for Bottom Servo movement
 *Pin 4 is for Top Servo movement
 */
 
#include <ESP32Servo.h>
#include <Preferences.h>

#define SLOW 50
#define MEDIUM 25
#define FAST 12
#define SUPERFAST 5
Servo myservotop;  // create servo object to control top servo
Servo myservobot;  // create servo object to control top servo
Preferences preferences; //object used to store non volatile data.
// 16 servo objects can be created on the ESP32

//function that will move servo to specified point at a slow rate (As  opposed to "Servo.write()" function which goes as fast as possible)
void servo_move (Servo *servo, uint *cur_deg, uint next_deg, int waitTime) {
  //if current degree < next degree, execute while statement
  if ((*cur_deg) < next_deg){
    //while current degrees < next degrees, keep adding +1 degree every few ms until current degree > next degree
    while((*cur_deg) < next_deg){
    delay(waitTime);
    (*cur_deg)++;
    (*servo).write(map((*cur_deg), 0, 270, 0, 180));
    }
  }
  // if current degree > next degree, execute while statement
    if ((*cur_deg) > next_deg){
    //While current degree > next degrees, keep subtracting -1 degree from current degree until current degree < next degree.
    while((*cur_deg) > next_deg){
    delay(waitTime);
    (*cur_deg)--;
    (*servo).write(map((*cur_deg), 0, 270, 0, 180));
    }
  }
}

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPinTop = 4;
int servoPinBot = 2;
uint top_degrees = 135;
uint bot_degrees = 120;

void setup() {
  Serial.begin(19200);
  preferences.begin("servo", false);
  preferences.getUInt("top_servo", top_degrees);
  preferences.getUInt("bot_servo", bot_degrees);
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservotop.setPeriodHertz(70);    // standard 70 hz servo
	myservobot.setPeriodHertz(70);    // standard 50 hz servo
	myservotop.attach(servoPinTop, 500, 2400); // attaches the servo on pin 18 to the servo object
	myservobot.attach(servoPinBot, 500, 2450); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
}

void loop() {
  //moves servo to set degrees at a slow speed
  servo_move(&myservotop, &top_degrees, 45, MEDIUM);
  // servo_move(&myservobot, &bot_degrees, 0, MEDIUM);
  // preferences.putUInt("bot_servo", bot_degrees);
  delay(5000);
  //moves servo to set degrees at a slow speed
  servo_move(&myservotop, &top_degrees, 225, MEDIUM);
  // servo_move(&myservobot, &bot_degrees, 180, MEDIUM);
  delay(5000);
		// myservotop.write(map(top_degrees, 0, 270, 0, 180));    // tell servo to go to position in variable 'top_degrees'
		// delay(5000);             // waits 15ms for the servo to reach the position
		// myservobot.write(map(bot_degrees, 0, 270, 0, 180));    // tell servo to go to position in variable 'bot_degrees'
		// delay(5000);             // waits for the servo to reach the position
		// myservotop.write(map(225, 0, 270, 0, 180));    // tell servo to go to position in variable 'pos'
		// delay(5000);             // waits 15ms for the servo to reach the position
	
}