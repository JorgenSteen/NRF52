

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "SEGGER_RTT.h"
#include "nrf_log.h"



void Basic_Init(void);
void ADC_init(void);

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
      SEGGER_RTT_WriteString(0, "bitch please");
    }
}
void Basic_Init(void)
{
  for(uint8_t i = 17; i<21; i++)
  {
    NRF_GPIO->PIN_CNF[i] = 1;
    }
}

void ADC_init(void)
{
    NRF_SAADC->ENABLE = true;
    NRF_SAADC->CH[0].PSELP = 1;
    NRF_SAADC->CH[0].CONFIG = (1<<8);
}

uint32_t ADC_read(void)
{
    NRF_SAADC->TASKS_SAMPLE = true;
    while(!(NRF_SAADC->EVENTS_DONE == true));
    uint32_t ADC_Result = NRF_SAADC->RESULT.PTR;
    return(ADC_Result);
 }
