
#include "server_Paradas.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

#define ERROR -1
#define PARTIDA 0
#define LLEGADA 1
#define TIEMPO 2

server_Paradas::server_Paradas() {
}

void server_Paradas::parsearInformacion(const std::string& linea) {
	std::string buf;
	std::stringstream ss(linea);
	std::vector<int> tokens;
	while (ss >> buf) {
		tokens.push_back(atoi(buf.c_str()));
	}
	this->paradas.push_back(tokens);
}

int server_Paradas::getTiempoEntreParadas(int inicio, int fin) {
	bool encontrado = false;
	unsigned int i = 0;
	int tiempo;
	while((!encontrado) && (i<paradas.size())) {
		std::vector<int> inicio_fin_tiempo = paradas.at(i);
		int partida = inicio_fin_tiempo.at(PARTIDA);
		int llegada = inicio_fin_tiempo.at(LLEGADA);
		if((partida == inicio) && (llegada == fin)) {
			tiempo = inicio_fin_tiempo.at(TIEMPO);
			encontrado = true;
		}
		i++;
	}
	if (encontrado == false) {
		tiempo = ERROR;
	}
	return tiempo;
}

int server_Paradas::cargarParadas(char* file) {
	std::string linea;
	std::ifstream archivo(file);
	if(archivo.is_open()) {
		while(getline(archivo,linea)) {
			parsearInformacion(linea);
	    }
		archivo.close();
	} else {
		return ERROR;
	}
	return 0;
}

server_Paradas::~server_Paradas() {
}
