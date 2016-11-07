
#include <iostream>
#include <vector>
#include "server_EsperarSalida.h"
#include "server_ProcesarDatos.h"
#include "server_Thread.h"
#include "server_Servidor.h"

#define PORT 1
#define FILE_PARADAS 2
#define FILE_RECORRIDOS 3
#define CANT_ARGS 4
#define VACIO 0
#define UN_BYTE 1
#define CUATRO_BYTES 4
//Defino un error especial para no confundir con el mensaje de
//error utilizado para responderle al cliente
#define ERROR_INTERNO -1

//Defino los retornos de error de cada comando
#define SUCCESS_A 0x00
#define SUCCESS_F 0x02
#define SUCCESS_L 0x03
#define SUCCESS_R 0x04
#define ERROR 0xff

server_Servidor::server_Servidor() : encendido(true) {
}

int server_Servidor::iniciar(int argc, char* argv[]) {
	if(argc == CANT_ARGS) {
		paradas.cargarParadas(argv[FILE_PARADAS]);
		recorridos.cargarRecorridos(argv[FILE_RECORRIDOS]);
		skt.enlazarYEscuchar(atoi(argv[PORT]));
	}

	//Lanzo el hilo que espera la "q" de salida
	server_EsperarSalida salida(this);
	salida.start();

	//Lanzo los hilos segun la cantidad de clientes que se conectan
	while(encendido) {
      	server_ProxyClient* cliente = new server_ProxyClient();
      	clientes.push_back(cliente);
    	if (cliente->conectar(&skt) != ERROR_INTERNO) {
	    	server_ProcesarDatos *procesarCliente =
	    		new server_ProcesarDatos(cliente, this);
	      	clientesVivos.push_back(procesarCliente);
	    	procesarCliente->start();
    	}
    	//Busco clientes que hayan finalizado para joinearlos
    	//recolectarClientes();
    }
	salida.join();
	return 0;
}

void server_Servidor::finalizarCliente(server_ProxyClient* cliente) {
	bool encontrado = false;
	unsigned int i = 0;
	while((!encontrado) && (i<clientesVivos.size())) {
		server_ProcesarDatos* clienteVivo = 
			(server_ProcesarDatos*)(clientesVivos.at(i));
		if ((clienteVivo->getCliente() == cliente) && 
				(!clienteVivo->finalizado())) {
			clientesVivos.at(i)->join();
			clienteVivo->finalizar();
			encontrado = true;
		}
		i++;
	}
}

void server_Servidor::recolectarClientes() {
	m.lock();
	//Todos aquellos clientes que ya se desconectaron y/o finalizaron se joinean
	for(unsigned int i=0; i<clientes.size(); i++) {
		server_ProxyClient* cliente = clientes.at(i);
		if (cliente->estaConectado() == false) {
			finalizarCliente(cliente);
		}
	}
	m.unlock();
}

void server_Servidor::apagar() {
	skt.apagar();
	encendido = false;
	for(unsigned int i=0; i<clientesVivos.size(); i++) {
		server_ProcesarDatos* clienteVivo = 
			(server_ProcesarDatos*)(clientesVivos.at(i));
		//Los clientes que aun no se recolectaron durante la ejecucion son joineados
		if (!clienteVivo->finalizado()) {
			clientesVivos.at(i)->join();
		}
	}
}

void server_Servidor::responderCliente(server_ProxyClient *cliente,
		uint32_t errorCode, uint32_t codigo_1, uint32_t codigo_2) {
	char code = static_cast<char>(errorCode);
	cliente->enviar(&code, UN_BYTE);
	if ((errorCode == SUCCESS_A) || (errorCode == SUCCESS_F)) {
		cliente->enviar(codigo_1, CUATRO_BYTES);
	} else if ((errorCode == SUCCESS_L) || (errorCode == SUCCESS_R)) {
		cliente->enviar(codigo_1, CUATRO_BYTES);
		cliente->enviar(codigo_2, CUATRO_BYTES);
	}
}

void server_Servidor::setNuevoRecorrido(server_ProxyClient
		*cliente, uint32_t tiempo, uint32_t linea) {
	m.lock();
	int errorCode;
	std::vector<uint32_t> recorrido;
	recorridos.obtenerRecorrido(linea, &recorrido);
	if (recorrido.size() != 0) {
		server_Bondi bondi(linea, tiempo, &recorrido);
		bondis.push_back(bondi);
		errorCode = SUCCESS_A;
	} else {
		errorCode = ERROR;
	}
	responderCliente(cliente, errorCode, linea, VACIO);
	m.unlock();
}

void server_Servidor::calcularTiempoFaltante(server_ProxyClient
		*cliente, uint32_t tiempoPasajero, uint32_t linea,
		uint32_t parada) {
	int errorCode = ERROR;
	uint32_t tiempoRestante = 0;
	bool tiempoSeteado = false;
	//Recorro todos los colectivos cargados ya que puede
	//haber mas de un colectivo de la misma linea en circulacion
	for(unsigned int i=0; i<bondis.size(); i++) {
		server_Bondi bondi = bondis.at(i);
		if ((bondi.getLinea() == linea) && (bondi.paradaExiste(parada))) {
			int tiempoRecorrido = bondi.getTiempoDesdeSalidaA(
				parada, &paradas);
			int horarioSalida = bondi.getHorarioSalida();
			int tiempo = (tiempoRecorrido+horarioSalida) - tiempoPasajero;
			//Si aun no se cargo ningun tiempo, seteo el primer
			//resultado valido encontrado.
			//Es valido aquel que el tiempo es positivo, quiere decir
			//que el colectivo aun no paso por la parada del usuario
			if (tiempo>=0) {
				uint32_t u_tiempo = tiempo;
				if (tiempoSeteado == false) {
					tiempoRestante = u_tiempo;
					errorCode = SUCCESS_F;
					tiempoSeteado = true;
				}
				if (tiempoRestante > u_tiempo) {
					tiempoRestante = u_tiempo;
				}
			}
		}
	}
	responderCliente(cliente, errorCode, tiempoRestante, VACIO);
}

void server_Servidor::calcularLineaMasRapida(server_ProxyClient
		*cliente, uint32_t partida, uint32_t llegada) {
	int errorCode;
	uint32_t linea = 0;
	uint32_t segundos = 0;
	recorridos.calcularlineaMasRapida(partida, llegada,
			&paradas, &linea, &segundos);
	if (linea != 0) {
		errorCode = SUCCESS_L;
	} else {
		errorCode = ERROR;
	}
	responderCliente(cliente, errorCode, linea, segundos);
}

void server_Servidor::calcularColectivoRecomendado(server_ProxyClient
		*cliente, uint32_t horarioPasajero, uint32_t inicioPasajero,
		uint32_t llegadaPasajero) {
	int errorCode;
	uint32_t tiempoTotal = 0;
	uint32_t linea = 0;
	bool tiempoSeteado = false;
	for(unsigned int i=0; i<bondis.size(); i++) {
		server_Bondi bondi = bondis.at(i);
		//Verifico que el colectivo pase por las paradas solicitdas
		if (bondi.poseeParadasSolicitadas(inicioPasajero, llegadaPasajero)) {
			uint32_t horarioLlegadaAInicio = bondi.getTiempoDesdeSalidaA(
				inicioPasajero, &paradas) + bondi.getHorarioSalida();
			//Si aun el colectivo no llego a la parada en la que esta el
			//usuario es un colectivo valido para recomendar
			if (horarioPasajero <= horarioLlegadaAInicio) {
				uint32_t tiempoViaje = bondi.getTiempoDesde(
					inicioPasajero, llegadaPasajero, &paradas);
				uint32_t tiempoEspera = horarioLlegadaAInicio - horarioPasajero;
				uint32_t t_total = tiempoViaje + tiempoEspera;
				//Si aun no se cargo ningun tiempo, seteo el primer
				//resultado encontrado
				if (tiempoSeteado == false) {
					tiempoTotal = t_total;
					linea = bondi.getLinea();
					tiempoSeteado = true;
				}
				if (tiempoTotal > t_total) {
					tiempoTotal = t_total;
					linea = bondi.getLinea();
				}
			}
		}
	}
	if (linea != 0) {
		errorCode = SUCCESS_R;
	} else {
		errorCode = ERROR;
	}
	responderCliente(cliente, errorCode, linea, tiempoTotal);
}

void server_Servidor::procesarDatos(server_ProxyClient *cliente) {
	char modo;
	char buffer[CUATRO_BYTES] = {0};
	int32_t tiempoUnix;
	int32_t campo_1;
	int32_t campo_2;
	cliente->recibir(&modo, UN_BYTE);
	if(modo != '\n') {
		std::cerr<<"Comando "<<modo<<" recibido."<<std::endl;
		switch(modo) {
			case 'A':
				// Uso: A, tiempo, linea
				tiempoUnix = cliente->recibir(buffer, CUATRO_BYTES);
				campo_1 = cliente->recibir(buffer, CUATRO_BYTES);
				setNuevoRecorrido(cliente, tiempoUnix, campo_1);
				break;
			case 'F':
				//Uso: F, tiempo, linea, parada
				tiempoUnix = cliente->recibir(buffer, CUATRO_BYTES);
				campo_1 = cliente->recibir(buffer, CUATRO_BYTES);
				campo_2 = cliente->recibir(buffer, CUATRO_BYTES);
				calcularTiempoFaltante(cliente, tiempoUnix, campo_1, campo_2);
				break;
			case 'L':
				// Uso: L, tiempo, paradaPartida, paradaLlegada
				campo_1 = cliente->recibir(buffer, CUATRO_BYTES);
				campo_2 = cliente->recibir(buffer, CUATRO_BYTES);
				calcularLineaMasRapida(cliente, campo_1, campo_2);
				break;
			case 'R':
				// Uso: R, tiempo, paradaPartida, paradaLlegada
				tiempoUnix = cliente->recibir(buffer, CUATRO_BYTES);
				campo_1 = cliente->recibir(buffer, CUATRO_BYTES);
				campo_2 = cliente->recibir(buffer, CUATRO_BYTES);
				calcularColectivoRecomendado(cliente, tiempoUnix, campo_1, campo_2);
				break;
			default:
				break;
		}
	}
}

server_Servidor::~server_Servidor() {
	for(unsigned int i=0; i<clientes.size(); i++) {
		delete clientes.at(i);
	}
	for(unsigned int j=0; j<clientesVivos.size(); j++) {
		delete clientesVivos.at(j);
	}
}

