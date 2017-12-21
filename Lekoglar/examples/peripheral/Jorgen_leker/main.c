
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"

void timer1_init(void);

int main(void)
{
    NRF_GPIO->PIN_CNF[17] = 1;
    NRF_GPIO->PIN_CNF[18] = 1;
    NRF_GPIO->PIN_CNF[19] = 1;
    NRF_GPIO->PIN_CNF[20] = 1;
    NRF_GPIO->PIN_CNF[13] = (3<<2);
    timer1_init();

    while (true)
    {
  //  NRF_GPIO->OUT ^= (1<<17);
    //  nrf_delay_ms(500);
      if(!(NRF_GPIO->IN & (1<<13)))
      {
        NRF_GPIO->OUT ^= (1<<20);
    //    nrf_delay_ms(500);
      }

    }
}

void TIMER1_IRQHandler(void) {
  if (NRF_TIMER1->EVENTS_COMPARE[0]) {
    NRF_TIMER1->EVENTS_COMPARE[0] = 0;
    NRF_GPIO->OUT ^= (1<<19);
  }
}

void timer1_init(void)
{
  NRF_TIMER1->TASKS_STOP = 1;
  NRF_TIMER1->SHORTS = 1;
  NRF_TIMER1->MODE= 0;
  NRF_TIMER1->BITMODE =0;
  NRF_TIMER1->PRESCALER = 0x9;
  NRF_TIMER1->CC[0]= 0xFFFF;
  NRF_TIMER1->INTENSET |= (1<<16)   ;
  __DMB();
  NVIC_EnableIRQ(TIMER1_IRQn);


  NRF_TIMER1->TASKS_START = 1;
}
