#include "delay.h"

void delay_us(u32 us)
{
    volatile u32 i;
    while(us--)
        for(i=0; i<4; i++);
}

void delay_ms(u32 ms)
{
    while(ms--)
        delay_us(1000);
}

void delay_s(u32 sec)
{
    while(sec--)
        delay_ms(1000);
}
