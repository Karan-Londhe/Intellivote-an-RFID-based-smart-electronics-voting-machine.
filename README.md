# Intellivote-an-RFID-based-smart-electronics-voting-machine.
IntelliVote is an RFID-based smart electronic voting machine using an ARM7 LPC2148 microcontroller. It authenticates voters through RFID, checks voting time using RTC, prevents duplicate voting, records votes in EEPROM, and displays voting status on an LCD. An officer mode allows authorized configuration of voting time.
  IntelliVote – RFID Based Smart Electronic Voting System
📌 Project Overview

IntelliVote is an RFID-based smart electronic voting system developed using the ARM7 LPC2148 microcontroller.

The system uses RFID technology to identify voters before allowing them to vote. Each voter is assigned a unique RFID card. The RFID reader reads the card information and the controller verifies the voter before proceeding with the voting process.

The system also uses RTC-based voting time control, EEPROM data storage, UART communication, LCD, and keypad to provide a controlled electronic voting process.

🎯 Aim

The main aim of IntelliVote is to provide a safe, fast, and reliable electronic voting system using RFID-based voter identification.

The system is designed to help prevent:
Unauthorized voting
Multiple voting by the same voter
Voting outside the authorized voting time

⚙️ Hardware Requirements
ARM7 LPC2148 Microcontroller
RFID Reader
RFID Cards
20x4 LCD
4x4 Matrix Keypad
Buzzer / LEDs
AT24C256 EEPROM
USB-to-UART Converter


💻 Software Requirements
Embedded C
Keil C Compiler
Flash Magic
Proteus (for simulation, if used)


🔌 Technologies and Protocols
Embedded C
ARM7 LPC2148
RFID
UART
I2C
EEPROM
RTC
LCD
4x4 Keypad
External Interrupt
🔄 System Working
1. System Initialization

When the system is powered ON, the controller initializes:

LCD
Keypad
UART
EEPROM
I2C
RTC

The project name and current RTC information can then be displayed on the LCD.

2. Waiting for RFID Card

The system displays:

Waiting for Card

When an RFID card is placed near the reader, the RFID reader transmits the card number through serial communication at 9600 baud rate. The controller receives the RFID data using a UART interrupt.

3. Voting Time Verification

The controller reads:

Voting start time
Voting end time
Voting enable status

from EEPROM.

It then reads the current time from the RTC and compares it with the configured voting duration.

If voting is disabled or the current time is outside the allowed voting period:

Voting Closed
Access Denied

The red LED is turned ON and the system returns to the waiting state.

4. Voter Authentication

If voting is enabled and the current time is valid, the RFID card is checked against the predefined voter IDs stored in EEPROM.

If the card is valid, the corresponding voter identification is displayed on the LCD.

5. Voter Menu

The voter menu provides:

1. VOTING
2. EDIT PASSWORD
3. EXIT




6. Password Verification

When the voter selects the voting option, the system asks for a password.

The password characters are displayed as:

****

instead of showing the actual numbers.

If the password is incorrect, the red LED is turned ON and the voter is denied access.

7. Vote Casting

After successful password verification, the party selection menu is displayed.

The selected party's vote count is:

Read from EEPROM
Incremented by one
Stored back into EEPROM

After successful voting, the system displays:

Vote Casted Successfully

and the green LED is turned ON.

8. Password Change

The voter can change the password by entering:

Old password
New password
Confirmation password

After successful validation, the new password is stored in EEPROM.

👨‍💼 Officer Mode

A separate RFID card is assigned to the election officer.

When the officer card is detected through the officer access mechanism, the officer menu is displayed.

The officer menu contains:

1. SET VOTING TIME
2. START VOTING
3. STOP VOTING
4. VIEW RESULT
5. RESET VOTING
6. RTC EDIT
7. EXIT


Officer Functions

Set Voting Time

The officer can configure:

Start hour
Start minute
End hour
End minute

The configured values are stored in EEPROM.

Start Voting

The officer enters the security password and enables the voting status flag in EEPROM.

Stop Voting

The officer enters the password and disables the voting status flag.

View Result

The officer can enter the password and view the party vote counts stored in EEPROM.

Reset Voting

The officer can reset the party vote counts and voting status.

RTC Edit

The officer can set the date and time through the keypad.

📡 RFID Communication

The RFID reader transmits the card number through serial communication.

For example, if the card number is:

12345678

the reader output is represented as:

0x02 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x03

The controller receives and processes this data to identify the RFID card.

📁 Project Modules

The project can be organized into separate driver and application modules:
main.c                  Main voting application
header.h                Application-wide declarations
types.h                 Fixed-width aliases
delay.c/h               Delay functions
lcd.c/h + lcd_defines.h LCD driver
kpm.c/h + kpm_defines.h Keypad driver
uart0.c/h + defines     RFID UART driver + interrupt
i2c_Row_Test.c          I2C + AT24C256 EEPROM driver
rtc.c/h + defines.h     RTC driver
switch.c/h              Officer external interrupt
init_system.c            Startup + RTC edit
setStartEndTime.c        Voting window configuration
user_application.c       Voter authentication/voting
officer.c                Officer menu
Initial_Data_Storing.c   One-time EEPROM initialization utility
rtc_main.c               Standalone RTC test utility


🔗 Overall Data Flow
RFID Card
    ↓
RFID Reader
    ↓
UART
    ↓
LPC2148
    ↓
Check Voting Status
    ↓
Check RTC Time
    ↓
Verify RFID Card
    ↓
Password Authentication
    ↓
Party Selection
    ↓
Update Vote Count
    ↓
EEPROM
    ↓
Vote Confirmation
🧰 Development Tools
Tool	Purpose
Embedded C	Application development
Keil C	Compilation
LPC2148	Main microcontroller
Proteus	Circuit simulation
Flash Magic	Microcontroller programming
EEPROM	Data storage
📌 Key Learning Outcomes

Through this project, the following concepts are demonstrated:

Embedded C programming
LPC2148 ARM7 architecture
GPIO interfacing
UART communication
UART interrupts
RFID interfacing
I2C communication
EEPROM read/write
RTC operation
LCD interfacing
Keypad interfacing
External interrupt
Modular driver development
🚀 Project Purpose

The project demonstrates how multiple embedded peripherals and communication interfaces can be integrated into a single real-time application.

It combines RFID identification, RTC-based time control, password authentication, EEPROM storage, LCD/keypad interaction, and voting management into one embedded system.
