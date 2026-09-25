#ifndef HEADER_H
#define HEADER_H

#include "types.h"
#include "lpc21xx.h"
#include "delay.h"
#include "lcd.h"
#include "kpm.h"
#include "uart0.h"
#include "rtc.h"
#include "switch.h"

/* Application states */
#define VOTING_OFF      0
#define VOTING_ON       1
#define MAX_VOTERS      5
#define RFID_LEN        8
#define PIN_LEN         4
#define PARTY_COUNT     3

/* EEPROM map */
#define EE_FLAG_ADDR        0x0000
#define EE_START_ADDR       0x0010
#define EE_END_ADDR         0x0013
#define EE_OFFICER_ID_ADDR  0x0100
#define EE_OFFICER_PIN_ADDR 0x0110
#define EE_VOTER_BASE       0x0200
#define EE_VOTER_STRIDE     0x0020
#define EE_VOTED_OFFSET     0x0008
#define EE_PIN_OFFSET       0x0010
#define EE_VOTE_BASE        0x0400

extern volatile u8 CardPlaced;
extern volatile u8 officer_request;
extern volatile u8 rfid_buf[RFID_LEN + 1];

void Init_system(void);
void checking_point(void);
void Officer_Menu(void);
void User_Menu(u8 voter_index);
void officer_card_checking(void);
void user_card_checking(void);
void setStartEndVoting(void);
void startVoting(void);
void stopVoting(void);
void viewResults(void);
void resetVoting(void);
void Edit_RTC_Info(void);
void CHANGE_PASSWORD(u8 voter_index);
u8 authenticate_voter(u8 *id, u8 *index);
u8 authenticate_officer(u8 *id);
u8 password_check(u8 voter_index);
void cast_vote(u8 voter_index);
u8 voting_time_valid(void);

#endif
