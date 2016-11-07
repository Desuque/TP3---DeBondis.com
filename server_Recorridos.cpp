
#include "server_Recorridos.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

#define LINEA 0
#define PARADA_INICIO 1
#define ERROR -1
#define SUCCESS 0

server_Recorridos::server_Recorridos() {
}

void server_Recorridos::parsearInformacion(const std::string& linea) {
	std::string buf;
	std::stringstream ss(linea);
	std::vector<uint32_t> tokens;
	while (ss >> buf) {
		tokens.push_back(atoi(buf.c_str()));
	}
	this->recorridos.push_back(tokens);
}

int server_Recorridos::cargarRecorridos(char* file) {
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
	return SUCCESS;
}

void server_Recorridos::calcularlineaMasRapida(uint32_t inicio, uint32_t fin,
		server_Paradas* paradas, uint32_t* linea, uint32_t* tiempo) {
	unsigned int i = 0;
	while(i<recorridos.size()) {
		std::vector<uint32_t> recorrido = recorridos.at(i);
		bool encontrado = false;
		unsigned int j = PARADA_INICIO;
		std::vector<int> recorridoParcial;
		while((!encontrado) && (j<recorrido.size())) {
			if(recorrido.at(j) == inicio) {
				//Si encuentro la parada de inicio, creo un vector
				//de recorridos parciales
				recorridoParcial.push_back(inicio);
				j++;
			}
			//Si ya se encontro la parada de inicio, se carga hasta encontrar
			//la parada de llegada

			if(recorridoParcial.size() > 0) {
				if(recorrido.at(j) == fin) {
					encontrado = true;
				}
				recorridoParcial.push_back(recorrido.at(j));
			}
			j++;
		}
		//Si se activo el flag, entonces tanto el inicio como el fin
		//se encontraban en el recorrido, procedo a calcular el tiempo
		if (encontrado == true) {
			uint32_t tiempoTotal = 0;
			for(unsigned int k=0; k<recorridoParcial.size()-1; k++) {
				tiempoTotal+= paradas->getTiempoEntreParadas
						(recorridoParcial.at(k), recorridoParcial.at(k+1));
			}
			if (*tiempo == 0) {
				*tiempo = tiempoTotal;
				*linea = recorrido.at(LINEA);
			} else if (tiempoTotal<*tiempo) {
				*tiempo = tiempoTotal;
				*linea = recorrido.at(LINEA);
			}
		}
		i++;
	}
}

void server_Recorridos::obtenerRecorrido(uint32_t linea,
		std::vector<uint32_t>* paradas) {
	bool encontrado = false;
	unsigned int i = 0;
	while((!encontrado) && (i<recorridos.size())) {
		std::vector<uint32_t> recorrido = recorridos.at(i);
		if(recorrido.at(LINEA) == linea) {
			encontrado = true;
			for(unsigned int j=PARADA_INICIO; j<recorrido.size(); j++) {
				paradas->push_back(recorrido.at(j));
			}
		}
		i++;
	}
}

server_Recorridos::~server_Recorridos() {
}
