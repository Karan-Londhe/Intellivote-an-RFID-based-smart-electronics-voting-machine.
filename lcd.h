#ifndef LCD_H
#define LCD_H
#include "types.h"
void Init_LCD(void);
void CmdLCD(u8 cmd);
void CharLCD(u8 ch);
void StrLCD(const char *s);
void U32LCD(u32 n);
void LCD_Goto(u8 row, u8 col);
void LCD_Clear(void);
#endif
