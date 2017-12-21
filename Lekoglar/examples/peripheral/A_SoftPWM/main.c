
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"


#define SOFT_PWM_resolution 100
int GREEN_LED_value =100;
int RED_LED_value = 50;
int BLUE_LED_value = 0;

void Led_init(int nr_of_LED);
void TIMER1_init(void);
int counter = 0;

int main(void)
{
   TIMER1_init();
    Led_init(4);
    /* Toggle LEDs. */
    while (true)
    {

            NRF_GPIO->OUTSET = (1<<17);
            nrf_delay_ms(1000);
            NRF_GPIO->OUTCLR = (1<<17);
            nrf_delay_ms(1000);
    }
}

void Led_init(int nr_of_LED)
{
  for (int i = 0; i < nr_of_LED; i++) {
    NRF_GPIO->PIN_CNF[17+i] |= 0x01;
  }

}
void TIMER1_init(void)
{
  NRF_TIMER1->TASKS_STOP = 0x01;
  NRF_TIMER1->SHORTS |= (1<<0);
  NRF_TIMER1->INTENSET |= (1<<16);
  NRF_TIMER1->MODE = 0x00;
  NRF_TIMER1->BITMODE = 0x00;
  NRF_TIMER1->PRESCALER = 0x2;
  NRF_TIMER1->CC[0] =2;//125;

  __DMB();
  NVIC_EnableIRQ(TIMER1_IRQn);
  NRF_TIMER1->TASKS_START = 0x01;


}
void TIMER1_IRQHandler(void)
{
  if (NRF_TIMER1->EVENTS_COMPARE[0]) {
    NRF_TIMER1->EVENTS_COMPARE[0] = 0;
      if (counter<5)
      {
        counter = SOFT_PWM_resolution;
        NRF_GPIO->OUTCLR = (1<<18)| (1<<19) | (1<<20);
      }


    	counter--;
    	if ((counter<RED_LED_value))
    	{
    		  NRF_GPIO->OUTSET = (1<<18);
    	}

    	if ((counter<GREEN_LED_value))
    	{
          NRF_GPIO->OUTSET = (1<<19);
    	}
      if (counter<BLUE_LED_value)
      {
          NRF_GPIO->OUTSET = (1<<20);
      }
    }
}
