/*
 * Steuerung.cpp
 *
 *  Created on: 02.09.2018
 *      Author: johannes
 */

#include "Steuerung.h"
#include <string>
#include "Ecommodbus.h"
#include <sstream>
#include <thread>
#include <queue>
#include "Rest.h"
#include <fstream>
using namespace std;

Steuerung::Steuerung() {
	// TODO Auto-generated constructor stub

}

string Steuerung::ipadressen_extrahieren(){

	fstream datei;
	string ipadressen;
	datei.open("/home/johannes/git/repository/Ecommodbus/src/ipadressen");
	if(datei.is_open()){

		while(getline(datei,ipadressen)){

			cout<<ipadressen<<endl;
		}

	}
	else{

		cout<<"Die Konfigurationsdatei exsistiert nicht";
	}
	datei.close();
	return ipadressen;

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


int main(int argc,char *argv[],char *envp[]){


	//------------------------------------------------------------------------------------------


	Steuerung befehl;
	befehl.ipadressen_extrahieren();
	/*
	while(1){

		//std::thread p1(&Steuerung::hole_daten,&befehl,"127.0.0.1");
		std::thread p2(&Steuerung::hole_daten,&befehl,"192.168.178.50");

		p2.join();
		//p1.join();
		while(!befehl.schlange.empty()){

			befehl.parse_datenstrom(befehl.schlange.front());
			befehl.schlange.pop();
		}
	}
	*/
}
