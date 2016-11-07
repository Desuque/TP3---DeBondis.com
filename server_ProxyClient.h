
#ifndef SERVER_PROXYCLIENT_H_
#define SERVER_PROXYCLIENT_H_

#include "common_Socket.h"
#include "common_Utilidades.h"
#include "stdint.h"

class server_ProxyClient {
private:
	common_Socket *socket; //Cliente
	common_Utilidades utilidades;
	bool conectado;
public:
	server_ProxyClient();
	int conectar(common_Socket* skt);
	uint32_t recibir(char* buffer, int tam_buf);
	void enviar(char* buffer, int tam_buffer);
	void enviar(uint32_t buffer, int tam_buffer);
	bool estaConectado();
	virtual ~server_ProxyClient();
private:
	void desconectar();
};

#endif /* PROXYSERVER_H_ */
