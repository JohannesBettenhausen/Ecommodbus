/*
 * Steuerung.h
 *
 *  Created on: 02.09.2018
 *      Author: johannes
 */

#ifndef IMPLEMENT_STEUERUNG_H_
#define IMPLEMENT_STEUERUNG_H_

#include <string>
#include <queue>
#include <mutex>

class Steuerung {

private:

	/*Stellt sicher, dass die Queue nur von einem Thread gleichzeitig befüllt wird*/
	std::mutex queueschranke;
	/*Jsonstring für REsT*/
	std::string jsonstring;

public:

	/*Queue in der die einzelnen Outputsteams der Threads af die bearbeitung warten*/
	std::queue<std::string> schlange;
	/**/
	Steuerung();
	/*Greift die Daten von den Brennern ab*/
	void hole_daten(std::string ipv4);
	/*Extrahiert die relevanten Daten aus dem Outputstream*/
	void parse_datenstrom(std::string eingang);

	void json_abschluss();
};

#endif /* IMPLEMENT_STEUERUNG_H_ */
