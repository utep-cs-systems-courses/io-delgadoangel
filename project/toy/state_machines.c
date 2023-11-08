#include <msp430.h>
#include "state_machines.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"

// blink state machine
static int jumpStateMachine = 0;
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

void timeUpdate()  // called every 1/250 sec
{
  static int timeCount = 0; // state variable representing repeating time in 1/20 steps
  timeCount ++;
  if (timeCount >= 250/20) { // once each 1/20 of a second
    timeCount = 0;
    oncePerTimePeriod();
  }
}

void oncePerSecond() // called every second
{
  if (red_on) {
    red_on = 0;
    green_on = 1;
  } else {
    red_on = 1;
    green_on = 0;
  }
  
  led_update();
}

void secondUpdate()  // called every 1/250 sec to call oncePerSecond once per second                                             
{
  static int secondCount = 0; // state variable representing repeating time 0…1s                                                 
  secondCount ++;
  if (secondCount >= 250) { // once each second                                                                               
    secondCount = 0;
    oncePerSecond();
  }
}
void buzzerUpdate()
{
  static int note = 1;
  if (note >= 4)
    note = 0;
    
  buzzer_set_period(song_a[note]);
  note ++;
}

void buzzerTimeUpdate() // called every 1/250 sec to call oncePerSecond once per second
{
  static int buzzTimeCount = 0;
  buzzTimeCount ++;
  if (buzzTimeCount >= 300) {
    buzzTimeCount = 0;
    buzzerUpdate();
  }
}
    

void switchJump()
{
  if (switch_state_changed) {
    jumpStateMachine = switch_state_down; // Setting a different state machine
  }
  switch_state_changed = 0;
}

void timeAdvStateMachines() // called every 1/250 sec
{
  if (jumpStateMachine) {
    secondUpdate();
    buzzer_set_period(0);
  } else {
    blinkUpdate();
    timeUpdate();
    buzzerTimeUpdate();
  }
}
