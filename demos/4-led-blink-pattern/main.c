//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);			/* CPU off, GIE on */
}

// global state vars that control blinking
int blinkGreenLimit = 7;  // duty cycle = 1/blinkLimit
int blinkRedLimit = 0;  // duty cycle = 1/blinkLimit  
int blinkGreenCount = 0;  // cycles 0...blinkLimit-1
int blinkRedCount = 0; // cycles 0...blinkLimit-1
int secondCount = 0; // state var representing repeating time 0…1s

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  // handle blinking 
  blinkGreenCount ++;
  if (blinkGreenCount >= blinkGreenLimit) { // on for 1 interrupt period
    blinkGreenCount = 0;
    P1OUT |= LED_GREEN;
  } else {                       // off for blinkLimit - 1 interrupt periods
    P1OUT &= ~LED_GREEN;
  }

  blinkRedCount ++;
  if (blinkRedCount >= blinkRedLimit) {
    blinkRedCount = 0;
    P1OUT |= LED_RED;
  } else {
    P1OUT &= ~LED_RED;
  }

  

  // measure a second
  secondCount ++;
  if (secondCount >= 250/4) {  // once each second
    secondCount = 0;
    blinkGreenLimit --;	     // increase duty cycle
    blinkRedLimit ++;        // decrease duty cycle
    if (blinkGreenLimit <= 0)     // but don't let duty cycle go below 1/7.
      blinkGreenLimit = 7;
    if (blinkRedLimit >= 8)
      blinkRedLimit = 0;
  }
} 

