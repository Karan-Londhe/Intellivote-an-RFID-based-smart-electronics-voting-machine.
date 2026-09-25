/*
 * Run this as a SEPARATE Keil project once to initialize AT24C256.
 * Do not add this file to the normal voting application's build because it
 * contains its own main().
 */
#include "header.h"

static const u8 officer_pin[PIN_LEN] = {'9','9','9','9'};
static const u8 voter_pin[PIN_LEN]   = {'1','2','3','4'};

int main(void)
{
    u8 i;
    Init_LCD();
    I2C_Init();

    EEPROM_WriteByte(EE_FLAG_ADDR,VOTING_OFF);

    EEPROM_WriteBuffer(EE_OFFICER_PIN_ADDR,officer_pin,PIN_LEN);

    for(i=0;i<MAX_VOTERS;i++)
    {
        EEPROM_WriteBuffer(EE_VOTER_BASE+i*EE_VOTER_STRIDE+EE_PIN_OFFSET,
                           voter_pin,PIN_LEN);
        EEPROM_WriteByte(EE_VOTER_BASE+i*EE_VOTER_STRIDE+EE_VOTED_OFFSET,0);
    }

    for(i=0;i<PARTY_COUNT;i++)
        EEPROM_WriteByte(EE_VOTE_BASE+i,0);

    /* Example default voting window: 09:00:00 to 20:00:00 */
    EEPROM_WriteByte(EE_START_ADDR,9);
    EEPROM_WriteByte(EE_START_ADDR+1,0);
    EEPROM_WriteByte(EE_START_ADDR+2,0);
    EEPROM_WriteByte(EE_END_ADDR,20);
    EEPROM_WriteByte(EE_END_ADDR+1,0);
    EEPROM_WriteByte(EE_END_ADDR+2,0);

    LCD_Clear();
    StrLCD("EEPROM INIT OK");
    while(1);
}
