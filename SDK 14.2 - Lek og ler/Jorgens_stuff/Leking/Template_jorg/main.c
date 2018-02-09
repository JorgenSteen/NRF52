

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"


void Basic_Init(void);

int main(void)
{
  Basic_Init();
    while (true)
    {
      
        for (int i = 17; i < 21; i++)
        {
            NRF_GPIO->OUT &= ~(1<<i);
            nrf_delay_ms(500);
        }
        for (int i = 17; i < 21; i++)
        {
            NRF_GPIO->OUT |= (1<<i);
            nrf_delay_ms(500);
        }
    }
}
void Basic_Init(void)
{
  for(uint8_t i = 17; i<21; i++)
  {
    NRF_GPIO->PIN_CNF[i] = 1;
    }
}