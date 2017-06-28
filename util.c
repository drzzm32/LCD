#include "util.h"

void delay(uint16_t time) {
	uint8_t i, j; uint16_t t;
	i = 30; j = 43; t = time;
	do {
		do {
			while (--j);
		} while (--i);
	} while (--t);
}

void delayMicroseconds(uint16_t time) {
	uint8_t i; uint16_t t;
	i = 5; t = time;
	while (--t)
		while (--i);
}