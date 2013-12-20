#include <or1k-support.h>
#include <stdio.h>
#include <spr-defs.h>

#define TIMER_RESOLUTION 10000
#define PWM_RESOLUTION 100

char* gpio_base = (char*) 0x91000000;

/* Tick rate storage */
extern unsigned long or1k_timer_period;

extern volatile unsigned long or1k_timer_ticks;

int on_ticks;

unsigned char led_state;

unsigned char led_pattern = 0x3;

int pattern_update_rate = TIMER_RESOLUTION/25;
int pattern_dir = 1; /* 1 left, 0 right */
int pattern_edge_wait_counter = 0;
int pattern_edge_wait_amount = 6;

void or1k_timer_user_handler(void)
{

  /* Should we update the pattern? */
  if (or1k_timer_ticks % pattern_update_rate==0)
    {
      if (led_pattern & 0x80)
	{
	  if (pattern_edge_wait_counter==pattern_edge_wait_amount)
	    {
	      pattern_edge_wait_counter = 0;
	      pattern_dir = 0;
	      led_pattern >>= 1;
	    }
	  else
	    {
	      pattern_edge_wait_counter++;
	    }
	}
      else if (led_pattern & 0x01)
	{
	  if (pattern_edge_wait_counter==pattern_edge_wait_amount)
	    {
	      pattern_edge_wait_counter=0;
	      pattern_dir = 1;
	      led_pattern <<= 1;
	    }
	  else
	    {
	      pattern_edge_wait_counter++;
	    }
	}
      else
	{
	  led_pattern = pattern_dir ? (led_pattern<<1) : (led_pattern>>1);
	}
    }
  
  /* Should the LED be on or off? */
  if (or1k_timer_ticks % PWM_RESOLUTION > on_ticks && led_state)
    {
      /* Turn them off */
      *(gpio_base+0) = 0;
      led_state = 0;
    }
  else if (or1k_timer_ticks % PWM_RESOLUTION < on_ticks && !led_state)
    {
      /* Turn it on */
      *(gpio_base+0) = led_pattern;
      led_state = 1;
    }
  else if (led_state)
    {
      /* Update incase the pattern updated */
      *(gpio_base+0) = led_pattern;
    }
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

  printf("\r\nTimer + LED PWM example\r\n");
  printf("-----------------------\r\n");
  printf("Press your up and down arrows and increase and decrease the brightness\r\n");
  printf("of the LEDs, respectively.\r\n");
  printf("The left and right arrows decrease and increase the rate at which they\r\n");
  printf("are dimmed, respectively..\r\n");

  

  int pwm_cycle_modifier_divisor = 15;

  int pwm_cycle_modifier_amount = PWM_RESOLUTION/pwm_cycle_modifier_divisor;

  /* Set the GPIO to all out */
  char dir = 0xff;
  *(gpio_base+1) = dir;

  led_state = 1;

  /* Initialise with the first data value */
  *(gpio_base+0) = led_state;

  // Put the LEDs at full on to begin with
  on_ticks = PWM_RESOLUTION-1;

  /* Initialise the counter to the desired resolution, it won't start yet */
  or1k_timer_init(TIMER_RESOLUTION);

  /* Install a custom timer handler */
  or1k_exception_handler_add(0x5,or1k_timer_interrupt_handler_new);
  
  /* OK, start the timer now */
  or1k_timer_enable();

  /* Now loop and do nothing, the timer will interrupt us when it fires */
  char last_chars[3];
  while (1)
    {
      unsigned char c = __uart_getc();
      last_chars[2] = last_chars[1];
      last_chars[1] = last_chars[0];
      last_chars[0] = c;
      
      if (last_chars[2]==27 && 
	  last_chars[1]==91)
	{
	  /* We have an arrow sequence */
	  if (c==66)
	    {
	      /* Down arrow - reduce on_ticks*/
	      if (on_ticks - pwm_cycle_modifier_amount < 0)
		{
		  on_ticks = 1;
		}
	      else
		{
		  on_ticks -= pwm_cycle_modifier_amount;
		}
	    }
	  if (c==65)
	    {
	      /* Up arrow - increase on_ticks*/
	      if (on_ticks + pwm_cycle_modifier_amount > PWM_RESOLUTION)
		{
		  on_ticks = PWM_RESOLUTION;
		}
	      else
		{
		  on_ticks += pwm_cycle_modifier_amount;
		}
	    }
	  if (c==67)
	    {
	      /* Left arrow - decrease the adjust amount */
	      if (pwm_cycle_modifier_divisor > 0)
		{
		  pwm_cycle_modifier_divisor -= 1;
		}
	      /* Recalculate the pwm_cycle_modifier_amount value */
	      pwm_cycle_modifier_amount = PWM_RESOLUTION / pwm_cycle_modifier_divisor;
	    }
	  if (c==68)
	    {
	      /* Right arrow - increase the adjust amount */
	      if (pwm_cycle_modifier_divisor < PWM_RESOLUTION)
		{
		  pwm_cycle_modifier_divisor += 1;
		}
	      /* Recalculate the pwm_cycle_modifier_amount value */
	      pwm_cycle_modifier_amount = PWM_RESOLUTION / pwm_cycle_modifier_divisor;
	    }
	}
    }

  return 0;
}
