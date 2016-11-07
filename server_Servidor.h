
#ifndef SERVER_SERVIDOR_H_
#define SERVER_SERVIDOR_H_

#include "common_Socket.h"
#include "server_Bondi.h"
#include "server_Paradas.h"
#include "server_ProxyClient.h"
#include "server_Recorridos.h"
#include "server_Thread.h"
#include <string>
#include <vector>
#include <mutex>

class server_Servidor {
private:
	std::mutex m;
	common_Socket skt;
    std::vector<server_Bondi> bondis;
    server_Paradas paradas;
    server_Recorridos recorridos;
    std::vector<server_Thread* > clientesVivos;
    std::vector<server_ProxyClient* > clientes;
    bool encendido;

public:
	server_Servidor();
	int iniciar(int argc, char* argv[]);
	void apagar();
	void procesarDatos(server_ProxyClient *cliente);
	virtual ~server_Servidor();

private:
	void responderCliente(server_ProxyClient *cliente, uint32_t errCode,
			uint32_t codigo_1, uint32_t codigo_2);
	void setNuevoRecorrido(server_ProxyClient *cliente, uint32_t tiempo,
			uint32_t linea);
	void calcularTiempoFaltante(server_ProxyClient *cliente, uint32_t tiempo,
			uint32_t linea, uint32_t parada);
	void calcularLineaMasRapida(server_ProxyClient *cliente, uint32_t partida,
			uint32_t llegada);
	void calcularColectivoRecomendado(server_ProxyClient *cliente, uint32_t tiempo,
			uint32_t partida, uint32_t llegada);
	void recolectarClientes();
	void finalizarCliente(server_ProxyClient* cliente);
};

#endif /* SERVER_SERVIDOR_H_ */
