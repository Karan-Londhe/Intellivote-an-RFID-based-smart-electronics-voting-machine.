#include "lpc21xx.h"
#include "rtc_defines.h"
#include "rtc.h"
#include "delay.h"

static u8 bcd_to_dec(u8 x){ return (u8)(((x>>4)*10)+(x&0x0F)); }
static u8 dec_to_bcd(u8 x){ return (u8)(((x/10)<<4)|(x%10)); }

static void rtc_write(u8 reg, u8 data)
{
    I2C0CONSET=0x20; I2C0CONCLR=0x08; while(!(I2C0CONSET&8));
    I2C0DAT=RTC_ADDR_W; I2C0CONCLR=0x28; while(!(I2C0CONSET&8));
    I2C0DAT=reg;        I2C0CONCLR=0x28; while(!(I2C0CONSET&8));
    I2C0DAT=data;       I2C0CONCLR=0x28; while(!(I2C0CONSET&8));
    I2C0CONSET=0x10; I2C0CONCLR=0x28;
}

static u8 rtc_read(u8 reg)
{
    u8 d;
    I2C0CONSET=0x20; I2C0CONCLR=0x08; while(!(I2C0CONSET&8));
    I2C0DAT=RTC_ADDR_W; I2C0CONCLR=0x28; while(!(I2C0CONSET&8));
    I2C0DAT=reg;        I2C0CONCLR=0x28; while(!(I2C0CONSET&8));
    I2C0CONSET=0x20; I2C0CONCLR=0x08; while(!(I2C0CONSET&8));
    I2C0DAT=RTC_ADDR_R; I2C0CONCLR=0x28; while(!(I2C0CONSET&8));
    I2C0CONCLR=0x04|0x20|0x10|0x08;
    while(!(I2C0CONSET&8));
    d=I2C0DAT;
    I2C0CONSET=0x10; I2C0CONCLR=0x28;
    return d;
}

void RTC_Init(void)
{
    /* RTC is external and uses I2C0 in this implementation. */
}

void GetRTCTimeInfo(u8 *h,u8 *m,u8 *s)
{
    *s=bcd_to_dec(rtc_read(0));
    *m=bcd_to_dec(rtc_read(1));
    *h=bcd_to_dec(rtc_read(2)&0x3F);
}

void SetRTCTimeInfo(u8 h,u8 m,u8 s)
{
    rtc_write(0,dec_to_bcd(s));
    rtc_write(1,dec_to_bcd(m));
    rtc_write(2,dec_to_bcd(h));
}

void GetRTCDateInfo(u8 *d,u8 *mo,u8 *y)
{
    *d=bcd_to_dec(rtc_read(4));
    *mo=bcd_to_dec(rtc_read(5));
    *y=bcd_to_dec(rtc_read(6));
}

void SetRTCDateInfo(u8 d,u8 mo,u8 y)
{
    rtc_write(4,dec_to_bcd(d));
    rtc_write(5,dec_to_bcd(mo));
    rtc_write(6,dec_to_bcd(y));
}
