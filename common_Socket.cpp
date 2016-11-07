#include "common_Socket.h"
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <string>

#define CLIENTES 20
#define ERR_COD 1

common_Socket::common_Socket() {
	skt = socket(AF_INET, SOCK_STREAM, 0);
}

int common_Socket::enlazarYEscuchar(unsigned short port) {
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	memset(address.sin_zero, 0, sizeof(address.sin_zero));
	if (bind(this->skt, (struct sockaddr*) &address,
			sizeof(struct sockaddr_in)) == -1) {
		return ERR_COD;
	}
	if (listen(this->skt, CLIENTES) == -1) {
		return ERR_COD;
	}
	return 0;
}

int common_Socket::conectar(char* host_name, int port) {
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(host_name);
	memset(address.sin_zero, 0, sizeof(address.sin_zero));
	int connected = connect(skt, (struct sockaddr*)&address,
			sizeof(struct sockaddr_in));
	if (connected != 0) {
		return connected;
	}
	return 0;
}

void common_Socket::setSocket(int skt) {
	apagar();
	close(this->skt); //Cierro el socket que habia creado al hacer new
	this->skt = skt;
}

common_Socket* common_Socket::aceptar() {
	int sock = accept(this->skt, NULL, NULL);
	if (sock < 0) {
		return NULL;
	} else {
		common_Socket* cliente = new common_Socket();
		cliente->setSocket(sock);
		return cliente;
	}
}

int common_Socket::enviar(char* buffer, int length) {
	int sent = 0;
	int s = 0;
	bool socketValido = true;
	while (sent < length && socketValido) {
		s = send(this->skt, &buffer[sent], length-sent, MSG_NOSIGNAL);
		if (s == 0) {
			socketValido = false;
		} else if (s < 0) {
			socketValido = false;
		} else {
			sent += s;
		}
	}
	if (socketValido) {
		return sent;
	} else {
		return -1;
	}
}

int common_Socket::recibir(char* buffer, int length) {
	int received = 0;
	int s = 0;
	bool socketValido = true;
	while (received < length && socketValido) {
		s = recv(this->skt, &buffer[received], length-received,
				MSG_NOSIGNAL);
		if (s == 0) { // Socket cerrado
			socketValido = false;
		} else if (s < 0) { // Error
			socketValido = false;
		} else {
			received = received + s;
		}
	}
	if (socketValido) {
		return received;
	} else {
		return -1;
	}
}

void common_Socket::apagar() {
	shutdown(skt, SHUT_RDWR);
}

common_Socket::~common_Socket() {
	close(skt);
}
