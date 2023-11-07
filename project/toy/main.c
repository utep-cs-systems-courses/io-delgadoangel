#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"

void main(void) 
{  
  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */

  //buzzer_init();
  //buzzer_set_period(1000);	/* start buzzing!!! 2MHz/1000 = 2kHz*/

  //switch_init();
  led_init();

  or_sr(0x18);  // CPU off, GIE on
} 
