
#ifndef SERVER_PROCESARDATOS_H_
#define SERVER_PROCESARDATOS_H_

#include "common_Socket.h"
#include "server_ProxyClient.h"
#include "server_Servidor.h"
#include "server_Thread.h"

class server_ProcesarDatos : public server_Thread {
private:
	server_ProxyClient* cliente;
	server_Servidor* server;
	bool estado;

public:
	server_ProcesarDatos(server_ProxyClient* cliente, server_Servidor* server);
	void run();
	bool finalizado();
	void finalizar();
	server_ProxyClient* getCliente();
	virtual ~server_ProcesarDatos();
};

#endif /* SERVER_PROCESARDATOS_H_ */
