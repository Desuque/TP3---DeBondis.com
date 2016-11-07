
#ifndef CLIENT_PROXYSERVER_H_
#define CLIENT_PROXYSERVER_H_

#include "common_Socket.h"
#include "common_Utilidades.h"
#include "stdint.h"

class client_ProxyServer {
private:
	common_Socket socket;
	common_Utilidades utilidades;
public:
	client_ProxyServer();
	int conectar(char* ip, int puerto);
	void empaquetar(char modo, uint32_t tiempoUnix, uint32_t campo_1,
			uint32_t campo2);
	void desconectar();
	int enviar(char* mensaje, int tam_buf);
	int recibir(char* buffer, int tam_buf);
	virtual ~client_ProxyServer();
};

#endif /* CLIENT_PROXYSERVER_H_ */
