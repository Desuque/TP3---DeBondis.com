#include "server_Bondi.h"
#include <iostream>
#include <vector>

#define INICIO 0

server_Bondi::server_Bondi() : linea(0), horario(0) {
}

server_Bondi::server_Bondi(uint32_t linea, uint32_t horarioSalida
		, std::vector<uint32_t>* recorrido) : linea(linea),
		horario(horarioSalida){
	cargarRecorrido(recorrido);
}

void server_Bondi::cargarRecorrido(std::vector<uint32_t>* recorrido) {
	for(unsigned int i=0; i<recorrido->size(); i++) {
		this->recorrido.push_back(recorrido->at(i));
	}
}

bool server_Bondi::paradaExiste(uint32_t parada) {
	bool encontrado = false;
	unsigned int i = 0;
	while((!encontrado) && (i<recorrido.size())) {
		if (parada == recorrido.at(i)) {
			encontrado = true;
		}
		i++;
	}
	return encontrado;
}

bool server_Bondi::poseeParadasSolicitadas(uint32_t paradaInicio, 
		uint32_t paradaLlegada) {
	bool poseeParadas = false;
	if ((paradaExiste(paradaInicio)) && (paradaExiste(paradaLlegada))) {
		poseeParadas = true;
	}
	return poseeParadas;
}

int server_Bondi::getTiempoDesde(uint32_t partida, uint32_t llegada,
		server_Paradas* paradas) {
	unsigned int i = 0;
	int tiempo = 0;
	bool encontrada = false;
	bool terminado = false;
	while((!terminado) && (i<recorrido.size())) {
		uint32_t una_parada = recorrido.at(i);
		if(una_parada == partida) {
			encontrada = true;
		}
		if (encontrada == true) {
			uint32_t siguiente_parada = recorrido.at(i+1);
			tiempo += paradas->getTiempoEntreParadas(una_parada,
					siguiente_parada);
			if(siguiente_parada == llegada) {
				terminado = true;
			}
		}
		i++;
	}
	return tiempo;
}

int server_Bondi::getTiempoDesdeSalidaA(uint32_t parada,
		server_Paradas* paradas) {
	return getTiempoDesde(recorrido.at(INICIO), parada, paradas);
}

uint32_t server_Bondi::getLinea() {
	return linea;
}

uint32_t server_Bondi::getHorarioSalida() {
	return horario;
}

server_Bondi::~server_Bondi() {
}

