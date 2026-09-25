#include "header.h"

void Init_system(void)
{
    Init_LCD();
    Init_KPM();
    UART0_Init();
    I2C_Init();
    RTC_Init();
    Init_interrupt();

    IO0DIR |= (1U<<24) | (1U<<25); /* LEDs */
    IO0CLR = (1U<<24) | (1U<<25);

    LCD_Clear();
    StrLCD("INTELLIVOTE");
    LCD_Goto(1,0);
    StrLCD("RFID VOTING");
    delay_ms(1500);
}

static u8 read_2digit(void)
{
    u8 a,b;
    a=(u8)(KeyScan()-'0');
    b=(u8)(KeyScan()-'0');
    return (u8)(a*10+b);
}

void Edit_RTC_Info(void)
{
    u8 h,m,s,d,mo,y;
    LCD_Clear(); StrLCD("SET HHMMSS");
    h=read_2digit(); m=read_2digit(); s=read_2digit();
    SetRTCTimeInfo(h,m,s);

    LCD_Clear(); StrLCD("SET DDMMYY");
    d=read_2digit(); mo=read_2digit(); y=read_2digit();
    SetRTCDateInfo(d,mo,y);

    LCD_Clear(); StrLCD("RTC UPDATED");
    delay_ms(1000);
}

void checking_point(void)
{
    while(1)
    {
        if(officer_request)
        {
            officer_request=0;
            officer_card_checking();
        }

        if(CardPlaced)
        {
            CardPlaced=0;
            user_card_checking();
        }

        LCD_Goto(0,0);
        StrLCD("WAITING FOR CARD ");
    }
}
