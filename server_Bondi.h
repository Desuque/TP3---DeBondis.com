
#ifndef SERVER_BONDI_H_
#define SERVER_BONDI_H_

#include "server_Paradas.h"
#include "stdint.h"
#include <vector>

class server_Bondi {
private:
	uint32_t linea;
	uint32_t horario;
	std::vector<uint32_t> recorrido;
public:
	server_Bondi();
	server_Bondi(uint32_t linea, uint32_t horarioSalida,
			std::vector<uint32_t>* recorrido);
	uint32_t getLinea();
	uint32_t getHorarioSalida();
	int getTiempoDesdeSalidaA(uint32_t parada, server_Paradas* paradas);
	int getTiempoDesde(uint32_t partida, uint32_t llegada,
			server_Paradas* paradas);
	bool poseeParadasSolicitadas(uint32_t paradaInicio,
			uint32_t paradaLlegada);
	bool paradaExiste(uint32_t parada);
	virtual ~server_Bondi();
private:
	void cargarRecorrido(std::vector<uint32_t>* recorrido);
};

#endif /* SERVER_BONDI_H_ */
