/*
 * Standalone RTC test program.
 * Build this separately from the normal voting application.
 */
#include "header.h"

int main(void)
{
    u8 h,m,s,d,mo,y;

    Init_LCD();
    I2C_Init();
    RTC_Init();

    SetRTCTimeInfo(12,10,45);
    SetRTCDateInfo(22,9,26);

    while(1)
    {
        GetRTCTimeInfo(&h,&m,&s);
        GetRTCDateInfo(&d,&mo,&y);

        LCD_Clear();
        StrLCD("TIME ");
        if(h<10) CharLCD('0'); U32LCD(h); CharLCD(':');
        if(m<10) CharLCD('0'); U32LCD(m); CharLCD(':');
        if(s<10) CharLCD('0'); U32LCD(s);

        LCD_Goto(1,0);
        StrLCD("DATE ");
        if(d<10) CharLCD('0'); U32LCD(d); CharLCD('/');
        if(mo<10) CharLCD('0'); U32LCD(mo); CharLCD('/');
        if(y<10) CharLCD('0'); U32LCD(y);

        delay_ms(500);
    }
}
