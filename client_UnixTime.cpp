
#include "client_UnixTime.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#define POS_ANIO 2
#define POS_MES 1
#define POS_DIA 0
#define POS_HORA 3
#define POS_MIN 4
#define POS_SEG 5

client_UnixTime::client_UnixTime() {
}

int client_UnixTime::calcularTiempo(std::vector<int>* fecha) {
	struct tm date;
	date.tm_sec = fecha->at(POS_SEG);
	date.tm_min = fecha->at(POS_MIN);
	date.tm_hour = fecha->at(POS_HORA);
	date.tm_mday = fecha->at(POS_DIA);
	date.tm_mon = fecha->at(POS_MES) - 1;
	date.tm_year = fecha->at(POS_ANIO) - 1900;

	int tiempo = timegm(&date);
	return tiempo;
}

void client_UnixTime::tokenizar(const std::string &tiempo,
		std::vector<int>* vector) {
	std::stringstream stringStream(tiempo);
	std::string line;
	while(std::getline(stringStream, line))
	{
	    std::size_t prev = 0, pos;
	    while ((pos = line.find_first_of("/-:", prev)) != std::string::npos)
	    {
	        if (pos > prev) {
	            vector->push_back(atoi((line.substr(prev, pos-prev)).c_str()));
	        }
	        prev = pos+1;
	    }
	    if (prev < line.length()) {
	    	vector->push_back(atoi((line.substr(prev, std::string::npos)).c_str()));
	    }
	}
}

int client_UnixTime::convertir(const std::string &tiempo) {
	std::vector<int> fecha;
	tokenizar(tiempo, &fecha);
	return calcularTiempo(&fecha);
}

client_UnixTime::~client_UnixTime() {
}

