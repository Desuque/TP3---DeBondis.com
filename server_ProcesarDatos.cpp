
#include "server_ProcesarDatos.h"
#include <iostream>

server_ProcesarDatos::server_ProcesarDatos(server_ProxyClient*
		cliente, server_Servidor* server) :
	cliente(cliente), server(server), estado(false) {
}

void server_ProcesarDatos::run() {
	if (cliente->estaConectado()) {
		std::cerr<<"Cliente conectado."<<std::endl;
	}
	while(cliente->estaConectado()) {
    	server->procesarDatos(cliente);
    }
	std::cerr<<"Cliente desconectado."<<std::endl;
}

void server_ProcesarDatos::finalizar() {
	estado = true;
}

bool server_ProcesarDatos::finalizado() {
	return estado;
}

server_ProxyClient* server_ProcesarDatos::getCliente() {
	return cliente;
}


server_ProcesarDatos::~server_ProcesarDatos() {
}

