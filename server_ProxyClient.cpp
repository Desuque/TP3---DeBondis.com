
#include <iostream>
#include <arpa/inet.h>
#include "server_ProxyClient.h"
#include "server_Servidor.h"

#define UN_BYTE 1
#define CUATRO_BYTES 4
#define ERROR -1

server_ProxyClient::server_ProxyClient() : socket(NULL) {
	conectado = true;
}

int server_ProxyClient::conectar(common_Socket* skt) {
	socket = skt->aceptar();
	if (socket == NULL) {
		return ERROR;
	}
	return 0;
}

uint32_t server_ProxyClient::recibir(char* buffer, int tam_buf) {
	socket->recibir(buffer, tam_buf);
	if (tam_buf == CUATRO_BYTES) {
	 	uint32_t codigo = ntohl(utilidades.charToInt(buffer));
		return codigo;
	}
	if(*buffer == '\n') {
		//Activo el flag que indica que el cliente finalizo
		//el envio de datos		
		desconectar();
	}
	return 0;
}

void server_ProxyClient::desconectar() {
	socket->apagar();
	conectado = false;
}

bool server_ProxyClient::estaConectado() {
	return conectado;
}

void server_ProxyClient::enviar(char* buffer, int tam_buffer) {
	socket->enviar(buffer, tam_buffer);
}

void server_ProxyClient::enviar(uint32_t buffer, int tam_buffer) {
	char c_val[CUATRO_BYTES] = {0};
	buffer = htonl(buffer);
	utilidades.intToChar(c_val, buffer);
	socket->enviar(c_val, tam_buffer);
}

server_ProxyClient::~server_ProxyClient() {
	if(socket != NULL) {
		delete socket;
	}
}
