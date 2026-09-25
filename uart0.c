#include "lpc21xx.h"
#include "uart0_defines.h"
#include "uart0.h"
#include "header.h"

volatile u8 CardPlaced = 0;
volatile u8 rfid_buf[RFID_LEN + 1];
static volatile u8 rfid_index = 0;
static volatile u8 rfid_receiving = 0;

static void uart0_isr(void) __irq
{
    u8 ch;
    u8 iid = U0IIR;

    if((iid & 0x0F) == 0x04 || (U0LSR & UART0_LSR_RDR))
    {
        ch = U0RBR;

        /* RFID frame: STX, 8 ASCII bytes, ETX */
        if(ch == 0x02)
        {
            rfid_index = 0;
            rfid_receiving = 1;
        }
        else if(ch == 0x03)
        {
            if(rfid_receiving && rfid_index == RFID_LEN)
            {
                rfid_buf[RFID_LEN] = 0;
                CardPlaced = 1;
            }
            rfid_receiving = 0;
        }
        else if(rfid_receiving && rfid_index < RFID_LEN)
        {
            rfid_buf[rfid_index++] = ch;
        }
    }

    VICVectAddr = 0;
}

void UART0_Init(void)
{
    /* P0.0=TXD0, P0.1=RXD0 */
    PINSEL0 &= ~0x0000000F;
    PINSEL0 |=  0x00000005;

    U0LCR = 0x83;       /* 8-bit, 1 stop, no parity, DLAB=1 */
    U0DLL = 97;         /* Example for PCLK around 15 MHz */
    U0DLM = 0;
    U0LCR = 0x03;

    U0FCR = 0x07;
    U0IER = UART0_IER_RBR;

    VICIntSelect &= ~(1U<<6);      /* IRQ */
    VICIntEnable |= (1U<<6);
    VICVectCntl0 = 0x20 | 6;
    VICVectAddr0 = (u32)uart0_isr;
}

void UART0_TxChar(u8 ch)
{
    while(!(U0LSR & (1U<<5)));
    U0THR = ch;
}

void UART0_TxStr(const char *s)
{
    while(*s) UART0_TxChar((u8)*s++);
}

u8 UART0_RxChar(void)
{
    while(!(U0LSR & UART0_LSR_RDR));
    return (u8)U0RBR;
}

void UART0_RxFlush(void)
{
    volatile u8 dummy;
    while(U0LSR & UART0_LSR_RDR)
        dummy = U0RBR;
}
