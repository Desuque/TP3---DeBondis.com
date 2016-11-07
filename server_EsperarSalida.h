
#ifndef SERVER_ESPERARSALIDA_H_
#define SERVER_ESPERARSALIDA_H_

#include "server_Servidor.h"
#include "server_Thread.h"

class server_EsperarSalida : public server_Thread {
private:
	server_Servidor* server;
public:
	explicit server_EsperarSalida(server_Servidor* server);
	virtual void run();
	virtual ~server_EsperarSalida();
};

#endif /* SERVER_ESPERARSALIDA_H_ */
