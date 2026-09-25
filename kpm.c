#include "lpc21xx.h"
#include "delay.h"
#include "kpm_defines.h"
#include "kpm.h"

static const u8 keymap[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

void Init_KPM(void)
{
    IO1DIR |= KPM_ROW_MASK;
    IO1DIR &= ~KPM_COL_MASK;
    IO1SET = KPM_COL_MASK;
}

u8 KeyScan(void)
{
    u8 r, c;
    while(1)
    {
        for(r=0; r<4; r++)
        {
            IO1SET = KPM_ROW_MASK;
            IO1CLR = (1U << (16+r));
            delay_us(100);

            if((IO1PIN & KPM_COL_MASK) != KPM_COL_MASK)
            {
                for(c=0; c<4; c++)
                {
                    if(!(IO1PIN & (1U << (20+c))))
                    {
                        while(!(IO1PIN & (1U << (20+c))));
                        return keymap[r][c];
                    }
                }
            }
        }
    }
}
