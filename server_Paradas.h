
#ifndef SERVER_PARADAS_H_
#define SERVER_PARADAS_H_

#include <string>
#include <vector>

class server_Paradas {
private:
	std::vector<std::vector<int> > paradas;
public:
	server_Paradas();
	int cargarParadas(char* file);
	int getTiempoEntreParadas(int inicio, int fin);
	virtual ~server_Paradas();
private:
	void parsearInformacion(const std::string& linea);
};

#endif /* SERVER_PARADAS_H_ */
