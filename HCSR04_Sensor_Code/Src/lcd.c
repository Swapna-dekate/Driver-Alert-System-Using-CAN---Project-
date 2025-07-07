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
	LcdWriteNibble(LCD_CMD, 0x03);
	DelayMs(5);
	LcdWriteNibble(LCD_CMD, 0x03);
	DelayMs(1);
	LcdWriteNibble(LCD_CMD, 0x03);
	DelayMs(1);
	LcdWriteNibble(LCD_CMD, 0x02);
	DelayMs(1);

	LcdWriteByte(LCD_CMD, LCD_FNSET_2LINE);
	LcdWriteByte(LCD_CMD, LCD_DISP_OFF);
	LcdWriteByte(LCD_CMD, LCD_CLEAR);
	LcdWriteByte(LCD_CMD, LCD_ENTRYMODE);
	LcdWriteByte(LCD_CMD, LCD_DISP_ON);
}

void LcdWriteNibble(uint8_t rs, uint8_t data) {
	uint8_t rsFlag = rs == LCD_DATA ? BV(LCD_RS_Pos) : 0;
	uint8_t val = (data << 4) | rsFlag | BV(LCD_BL_Pos) | BV(LCD_EN_Pos) ;
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
	LcdWriteByte(LCD_CMD, line);
	// write chars one by one
	for(int i=0; str[i] != '\0'; i++)
		LcdWriteByte(LCD_DATA, str[i]);
}
