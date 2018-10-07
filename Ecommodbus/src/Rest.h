/*
 * Rest.h
 *
 *  Created on: 20.09.2018
 *      Author: johannes
 */

#ifndef REST_H_
#define REST_H_
#include <cpprest/http_client.h>

namespace mod {

class Rest {

private:

	std::string datenart[15]={"offenname","O2","CO","CO2","NO","NO2","SO2","NOX","ETA","Lamda","Losses","Air_temperatur","Gas_temp","Airpressure","Sensor Temperature"};
	void jsonhochladen(web::json::value postData);
	//web::http::client::http_client *client(("http//localhost"));

public:
	Rest(std::string daten[]);
	void datenhochladen(std::string daten[]);
};

} /* namespace mod */

#endif /* REST_H_ */
