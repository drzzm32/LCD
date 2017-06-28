#include "LCD.h"
#include "font.h"

void setData(uint8_t v) {
	P04 = v & 0x80;
	P05 = v & 0x40;
	P06 = v & 0x20;
	P07 = v & 0x10;
	P10 = v & 0x08;
	P11 = v & 0x04;
	P12 = v & 0x02;
	P13 = v & 0x01;
}

uint8_t getData() {
	uint8_t v = 0, b = 0;
	b = P04;
	v |= b << 7;
	b = P05;
	v |= b << 6;
	b = P06;
	v |= b << 5;
	b = P07;
	v |= b << 4;
	b = P10;
	v |= b << 3;
	b = P11;
	v |= b << 2;
	b = P12;
	v |= b << 1;
	b = P13;
	v |= b;
	return v;
}

//10us@24MHz
void timer2Init() {
	AUXR |= 0x04;
	T2L = 0x10;
	T2H = 0xFF;
	IE2 |= (1 << 2);
	EA = 1;
	AUXR |= 0x10;
}

void waitBusy() {
	LCD_E = 0;
	LCD_RS = 0;
	delayMicroseconds(10);
	LCD_RW = 1;
	delayMicroseconds(10);
	setData(0xFF);
	delayMicroseconds(10);
	LCD_E = 1;
	while (getData() & 0x80);
	LCD_E = 0;
}

void writeByte(uint8_t value, uint8_t rs) {
	waitBusy();
	LCD_RS = rs;
	delayMicroseconds(10);
	LCD_RW = 0;
	delayMicroseconds(10);
	LCD_E = 0;
	setData(value);
	delayMicroseconds(10);
	LCD_E = 1;
	delayMicroseconds(10);
	LCD_E = 0;
}

void writeCmd(uint8_t value) { writeByte(value, 0); }

void writeData(uint8_t value) { writeByte(value, 1); }

void lcdInit() {
	P0M0 = P0M1 = 0;
	P1M0 = P1M1 = 0;
	P5M0 = P5M1 = 0;
	
	timer2Init();
	
	LCD_CONTRAST_SET = 0;
	LCD_BACKLIGHT_SET = 0;
	LCD_CONTRAST_VALUE = 0;
	LCD_BACKLIGHT_VALUE = 0;
	
	writeCmd(0x38);
	writeCmd(0x08);
	writeCmd(0x06);
	writeCmd(0x01);
	writeCmd(0x0C);
	writeCmd(0x80);
}

void lcdSetBright(uint8_t value) {
	if (value <= LCD_BACKLIGHT_MAX)
		LCD_BACKLIGHT_SET = LCD_BACKLIGHT_MAX - value;
	else
		LCD_BACKLIGHT_SET = 0;
}

void lcdSetContrast(uint8_t value) {
	if (value <= LCD_CONTRAST_MAX)
		LCD_CONTRAST_SET = LCD_CONTRAST_MAX - value;
	else
		LCD_CONTRAST_SET = 0;
}

void lcdDraw(uint8_t x, uint8_t y, char c) {
	writeCmd(0x80 + ((y > 0) ? 0x40 : 0) + x);
	writeData(c);
}

void lcdCusDraw(uint8_t x, uint8_t y, char c) {
	uint8_t i;
	writeCmd(LCD_CUSCMD_1);
	for (i = 1; i < 6; i++) writeData(font[c - 32][i]);
	writeCmd(0x80 + ((y > 0) ? 0x40 : 0) + x);
	writeData(LCD_CUSADD_1);
}

void lcdPrint(uint8_t x, uint8_t y, char* str) {
	uint8_t i; uint8_t tx = x, ty = y;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n') {
			tx = x;
			ty = 1 - ty;
		} else {
			lcdDraw(tx + i, ty, str[i]);
		}
	}
}

void lcdCusPrint(uint8_t x, uint8_t y, char* str) {
	uint8_t i; uint8_t tx = x, ty = y;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n') {
			tx = x;
			ty = 1 - ty;
		} else {
			lcdCusDraw(tx + i, ty, str[i]);
		}
	}
}

void lcdClear() {
	writeCmd(0x01);
	delay(1);
}

void timer2Interrupt() interrupt 12 {
	T2L = 0xD4;
	T2H = 0xFE;
	
	if (LCD_CONTRAST_VALUE <= LCD_CONTRAST_SET) LCD_CONTRAST = 1;
	else if (LCD_CONTRAST_VALUE <= LCD_CONTRAST_MAX) LCD_CONTRAST = 0;
	else LCD_CONTRAST_VALUE = 0;
	LCD_CONTRAST_VALUE += 1;
	
	if (LCD_BACKLIGHT_VALUE <= LCD_BACKLIGHT_SET) LCD_BACKLIGHT = 1;
	else if (LCD_BACKLIGHT_VALUE <= LCD_BACKLIGHT_MAX) LCD_BACKLIGHT = 0;
	else LCD_BACKLIGHT_VALUE = 0;
	LCD_BACKLIGHT_VALUE += 1;
}