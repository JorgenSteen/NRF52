

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"


void Basic_Init(void);

void UART_init(void);
void UART_send(char UART_IN);

int main(void)
{
  Basic_Init();
  UART_init();
    while (true)
    {
      
        for (int i = 17; i < 21; i++)
        {
            NRF_GPIO->OUT &= ~(1<<i);
            nrf_delay_ms(200);
        }
        for (int i = 17; i < 21; i++)
        {
            NRF_GPIO->OUT |= (1<<i);
            nrf_delay_ms(200);
             UART_send((char) i);
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

void UART_init(void)
{
    #define RX_PIN 8
    #define TX_PIN 6
    #define RTS_PIN 5
    #define CTS_PIN 7
    NRF_UART0->ENABLE = true;
    NRF_UART0->PSELRXD = RX_PIN; //(1<<RX_PIN);
    NRF_UART0->PSELTXD = TX_PIN; //(1<<TX_PIN);
    NRF_UART0->PSELRTS = RTS_PIN; //(1<<RTS_PIN);
    NRF_UART0->PSELCTS = CTS_PIN; //(1<<CTS_PIN);
    NRF_UART0->BAUDRATE = 0x01D7E000;
   
    //NRF_UART0->EVENTS_NCTS = true;

}
void UART_send(char UART_IN)
{
    NRF_UART0->TXD = UART_IN;
    NRF_UART0->TASKS_STARTTX = true;

    if (NRF_UART0->EVENTS_ERROR) {
        while(true);
    }

    while(!NRF_UART0->EVENTS_TXDRDY);
     NRF_UART0->EVENTS_TXDRDY = 0;

     


     
    
}
