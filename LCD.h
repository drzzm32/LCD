#include "util.h"

#ifndef __LCD_H_
#define __LCD_H_

#define LCD_CONTRAST	P17
#define LCD_RS			P16
#define LCD_RW			P15
#define LCD_E			P14
#define LCD_BACKLIGHT	P00

#define LCD_CUSCMD_1	0x40
#define LCD_CUSCMD_2	0x48
#define LCD_CUSCMD_3	0x50
#define LCD_CUSCMD_4	0x58
#define LCD_CUSCMD_5	0x60
#define LCD_CUSCMD_6	0x68
#define LCD_CUSCMD_7	0x70
#define LCD_CUSCMD_8	0x78

#define LCD_CUSADD_1	0x01
#define LCD_CUSADD_2	0x02
#define LCD_CUSADD_3	0x03
#define LCD_CUSADD_4	0x04
#define LCD_CUSADD_5	0x05
#define LCD_CUSADD_6	0x06
#define LCD_CUSADD_7	0x07
#define LCD_CUSADD_8	0x08

#define LCD_CONTRAST_MAX 16
#define LCD_BACKLIGHT_MAX 16
static uint8_t LCD_CONTRAST_SET;
static uint8_t LCD_BACKLIGHT_SET;
static uint8_t LCD_CONTRAST_VALUE;
static uint8_t LCD_BACKLIGHT_VALUE;

void setData(uint8_t v);
uint8_t getData();

void timer2Init();

void lcdInit();
void lcdSetBright(uint8_t value);
void lcdSetContrast(uint8_t value);
void lcdDraw(uint8_t x, uint8_t y, char c);
void lcdCusDraw(uint8_t x, uint8_t y, char c);
void lcdPrint(uint8_t x, uint8_t y, char* str);
void lcdCusPrint(uint8_t x, uint8_t y, char* str);
void lcdClear();

#endif