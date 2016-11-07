
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "client_Cliente.h"
#include "client_ProxyServer.h"
#include "client_UnixTime.h"

#define VACIO 0
#define INICIO 1
#define ESPACIO 2
#define POS_TIEMPO_UNIX 0
#define POS_MODO 1
#define POS_LINEA 2
#define POS_PARADA 3
#define POS_PARADA_PARTIDA 2
#define POS_PARADA_LLEGADA 3
#define SEGS_EN_MIN 60
#define UN_BYTE 1
#define CUATRO_BYTES 4
#define ERROR_INTERNO -1
#define SUCCESS 0
#define MIN_PARAM 2
#define IP 1
#define PORT 2

//Defino los retornos de error de cada comando
#define SUCCESS_A 0x00
#define SUCCESS_F 0x02
#define SUCCESS_L 0x03
#define SUCCESS_R 0x04
#define ERROR 0xff

client_Cliente::client_Cliente() {
}

int client_Cliente::conectar(int argc, char* argv[]) {
	if((argc>MIN_PARAM)) {
		char* ip = argv[IP];
		int puerto = atoi(argv[PORT]);
		servidor.conectar(ip, puerto);
		leerEntrada(); //Lee std::cin
	} else {
		return ERROR_INTERNO;
	}
	return SUCCESS;
}

void client_Cliente::desconectar() {
	servidor.desconectar();
}

int client_Cliente::getUnixTimeStamp(const std::string &timeStamp) {
	//Elimino los [...] que trae por defecto el timeStamp
	std::string time = timeStamp.substr(INICIO, timeStamp.size()-ESPACIO);
	client_UnixTime unixTime;
	return unixTime.convertir(time);
}

void client_Cliente::procesarRespuesta() {
	char error;
	char codigo_1[CUATRO_BYTES] = {0};
	char codigo_2[CUATRO_BYTES] = {0};
	if (servidor.recibir(&error, UN_BYTE) != ERROR_INTERNO) {
		uint32_t errorCode = (unsigned char)error;
		if (errorCode == SUCCESS_A) {
			uint32_t linea = servidor.recibir(codigo_1, CUATRO_BYTES);
			std::cout<<"Un colectivo de la línea "<<linea<<" ha sido agregado."
					<<std::endl;
		} else if (errorCode == SUCCESS_F) {
			uint32_t segundos = servidor.recibir(codigo_1, CUATRO_BYTES);
			int minutos = segundos / SEGS_EN_MIN;
			std::cout<<"Faltan "<<minutos<<" minutos para que llegue el siguiente "
					"colectivo."<<std::endl;
		} else if (errorCode == SUCCESS_L) {
			uint32_t linea = servidor.recibir(codigo_1, CUATRO_BYTES);
			uint32_t segundos = servidor.recibir(codigo_2, CUATRO_BYTES);
			int minutos = segundos / SEGS_EN_MIN;
			segundos %= SEGS_EN_MIN;
			std::cout<<"La línea con el recorrido más rápido es la "<<linea<<
			", que tarda "<<minutos<<" minutos y "<<segundos<<
			" segundos en llegar a destino."<<std::endl;
		} else if (errorCode == SUCCESS_R) {
			uint32_t linea = servidor.recibir(codigo_1, CUATRO_BYTES);
			uint32_t segundos = servidor.recibir(codigo_2, CUATRO_BYTES);
			int minutos = segundos / SEGS_EN_MIN;
			segundos %= SEGS_EN_MIN;
			std::cout<<"El colectivo de la línea "<<linea<<" tardará "<<minutos<<
					" minutos y "<<segundos<<" segundos en llegar "
					"a destino."<<std::endl;
		} else if (errorCode == ERROR) {
			std::cout<<"Error."<<std::endl;
		}
	}
}

void client_Cliente::sendValoresSegunModo(char modo,
		std::vector<std::string>* tokens) {
	uint32_t linea;
	uint32_t parada;
	uint32_t partida;
	uint32_t llegada;
	uint32_t tiempoUnix = getUnixTimeStamp(tokens->at(POS_TIEMPO_UNIX));

	switch(modo) {
	case 'A':
		// Uso: A, tiempo, linea
		linea = atoi((tokens->at(POS_LINEA)).c_str());
		servidor.empaquetar(modo, tiempoUnix, linea, VACIO);
		break;
	case 'F':
		//Uso: F, tiempo, linea, parada
		linea = atoi((tokens->at(POS_LINEA)).c_str());
		parada = atoi((tokens->at(POS_PARADA).c_str()));
		servidor.empaquetar(modo, tiempoUnix, linea, parada);
		break;
	case 'L':
		// Uso: L, tiempo, paradaPartida, paradaLlegada
		partida = atoi((tokens->at(POS_PARADA_PARTIDA).c_str()));
		llegada = atoi((tokens->at(POS_PARADA_LLEGADA).c_str()));
		servidor.empaquetar(modo, VACIO, partida, llegada);
		break;
	case 'R':
		// Uso: R, tiempo, paradaPartida, paradaLlegada
		partida = atoi((tokens->at(POS_PARADA_PARTIDA).c_str()));
		llegada = atoi((tokens->at(POS_PARADA_LLEGADA).c_str()));
		servidor.empaquetar(modo, tiempoUnix, partida, llegada);
		break;
	}
	procesarRespuesta();
}

void client_Cliente::parsearCampos(const std::string &cadena) {
	char opcion;
	std::string buf;
	std::stringstream ss(cadena);
	std::vector<std::string> tokens;
	while (ss >> buf) {
		tokens.push_back(buf);
	}
	if(tokens.size() != 0) {
		std::string pos_opcion = tokens.at(POS_MODO);
		opcion = pos_opcion[0];
		sendValoresSegunModo(opcion, &tokens);
	}
}

void client_Cliente::leerEntrada() {
	std::string str;
	while(!std::cin.eof()) {
		std::getline(std::cin, str);
		parsearCampos(str);
	}
	//Envio un caracter para informarle al server
	//que el cliente fue procesado de forma completa
	char c_val = '\n';
	servidor.enviar(&c_val, UN_BYTE);
}

client_Cliente::~client_Cliente() {
}

