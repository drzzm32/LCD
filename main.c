#include "LCD.h"

void main() {		
	lcdInit();
	lcdSetContrast(12);
	lcdSetBright(2);
	
	lcdPrint(0, 0, "Hello World!");
	lcdPrint(0, 1, "Hello Gensokyo!");
	
	while (1);
}
