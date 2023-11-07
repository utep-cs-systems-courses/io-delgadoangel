#include <msp430.h>
#include "state_machines.h"
#include "led.h"

// blink state machine
static int blinkCount = 0; // state var representing blink state
static int blinkLimit = 0;   //  state var representing reciprocal of duty cycle
static int reduce = 0;
void blinkUpdate() // called every 1/250s to blink with duty cycle 1/blinkLimit
{
  blinkCount ++;
  if (blinkCount >= blinkLimit) {
    blinkCount = 0;
    green_on = 1;
    red_on = 0;
  } else {
    green_on = 0;
    red_on = 1;
  }
  
  led_update();
}

void oncePerTimePeriod() // repeatedly start bright and gradually lower duty cycle, one step/sec
{
  if (reduce) {
    blinkLimit --;  // increase duty                                                                                                             
    if (blinkLimit <= 0)
      reduce = 0;
  } else {
    blinkLimit ++;  // reduce duty cycle

    if (blinkLimit >= 10)  // but don't let duty cycle go below 1/10.
      reduce = 1;
  }
}



void timeUpdate()  // called every 1/250 sec to call oncePerSecond once per second
{
  static int timeCount = 0; // state variable representing repeating time in 1/20 steps
  timeCount ++;
  if (timeCount >= 125/10) { // once each 1/20 of a second
    timeCount = 0;
    oncePerTimePeriod();
  }
}

void timeAdvStateMachines() // called every 1/250 sec
{
  blinkUpdate();
  timeUpdate();
}
