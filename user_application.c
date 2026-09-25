#include "header.h"
#include <string.h>

static u8 voter_ids[MAX_VOTERS][RFID_LEN+1] = {
    "12345678","23456789","34567890","45678901","56789012"
};

u8 authenticate_voter(u8 *id, u8 *index)
{
    u8 i;
    for(i=0;i<MAX_VOTERS;i++)
    {
        if(strncmp((char*)id,(char*)voter_ids[i],RFID_LEN)==0)
        {
            *index=i;
            return 1;
        }
    }
    return 0;
}

static u8 read_pin(u8 voter_index)
{
    u8 i,key;
    char pin[PIN_LEN+1];
    char stored[PIN_LEN+1];

    for(i=0;i<PIN_LEN;i++)
    {
        key=KeyScan();
        if(key<'0' || key>'9') return 0;
        pin[i]=(char)key;
        CharLCD('*');
    }
    pin[PIN_LEN]=0;

    EEPROM_ReadBuffer(EE_VOTER_BASE + voter_index*EE_VOTER_STRIDE + EE_PIN_OFFSET,
                      (u8*)stored, PIN_LEN);
    stored[PIN_LEN]=0;

    return (strncmp(pin,stored,PIN_LEN)==0);
}

u8 password_check(u8 voter_index)
{
    LCD_Goto(2,0);
    StrLCD("PIN: ");
    return read_pin(voter_index);
}

void CHANGE_PASSWORD(u8 voter_index)
{
    u8 i;
    char pin[PIN_LEN];

    LCD_Clear(); StrLCD("NEW PIN:");
    for(i=0;i<PIN_LEN;i++) { pin[i]=(char)KeyScan(); CharLCD('*'); }

    EEPROM_WriteBuffer(EE_VOTER_BASE + voter_index*EE_VOTER_STRIDE +
                       EE_PIN_OFFSET,(u8*)pin,PIN_LEN);

    LCD_Clear(); StrLCD("PIN UPDATED");
    delay_ms(1000);
}

void cast_vote(u8 voter_index)
{
    u8 key, addr;
    u8 count;

    LCD_Clear();
    StrLCD("1-P1 2-P2 3-P3");
    LCD_Goto(1,0);
    StrLCD("SELECT:");

    while(1)
    {
        key=KeyScan();
        if(key>='1' && key<='3') break;
    }

    addr=(u8)(EE_VOTE_BASE + (key-'1'));
    count=EEPROM_ReadByte(addr);
    EEPROM_WriteByte(addr,(u8)(count+1));

    EEPROM_WriteByte(EE_VOTER_BASE + voter_index*EE_VOTER_STRIDE +
                     EE_VOTED_OFFSET,1);

    IO0SET=(1U<<24);
    LCD_Clear(); StrLCD("VOTE CASTED");
    delay_ms(1200);
    IO0CLR=(1U<<24);
}

void User_Menu(u8 voter_index)
{
    u8 key, voted;

    voted=EEPROM_ReadByte(EE_VOTER_BASE + voter_index*EE_VOTER_STRIDE +
                          EE_VOTED_OFFSET);

    LCD_Clear();
    StrLCD("1 VOTE");
    LCD_Goto(1,0); StrLCD("2 CHANGE PIN");
    LCD_Goto(2,0); StrLCD("3 EXIT");

    while(1)
    {
        key=KeyScan();
        if(key=='1')
        {
            if(voted)
            {
                LCD_Clear(); StrLCD("ALREADY VOTED");
                delay_ms(1000);
            }
            else
            {
                if(password_check(voter_index)) cast_vote(voter_index);
                else { LCD_Clear(); StrLCD("WRONG PIN"); delay_ms(1000); }
            }
            return;
        }
        if(key=='2') { CHANGE_PASSWORD(voter_index); return; }
        if(key=='3') return;
    }
}

void user_card_checking(void)
{
    u8 idx;

    if(!voting_time_valid())
    {
        LCD_Clear(); StrLCD("VOTING CLOSED");
        IO0SET=(1U<<25);
        delay_ms(1000);
        IO0CLR=(1U<<25);
        return;
    }

    if(!authenticate_voter((u8*)rfid_buf,&idx))
    {
        LCD_Clear(); StrLCD("CARD NOT FOUND");
        delay_ms(1000);
        return;
    }

    LCD_Clear(); StrLCD("VOTER AUTH OK");
    delay_ms(500);

    if(!password_check(idx))
    {
        LCD_Clear(); StrLCD("WRONG PIN");
        delay_ms(1000);
        return;
    }

    User_Menu(idx);
}
