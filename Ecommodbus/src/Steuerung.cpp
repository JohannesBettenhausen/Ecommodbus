/*
 * Steuerung.cpp
 *
 *  Created on: 02.09.2018
 *      Author: johannes
 */

#include "Steuerung.h"
#include "Datenbank.h"
#include <string>
#include "Ecommodbus.h"
#include <sstream>
#include <thread>
#include <queue>
#include "Rest.h"
Steuerung::Steuerung() {
	// TODO Auto-generated constructor stub
	this->jsonstring="{ \n";
}

void Steuerung::hole_daten(std::string ipv4){

	std::string daten=ipv4+"\n";
	mod::Ecommodbus ecomS("TCP",ipv4);
	daten+=ecomS.ecom_modbus_master_parsemode(4);
	this->queueschranke.lock();
	this->schlange.push(daten);
	this->queueschranke.unlock();

}

void Steuerung::parse_datenstrom(string eingang){

	string jsonarr [15];
	int hintererzeiger=0;
	int vordererzeiger=0;
	for(int i=0;i<31;i++){


		if(i==0){

			hintererzeiger=(int)eingang.find_first_of("\n", vordererzeiger);

			jsonarr[i]="Offen_"+eingang.substr(vordererzeiger, hintererzeiger-vordererzeiger);
		}
			else if (i%2==1){

				hintererzeiger=(int)eingang.find_first_of(":", vordererzeiger);

				}
				else if(i%2==0){

					hintererzeiger=(int)eingang.find_first_of("\n", vordererzeiger);
					//stringstream temp_s;
					//temp_s<<i/2;
					//jsonarr[i]=temp_s.str();
					string temp=eingang.substr(vordererzeiger, hintererzeiger-vordererzeiger);
					temp.erase(std::remove(temp.begin(), temp.end(), ' '), temp.end());
					jsonarr[i/2]=temp;
				}

			vordererzeiger=hintererzeiger+1;
		}


		mod::Rest vb(jsonarr);
}

void Steuerung::json_abschluss(){

	this->jsonstring+="}";
	cout<<this->jsonstring;
}

int main(int argc,char *argv[],char *envp[]){


	/*mod::Rest vb;
	vb.jsonhochladen();
	*/


	//------------------------------------------------------------------------------------------


	Steuerung befehl;

	while(1){

		//std::thread p1(&Steuerung::hole_daten,&befehl,"127.0.0.1");
		std::thread p2(&Steuerung::hole_daten,&befehl,"192.168.178.50");

		p2.join();
		//p1.join();
		while(!befehl.schlange.empty()){

			befehl.parse_datenstrom(befehl.schlange.front());
			befehl.schlange.pop();
		}
		befehl.json_abschluss();
	}


	/*Datenbank Oeffen("ecomModbusOeffen","tcp://127.0.0.1:3306", "root","__ecomRoot__");
	string ids[2]={"Offennummer","INT"};
	Oeffen.tabelle_erstellen("Offen_1",ids,1);
	Oeffen.tabelle_erstellen("Offen_2",ids,1);
	cout<<Oeffen.tabelle_existiert("Offen_2")<< std::endl;
	cout<<Oeffen.tabelle_existiert("Offen_22")<< std::endl;
	string eintrag[2]={"Offennummer","3"};
	Oeffen.tabellen_eintrag("Offen_1",eintrag, 1);
	string eintrag2[2]={"Offennummer","5"};
	Oeffen.tabellen_eintrag("Offen_1",eintrag2, 1);
	cout<<Oeffen.datenbank_abfrage("Offen_1")<< std::endl;*/

}
