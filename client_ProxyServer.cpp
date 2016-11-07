
#include "client_ProxyServer.h"
#include "common_Socket.h"
#include <arpa/inet.h>
#include "stdint.h"
#include <iostream>

#define VACIO 0
#define UN_BYTE 1
#define CUATRO_BYTES 4
#define SUCCESS 0
#define ERROR_INTERNO -1

client_ProxyServer::client_ProxyServer() {
}

int client_ProxyServer::conectar(char* ip, int puerto) {
	return socket.conectar(ip, puerto);
}

void client_ProxyServer::desconectar() {
	socket.apagar();
}

void client_ProxyServer::empaquetar(char modo, uint32_t tiempoUnix,
		uint32_t campo_1, uint32_t campo_2) {
	char c_val[CUATRO_BYTES] = {0};
	enviar(&modo, UN_BYTE);
	if (tiempoUnix != VACIO) {
		tiempoUnix = htonl(tiempoUnix);
		utilidades.intToChar(c_val, tiempoUnix);
		enviar(c_val, CUATRO_BYTES);
	}
	campo_1 = htonl(campo_1);
	utilidades.intToChar(c_val, campo_1);
	enviar(c_val, CUATRO_BYTES);

	if (modo != 'A') {
		campo_2 = htonl(campo_2);
		utilidades.intToChar(c_val, campo_2);
		enviar(c_val, CUATRO_BYTES);
	}
}

int client_ProxyServer::enviar(char* mensaje, int tam_buf) {
	return socket.enviar(mensaje, tam_buf);
}

int client_ProxyServer::recibir(char* buffer, int tam_buf) {
	if (socket.recibir(buffer, tam_buf) != ERROR_INTERNO) {
		if (tam_buf == CUATRO_BYTES) {
			uint32_t codigo = ntohl(utilidades.charToInt(buffer));
			return codigo;
		} else {
			return SUCCESS;
		}
	} else {
		return ERROR_INTERNO;
	}
}

client_ProxyServer::~client_ProxyServer() {
}

