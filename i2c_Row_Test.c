#include "lpc21xx.h"
#include "delay.h"
#include "types.h"

/* LPC2148 I2C0: P0.2=SCL0, P0.3=SDA0 */
#define EEPROM_W 0xA0
#define EEPROM_R 0xA1

void I2C_Init(void)
{
    PINSEL0 &= ~0x000000F0;
    PINSEL0 |=  0x00000050;
    I2C0SCLH = 75;
    I2C0SCLL = 75;
    I2C0CONSET = 0x40;
}

static void i2c_start(void)
{
    I2C0CONSET = 0x20;
    I2C0CONCLR = 0x08;
    while(!(I2C0CONSET & 0x08));
}

static void i2c_stop(void)
{
    I2C0CONSET = 0x10;
    I2C0CONCLR = 0x28;
}

static void i2c_write(u8 d)
{
    I2C0DAT = d;
    I2C0CONCLR = 0x28;
    while(!(I2C0CONSET & 0x08));
}

static u8 i2c_read_ack(void)
{
    I2C0CONSET = 0x04;
    I2C0CONCLR = 0x28;
    while(!(I2C0CONSET & 0x08));
    I2C0CONCLR = 0x04;
    return I2C0DAT;
}

static u8 i2c_read_nack(void)
{
    I2C0CONCLR = 0x04;
    I2C0CONCLR = 0x28;
    while(!(I2C0CONSET & 0x08));
    return I2C0DAT;
}

void EEPROM_WriteByte(u16 addr, u8 data)
{
    i2c_start();
    i2c_write(EEPROM_W);
    i2c_write((u8)(addr >> 8));
    i2c_write((u8)addr);
    i2c_write(data);
    i2c_stop();
    delay_ms(8);
}

u8 EEPROM_ReadByte(u16 addr)
{
    u8 d;
    i2c_start();
    i2c_write(EEPROM_W);
    i2c_write((u8)(addr >> 8));
    i2c_write((u8)addr);

    i2c_start();
    i2c_write(EEPROM_R);
    d = i2c_read_nack();
    i2c_stop();
    return d;
}

void EEPROM_WriteBuffer(u16 addr, const u8 *p, u16 n)
{
    while(n--)
        EEPROM_WriteByte(addr++, *p++);
}

void EEPROM_ReadBuffer(u16 addr, u8 *p, u16 n)
{
    while(n--)
        *p++ = EEPROM_ReadByte(addr++);
}
