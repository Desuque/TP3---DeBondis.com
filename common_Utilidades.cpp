#include "common_Utilidades.h"
#include <cstring>
#include "stdint.h"

common_Utilidades::common_Utilidades() {
}

void common_Utilidades::intToChar(char *buffer, uint32_t valor) {
	memcpy(buffer, &valor, sizeof(uint32_t));
}

uint32_t common_Utilidades::charToInt(char* buf) {
	uint32_t value;
	memcpy(&value, buf, sizeof(uint32_t));
	return value;
}

common_Utilidades::~common_Utilidades() {
}

