#include <or1k-support.h>
#include <spr-defs.h>
#include<stdio.h>

char* gpio_base = (char*) 0x91000000;

unsigned char dat = 0x05;

/* Tick rate storage */
extern unsigned long or1k_timer_period;

extern volatile unsigned long or1k_timer_ticks;

void or1k_timer_user_handler(void)
{
  // Increment the GPIO counter
  *(gpio_base+0) = (++dat)%256;
}


void or1k_timer_interrupt_handler_new(void)
{
  /* Re-initialise timer */
  or1k_mtspr(SPR_TTMR, SPR_TTMR_IE | SPR_TTMR_RT | 
	     (or1k_timer_period & SPR_TTMR_PERIOD));

  /* Increment global ticks value */
  or1k_timer_ticks++;

  /* Call a user handler function */
  or1k_timer_user_handler();

}

int main(void)
{

  /* Set the GPIO to all out */
  char dir = 0xff;
  *(gpio_base+1) = dir;
  /* Initialise with the first data value */
  *(gpio_base+0) = dat;

  /* Initialise the counter at 1Hz, it won't start yet */
  or1k_timer_init(10000);

  /* Install a custom timer handler */
  or1k_exception_handler_add(0x5,or1k_timer_interrupt_handler_new);
  
  /* OK, start the timer now */
  or1k_timer_enable();


  /* Now loop and do nothing, the timer will interrupt us when it fires */
  while (1);

  return 0;
}
