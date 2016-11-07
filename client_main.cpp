#include <iostream>

#include "client_Cliente.h"

int main(int argc, char *argv[]) {
	client_Cliente cliente;
	cliente.conectar(argc, argv);
	cliente.desconectar();
	return 0;
}
