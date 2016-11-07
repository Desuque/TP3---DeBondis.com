
#ifndef COMMON_UTILIDADES_H_
#define COMMON_UTILIDADES_H_

#include "stdint.h"

class common_Utilidades {
public:
	common_Utilidades();
	void intToChar(char *buffer, uint32_t valor);
	uint32_t charToInt(char* buf);
	virtual ~common_Utilidades();
};

#endif /* COMMON_UTILIDADES_H_ */
