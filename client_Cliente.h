
#ifndef CLIENT_CLIENTE_H_
#define CLIENT_CLIENTE_H_

#include <string>
#include <vector>
#include "client_ProxyServer.h"

class client_Cliente {
private:
	client_ProxyServer servidor;

public:
	client_Cliente();
	int conectar(int argc, char* argv[]);
	void desconectar();
	virtual ~client_Cliente();
private:
	void leerEntrada();
	void parsearCampos(const std::string &cadena);
	int getUnixTimeStamp(const std::string &timeStamp);
	void sendValoresSegunModo(char modo, std::vector<std::string>* tokens);
	void procesarRespuesta();
};

#endif /* CLIENT_CLIENTE_H_ */
