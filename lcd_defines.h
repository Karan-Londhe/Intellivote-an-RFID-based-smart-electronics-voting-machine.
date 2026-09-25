#ifndef LCD_DEFINES_H
#define LCD_DEFINES_H

#define LCD_DATA_MASK   0x0000F000
#define LCD_RS          (1U<<8)
#define LCD_EN          (1U<<10)

#define LCD_CMD         0
#define LCD_DATA        1

#define LCD_CLEAR       0x01
#define LCD_HOME        0x02
#define LCD_4BIT_2LINE  0x28
#define LCD_DISP_ON     0x0C
#define LCD_ENTRY       0x06

#endif
