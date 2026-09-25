#ifndef UART0_H
#define UART0_H
#include "types.h"
void UART0_Init(void);
void UART0_TxChar(u8 ch);
void UART0_TxStr(const char *s);
u8 UART0_RxChar(void);
void UART0_RxFlush(void);
#endif
