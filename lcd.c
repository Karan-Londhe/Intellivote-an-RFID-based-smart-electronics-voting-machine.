#include "lpc21xx.h"
#include "delay.h"
#include "lcd_defines.h"
#include "lcd.h"

static void lcd_nibble(u8 n)
{
    IO0CLR = LCD_DATA_MASK;
    IO0SET = ((u32)(n & 0x0F) << 12);
    IO0SET = LCD_EN;
    delay_us(2);
    IO0CLR = LCD_EN;
    delay_us(50);
}

void CmdLCD(u8 cmd)
{
    IO0CLR = LCD_RS;
    lcd_nibble(cmd >> 4);
    lcd_nibble(cmd);
    if(cmd == LCD_CLEAR || cmd == LCD_HOME) delay_ms(2);
}

void CharLCD(u8 ch)
{
    IO0SET = LCD_RS;
    lcd_nibble(ch >> 4);
    lcd_nibble(ch);
}

void StrLCD(const char *s)
{
    while(*s) CharLCD((u8)*s++);
}

void U32LCD(u32 n)
{
    char b[11];
    s8 i = 0;
    if(n == 0) { CharLCD('0'); return; }
    while(n && i < 10) { b[i++] = (char)('0' + (n % 10)); n /= 10; }
    while(i) CharLCD((u8)b[--i]);
}

void LCD_Goto(u8 row, u8 col)
{
    static const u8 addr[4] = {0x80,0xC0,0x94,0xD4};
    CmdLCD(addr[row & 3] + col);
}

void LCD_Clear(void)
{
    CmdLCD(LCD_CLEAR);
}

void Init_LCD(void)
{
    IO0DIR |= LCD_DATA_MASK | LCD_RS | LCD_EN;
    delay_ms(20);
    IO0CLR = LCD_RS | LCD_EN;
    lcd_nibble(0x03); delay_ms(5);
    lcd_nibble(0x03); delay_us(150);
    lcd_nibble(0x02);
    CmdLCD(LCD_4BIT_2LINE);
    CmdLCD(LCD_DISP_ON);
    CmdLCD(LCD_ENTRY);
    CmdLCD(LCD_CLEAR);
}
