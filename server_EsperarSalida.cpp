
#include <string>
#include <iostream>
#include "server_EsperarSalida.h"

server_EsperarSalida::server_EsperarSalida(server_Servidor*
		server) : server(server) {
}

void server_EsperarSalida::run() {
	std::string quit;
	while(quit != "q") {
		std::cin >> quit;
	}
	server->apagar();
}

server_EsperarSalida::~server_EsperarSalida() {
}

