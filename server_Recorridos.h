
#ifndef SERVER_RECORRIDOS_H_
#define SERVER_RECORRIDOS_H_

#include <string>
#include <vector>
#include "stdint.h"
#include "server_Bondi.h"
#include "server_Paradas.h"

class server_Recorridos {
private:
	std::vector<std::vector<uint32_t> > recorridos;

public:
	server_Recorridos();
	int cargarRecorridos(char* file);
	void obtenerRecorrido(uint32_t linea, std::vector<uint32_t>* paradas);
	void calcularlineaMasRapida(uint32_t inicio, uint32_t fin,
			server_Paradas* paradas, uint32_t* linea, uint32_t* tiempo);
	virtual ~server_Recorridos();
private:
	void parsearInformacion(const std::string& linea);
};

#endif /* SERVER_RECORRIDOS_H_ */
