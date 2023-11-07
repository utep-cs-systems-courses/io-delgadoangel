#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"

void main(void) 
{
  led_init();
  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */

  switch_init();
  
  //buzzer_init();
  //buzzer_set_period(1000);	/* start buzzing!!! 2MHz/1000 = 2kHz*/

  or_sr(0x18);  // CPU off, GIE on
} 
