/*
 * This sketch uses the Polytone library to play two tones at the same time.
 * The user can toggle the engine noise on or off by pushing the engine button.
 * The user can also simultaneously pulse the lasers by pushing and holding the laser button.
 * The program uses state variables to track the states of the engine and laser and also the state of the laser pulses.
 */

#include "Polytone.h" // Include the Polytone library
Polytone poly;        // poly is the object that we use to play multiple voices on one speaker.

#include "pitches.h"  // include this library for the frequencies of the musical notes.

#define LED_LASER 8
#define LED_ENGINE 13
#define VOICE_PIN_LASER 2
#define VOICE_PIN_ENGINE 3
#define VOICE_LASER 0
#define VOICE_ENGINE 1
#define BUTTON_LASER 50
#define BUTTON_ENGINE 48

#define FREQ_LASER 660
#define FREQ_ENGINE 60

enum PULSE_STATE {
  ON,
  OFF
};

PULSE_STATE pulseState = ON;

long startTime = 0;
long currTime = 0;
long elapsedTime = 0;

bool bPrevBtnEngine = false;     // keep track of the previous engine button state.
bool bPrevBtnLaser = false;      // keep track of the previous laser button state.

bool engineOn = false;
bool laserOn = false;

bool isPressed(int pin) {
  if (digitalRead(pin) == LOW) {
    // button is pressed.
    return true;
  } else {
    return false;
  }
}

void setup() {
  // serial connecton.
  Serial.begin(9600);

  // place LED pins into output mode.
  pinMode(LED_LASER, OUTPUT);
  pinMode(LED_ENGINE, OUTPUT);

  // place the buttons into pullup mode.
  pinMode(BUTTON_LASER, INPUT_PULLUP);
  pinMode(BUTTON_ENGINE, INPUT_PULLUP);

  // set up the poly voices.
  poly.begin();

  poly.setPins(VOICE_PIN_LASER, VOICE_PIN_ENGINE);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  
  // has the engine button just been pressed?
  if (isPressed(BUTTON_ENGINE) && bPrevBtnEngine == false) {
    // button was just pressed.
    //Serial.println("Btn Engine Pressed.");
    bPrevBtnEngine = true;

    // toggle the engine on or off (flip its value).
    engineOn = !engineOn;

    // was engine just turned on?
    if (engineOn) {
      poly.tone(FREQ_ENGINE);
      digitalWrite(LED_ENGINE, HIGH);
    } else {
      //poly.stopFrequency(FREQ_ENGINE);
      poly.stopAll();
      digitalWrite(LED_ENGINE, LOW);
    }
    delay(25);
  } else if (isPressed(BUTTON_ENGINE) == false && bPrevBtnEngine == true) {
    // button was just released.
    //Serial.println("Btn Engine Released.");
    bPrevBtnEngine = false;
  }

  // check to see if we need to adjust engine sound.
  if (engineOn) {
    //Serial.println("ON");
    //digitalWrite(LED_ENGINE, HIGH);
  } else  {
    //Serial.println("OFF");
    //digitalWrite(LED_ENGINE, LOW); 
  }

  // has the laser button just been pressed?
  if (isPressed(BUTTON_LASER) && bPrevBtnLaser == false) {
    // laser button was just pressed.
    bPrevBtnLaser = true;
    laserOn = true;
    pulseState = ON;
    digitalWrite(LED_LASER, HIGH);
    poly.tone(FREQ_LASER);
    delay(20);
  } else if (isPressed(BUTTON_LASER) == false && bPrevBtnLaser == true) {
    // laser button was just released.
    bPrevBtnLaser = false;
    laserOn = false;
    digitalWrite(LED_LASER, LOW);
    poly.stopFrequency(FREQ_LASER);
    //Serial.println("Laser off");
    delay(20);
  }

  // update laser LED and sound.
  if (laserOn) {
    // it's on. first update elapsed time.
    currTime = millis();
    elapsedTime = currTime - startTime;
    if (pulseState == ON && elapsedTime > 150) {
      // only stay on for a bit then pause.
      // switch to off state.
      startTime = currTime;
      pulseState = OFF;
      digitalWrite(LED_LASER, LOW);
      poly.stopFrequency(FREQ_LASER);
    } else if (pulseState == OFF && elapsedTime > 40) {
      // switch to ON state.
      startTime = currTime;
      pulseState = ON;
      digitalWrite(LED_LASER, HIGH);
      poly.tone(FREQ_LASER);
    }
  }
}
