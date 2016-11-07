
#ifndef COMMON_SOCKET_H_
#define COMMON_SOCKET_H_

#include <string>

class common_Socket {
private:
	int skt;

public:
	common_Socket();
	int enlazarYEscuchar(unsigned short port);
	int conectar(char* host_name, int port);
	common_Socket* aceptar();
	int enviar(char *buffer, int length);
	int recibir(char *buffer, int length);
	void apagar();
	virtual ~common_Socket();
private:
	void setSocket(int skt);
};

#endif /* COMMON_SOCKET_H_ */
