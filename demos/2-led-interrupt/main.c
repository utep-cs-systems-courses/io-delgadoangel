//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT &= ~LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}

int secondCount = 0;

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  secondCount ++;
  
  for (int i = 0; i <= secondCount/2; i += 1) {
    P1OUT |= LED_GREEN;
    P1OUT &= ~LED_GREEN;
  }

  for (int i = 0; i <= 5*secondCount; i += 1) {
    P1OUT |= LED_RED;
    P1OUT &= ~LED_RED;
  }

  if (secondCount >= 500) {
    secondCount = 1;
  }
  
} 

