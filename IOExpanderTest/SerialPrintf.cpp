#include <Arduino.h>
#include <stdarg.h>

#include "SerialPrintf.h"

static void serialPrintfInternal(const char *str, va_list &argv) {
	const size_t kBufferSize = 256;
	int flag = 0;
	char temp[kBufferSize + 1];

	int j = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == '%') {
			temp[j] = '\0';
			Serial.print(temp);
			j = 0;
			temp[0] = '\0';

			switch (str[++i]) {
				case 'd': Serial.print(va_arg(argv, int)); break;
				// case 'h': Serial.print(va_arg(argv, short)); break;
				case 'z': Serial.print(va_arg(argv, size_t)); break;
				case 'l': Serial.print(va_arg(argv, long)); break;
				case 'f': Serial.print(va_arg(argv, double)); break;
				case 'c': Serial.print((char)va_arg(argv, int)); break;
				case 's': Serial.print(va_arg(argv, char *)); break;
				default: break;
			}
		} else {
			temp[j] = str[i];
			j = (j + 1) % kBufferSize;

			if (j == 0) {
				temp[kBufferSize] = '\0';
				Serial.print(temp);
				temp[0] = '\0';
			}
		}
	}

	if (j != 0) {
		temp[j] = '\0';
		Serial.print(temp);
	}
}

void serialPrintf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	serialPrintfInternal(format, args);
	va_end(args);
}

void serialPrintfln(const char *format, ...) {
	va_list args;
	va_start(args, format);
	serialPrintfInternal(format, args);
	va_end(args);

	Serial.println();
}
