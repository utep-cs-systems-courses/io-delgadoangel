#include <msp430.h>
#include "state_machines.h"
#include "led.h"

// blink state machine
static int blinkLimit = 0;   //  state var representing reciprocal of duty cycle 
void blinkUpdate() // called every 1/250s to blink with duty cycle 1/blinkLimit
{
  static int blinkCount = 0; // state var representing blink state
  blinkCount ++;
  if (blinkCount >= blinkLimit) {
    blinkCount = 0;
    green_on = 1;
  } else
    green_on = 0;
  
  led_update();
}

void void oncePerTimePeriod() // repeatedly start bright and gradually lower duty cycle, one step/sec
{
  blinkLimit ++;  // reduce duty cycle
  if (blinkLimit >= 8)  // but don't let duty cycle go below 1/7.
    blinkLimit = 0;
}



void timeUpdate()  // called every 1/250 sec to call oncePerSecond once per second
{
  static int timeCount = 0; // state variable representing repeating time in 1/20 steps
  timeCount ++;
  if (timeCount >= 125/10) { // once each second
    timeCount = 0;
    oncePerTimePeriod();
  } }

void timeAdvStateMachines() // called every 1/250 sec
{
  blinkUpdate();
  timeUpdate();
}
