
#ifndef CLIENT_UNIXTIME_H_
#define CLIENT_UNIXTIME_H_

#include <string>
#include <vector>

class client_UnixTime {
public:
	client_UnixTime();
	int convertir(const std::string &tiempo);
	virtual ~client_UnixTime();
private:
	void tokenizar(const std::string &tiempo, std::vector<int>* vector);
	int calcularTiempo(std::vector<int>* fecha);
};

#endif /* CLIENT_UNIXTIME_H_ */
