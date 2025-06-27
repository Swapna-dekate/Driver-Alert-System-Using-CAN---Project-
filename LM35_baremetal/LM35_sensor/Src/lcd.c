/*
 * lcd.c
 *
 *  Created on: Mar 31, 2025
 *      Author: admin
 */

#include "lcd.h"

void LcdInit(void) {
	// i2c initialization
	I2CInit();
	// lcd initialization
	DelayMs(20);
	LcdWriteNibble(LCD_CMD, 0x03);   //wake up
	DelayMs(5); 
	LcdWriteNibble(LCD_CMD, 0x03);   //repeat
	DelayMs(1);
	LcdWriteNibble(LCD_CMD, 0x03);   // repeat
	DelayMs(1);
	LcdWriteNibble(LCD_CMD, 0x02);   // set to 4 bit mode
	DelayMs(1);

       	LcdWriteByte(LCD_CMD, LCD_FNSET_2LINE);   // function set 2 line 
	LcdWriteByte(LCD_CMD, LCD_DISP_OFF);       //display off
	LcdWriteByte(LCD_CMD, LCD_CLEAR);           // clear display 
	LcdWriteByte(LCD_CMD, LCD_ENTRYMODE);
	LcdWriteByte(LCD_CMD, LCD_DISP_ON);
}

void LcdWriteNibble(uint8_t rs, uint8_t data) {
	uint8_t rsFlag = rs == LCD_DATA ? BV(LCD_RS_Pos) : 0;   // shift 4 bit data to upper nibble (D4 -D7)
	uint8_t val = (data << 4) | rsFlag | BV(LCD_BL_Pos) | BV(LCD_EN_Pos) ;    // enable trigger the lcd  to read input
	I2CWrite(LCD_SLAVE_ADDR_W, val);
	DelayMs(1);
	val = (data << 4) | rsFlag | BV(LCD_BL_Pos);
	I2CWrite(LCD_SLAVE_ADDR_W, val);
}

void LcdWriteByte(uint8_t rs, uint8_t data) {
	// divide data into two nibbles
	uint8_t high = data >> 4, low = data & 0x0F;  
	LcdWriteNibble(rs, high);
	LcdWriteNibble(rs, low);
	// wait for busy flag -- optional for low speed uc
	DelayMs(1);
}

void LcdPuts(uint8_t line, char *str) {
	// set line start addr
	LcdWriteByte(LCD_CMD, line);   //SET cursor to start of the line 
	// write chars one by one 
	for(int i=0; str[i] != '\0'; i++)
		LcdWriteByte(LCD_DATA, str[i]);   // write each character 
}
