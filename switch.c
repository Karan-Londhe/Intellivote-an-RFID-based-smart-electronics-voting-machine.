#include "lpc21xx.h"
#include "switch.h"

volatile u8 officer_request = 0;

static void eint2_isr(void) __irq
{
    officer_request = 1;
    EXTINT = (1U<<2);
    VICVectAddr = 0;
}

void Init_interrupt(void)
{
    /* P0.15 as EINT2, falling-edge */
    PINSEL0 &= ~(3U<<30);
    PINSEL0 |=  (2U<<30);
    EXTMODE |= (1U<<2);
    EXTPOLAR &= ~(1U<<2);

    VICIntSelect &= ~(1U<<16);
    VICIntEnable |= (1U<<16);
    VICVectCntl1 = 0x20 | 16;
    VICVectAddr1 = (u32)eint2_isr;
}
