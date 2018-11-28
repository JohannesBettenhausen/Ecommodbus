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



public:

	/*Queue in der die einzelnen Outputsteams der Threads af die bearbeitung warten*/
	std::queue<std::string> schlange;
	/*Variable fuer die anzahl der Oeffen*/
	int anzahl;
	/**/
	Steuerung();
	/*Greift die Daten von den Brennern ab*/
	void hole_daten(std::string ipv4);
	/*Extrahiert die relevanten Daten aus dem Outputstream*/
	void parse_datenstrom(std::string eingang);
	/*Gibt ein Array mit IPadressen zurueck*/
	void ermittle_ipadressen(std::string ipadressen[],std::string ipstrom);
	/*Extrahiert IPandressen aus der Konfigurationsdatei*/
	std::string ipadressen_extrahieren();

};

#endif /* IMPLEMENT_STEUERUNG_H_ */
