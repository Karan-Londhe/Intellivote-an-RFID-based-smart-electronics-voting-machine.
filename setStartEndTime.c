#include "header.h"

static u8 two_digits(void)
{
    u8 a=(u8)(KeyScan()-'0');
    u8 b=(u8)(KeyScan()-'0');
    return (u8)(a*10+b);
}

void setStartEndVoting(void)
{
    u8 h,m,s;

    LCD_Clear(); StrLCD("START HHMMSS");
    h=two_digits(); m=two_digits(); s=two_digits();
    EEPROM_WriteByte(EE_START_ADDR,h);
    EEPROM_WriteByte(EE_START_ADDR+1,m);
    EEPROM_WriteByte(EE_START_ADDR+2,s);

    LCD_Clear(); StrLCD("END HHMMSS");
    h=two_digits(); m=two_digits(); s=two_digits();
    EEPROM_WriteByte(EE_END_ADDR,h);
    EEPROM_WriteByte(EE_END_ADDR+1,m);
    EEPROM_WriteByte(EE_END_ADDR+2,s);

    LCD_Clear(); StrLCD("TIME SAVED");
    delay_ms(1000);
}

u8 voting_time_valid(void)
{
    u8 h,m,s,sh,sm,ss,eh,em,es;
    GetRTCTimeInfo(&h,&m,&s);

    sh=EEPROM_ReadByte(EE_START_ADDR);
    sm=EEPROM_ReadByte(EE_START_ADDR+1);
    ss=EEPROM_ReadByte(EE_START_ADDR+2);
    eh=EEPROM_ReadByte(EE_END_ADDR);
    em=EEPROM_ReadByte(EE_END_ADDR+1);
    es=EEPROM_ReadByte(EE_END_ADDR+2);

    {
        u32 now=(u32)h*3600+(u32)m*60+s;
        u32 start=(u32)sh*3600+(u32)sm*60+ss;
        u32 end=(u32)eh*3600+(u32)em*60+es;
        return (now>=start && now<=end);
    }
}
