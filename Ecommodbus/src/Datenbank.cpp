
#include<mysql_connection.h>
#include<cppconn/driver.h>
#include<cppconn/resultset.h>
#include<cppconn/exception.h>
#include<cppconn/statement.h>
#include<cppconn/sqlstring.h>
#include<string>
#include "Datenbank.h"
using namespace std;
using namespace sql;

/*Konstruktoren*/
Datenbank::Datenbank(string datenbankname, string  vbdaten, string  benutzer, string  passwort)
{
	
	this->verbindung = get_driver_instance();
	/*Verbinde mit SQL-Server*/
	this->datenbank = verbindung->connect(zuSQLString(vbdaten), zuSQLString(benutzer), zuSQLString(passwort));
	/*Lade Datenbank*/
	this->datenbank->setSchema(zuSQLString(datenbankname));
	this->befehlsgeber=NULL;
	this->ausgabe=NULL;
	
	
}

/*Private Methoden*/

/*Da die Statmentmethoden der API als Parameter nur Chars oder Objekte vom typ SQLString nehmen;*/
/*Das Arbeiten mit regulaeren srings aber angenehmer ist gibt es diese Methode.*/
sql::SQLString Datenbank::zuSQLString(string eingagnsstr) {

	sql::SQLString ausgabe;
	const char *uebergang = eingagnsstr.c_str();
	return ausgabe=uebergang;
}

/*Oeffentliche Methoden*/

void Datenbank::tabelle_erstellen(string  name, string  id[],int spaltenzahl) {

	/*Deklariere Befehlsvariablen fuer MySQL befehle*/
	string befehl = "CREATE TABLE ";
	string spalten = "(";
	string drop = "DROP TABLE IF EXISTS ";

	/*Loesche gleiche; bereits bestehende Tabelen (vorsichtsma�name)*/
	this->befehlsgeber = this->datenbank->createStatement();
	this->befehlsgeber->execute(zuSQLString(drop+name));
	befehl.append(name);

	/*Sammel alle Spallten der Tabelle fuer den Letztendlichen befehl*/
	/*Der Parameter Spaltenzahl wird -1*2 gerechnet weil von 0 angefanen wird zu z�hlen und der Index um 2 eroeht wird*/
	for (int i = 0;i <= 2 * (spaltenzahl - 1);i += 2) {

		sql::SQLString idstemp="";
		idstemp.append(id[i]);
		idstemp.append(" ");
		idstemp.append(id[i + 1]);

		if (i < (spaltenzahl - 1) * 2) {

			idstemp.append(",");
		}
		spalten.append(idstemp);
	}

		/*Zusammenfuegen des MySQL befels und Ausfuehrung*/
		spalten.append(")");
		befehl.append(spalten);
		this->befehlsgeber->execute(zuSQLString(befehl));
}


void Datenbank::tabellen_eintrag(string  tabelle, string  spalteundeintrag[],int spaltenzahl) {

	string sqlbefehl = "INSERT INTO " + tabelle + "(";
	string eintraege = "";
	this->befehlsgeber = this->datenbank->createStatement();
	spaltenzahl *= 2;

	/*i%2=0==Spalten; i%2=1==Eintrag*/
	for (int i = 0; i < spaltenzahl;i++) {

		if (i % 2 == 0) {

			sqlbefehl += spalteundeintrag[i];
		}

		if (i % 2 == 1) {

			eintraege += spalteundeintrag[i];
		}

		if (i + 2 < spaltenzahl && i % 2 == 0) {

			sqlbefehl += ", ";
		}

		if (i + 1 < spaltenzahl && i % 2 == 1) {

			eintraege += ", ";
		}
	}

	sqlbefehl += ") VALUES (" + eintraege + ")";
	this->befehlsgeber->execute(zuSQLString(sqlbefehl));
}

void Datenbank::sqlbefehl(string sqlbefehl) {

	this->befehlsgeber = this->datenbank->createStatement();
	this->befehlsgeber->execute(zuSQLString(sqlbefehl));
}

int Datenbank::tabelle_existiert(string name) {

	this->befehlsgeber = this->datenbank->createStatement();
	this->ausgabe= this->befehlsgeber->executeQuery("show tables");

	while (ausgabe->next())
	{
		
		string tempname=ausgabe->getString(1);

		if (tempname==name){

			return 1;
		}
	}

	return  0;
}

string Datenbank::datenbank_abfrage(string tabelle){

	string ergebniss;
	this->befehlsgeber = this->datenbank->createStatement();
	this->ausgabe= this->befehlsgeber->executeQuery("SELECT * FROM "+tabelle+" AS JSON");

	cout<<ausgabe->next()<<endl;
	return ergebniss=this->ausgabe->getString(1);
}

/*Destuktoren*/
Datenbank::~Datenbank()
{
	this->befehlsgeber = NULL;
	this->datenbank = NULL;
	this->ausgabe = NULL;
	this->verbindung = NULL;
}
