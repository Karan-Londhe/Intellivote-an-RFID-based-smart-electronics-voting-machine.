#include "header.h"
#include <string.h>

static const u8 officer_id[RFID_LEN+1] = "99999999";

u8 authenticate_officer(u8 *id)
{
    return (strncmp((char*)id,(char*)officer_id,RFID_LEN)==0);
}

static u8 officer_pin_ok(void)
{
    u8 i;
    char pin[PIN_LEN];

    LCD_Clear(); StrLCD("OFFICER PIN:");
    for(i=0;i<PIN_LEN;i++)
    {
        pin[i]=(char)KeyScan();
        CharLCD('*');
    }

    {
        char stored[PIN_LEN];
        EEPROM_ReadBuffer(EE_OFFICER_PIN_ADDR,(u8*)stored,PIN_LEN);
        return (memcmp(pin,stored,PIN_LEN)==0);
    }
}

void startVoting(void)
{
    EEPROM_WriteByte(EE_FLAG_ADDR,VOTING_ON);
    LCD_Clear(); StrLCD("VOTING STARTED");
    delay_ms(1000);
}

void stopVoting(void)
{
    EEPROM_WriteByte(EE_FLAG_ADDR,VOTING_OFF);
    LCD_Clear(); StrLCD("VOTING STOPPED");
    delay_ms(1000);
}

void viewResults(void)
{
    u8 i,c;
    LCD_Clear();
    for(i=0;i<PARTY_COUNT;i++)
    {
        c=EEPROM_ReadByte(EE_VOTE_BASE+i);
        LCD_Goto(i,0);
        StrLCD("P"); CharLCD('1'+i); StrLCD(": ");
        U32LCD(c);
    }
    delay_ms(2000);
}

void resetVoting(void)
{
    u8 i;
    for(i=0;i<PARTY_COUNT;i++)
        EEPROM_WriteByte(EE_VOTE_BASE+i,0);

    for(i=0;i<MAX_VOTERS;i++)
        EEPROM_WriteByte(EE_VOTER_BASE+i*EE_VOTER_STRIDE+
                         EE_VOTED_OFFSET,0);

    LCD_Clear(); StrLCD("VOTES RESET");
    delay_ms(1000);
}

void Officer_Menu(void)
{
    u8 key;

    if(!officer_pin_ok())
    {
        LCD_Clear(); StrLCD("WRONG OFFICER PIN");
        delay_ms(1000);
        return;
    }

    while(1)
    {
        LCD_Clear();
        StrLCD("1 TIME 2 START");
        LCD_Goto(1,0); StrLCD("3 STOP 4 RESULT");
        LCD_Goto(2,0); StrLCD("5 RESET 6 RTC");
        LCD_Goto(3,0); StrLCD("7 EXIT");

        key=KeyScan();

        if(key=='1') setStartEndVoting();
        else if(key=='2') startVoting();
        else if(key=='3') stopVoting();
        else if(key=='4') viewResults();
        else if(key=='5') resetVoting();
        else if(key=='6') Edit_RTC_Info();
        else if(key=='7') return;
    }
}

void officer_card_checking(void)
{
    /* The switch requests officer access; the next RFID card must be officer card. */
    LCD_Clear(); StrLCD("PLACE OFFICER CARD");

    CardPlaced=0;
    while(!CardPlaced);
    CardPlaced=0;

    if(authenticate_officer((u8*)rfid_buf))
        Officer_Menu();
    else
    {
        LCD_Clear(); StrLCD("NOT OFFICER CARD");
        delay_ms(1000);
    }
}

void checking_point(void); /* implemented in init_system.c */
