#pragma once
#include <string>
#include<mysql_connection.h>
#include<cppconn/driver.h>
#include<cppconn/resultset.h>
#include<cppconn/exception.h>
#include<cppconn/statement.h>
#include<cppconn/sqlstring.h>
using namespace std;

class Datenbank
{
private:

	sql::Connection *datenbank;
	sql::Statement *befehlsgeber;
	sql::Driver *verbindung;
	sql::ResultSet *ausgabe;

	/*Da die Statmentmethoden der API kann als Parameter nur Chars oder Objekte vom typ SQLString nemen;*/
	/*Das Arbeiten mit regul�ren srings aber angenehmer ist gibt es diese Methode.*/
	sql::SQLString zuSQLString(string eingansstr);

public:

	/*Konstruktor stellt verbindung mit dem SQLserver und der Datenbank her*/
	/*Parameter: string datenbankname, string  vbdaten, string  benutzer, string  passwort)*/
	/*vbdaten beispiel: tcp://127.0.0.1:3306*/
	Datenbank(string datenbankname, string  vbdaten, string  benutzer, string  passwort);

	/*Erstellt eine neue Tabelle*/
	/*�bersreibt die alte falls bereits eine mit dem selben namen exsistiert*/
	/*Parameter:string  name, string  id[],int spaltenzahl*/
	/*Syntax id[]: {Spaltenid1, Typ1,...,Spaltenidn,Typn}*/
	/*Der Name darf keine Leerzeichen enthalten*/
	void tabelle_erstellen(string  name, string id[],int spaltenzahl);

	/*Tr�gt werte in die Spalte der Tabelle ein*/
	/*Parameter: string tabelle (name der Tabelle), string spalteundeintrag[] (Spalte und eintag fuer die entsprechende Spalte), int spaltenzahl (anzahl der Spalten bei denen werte eingeragen werden sollen)*/
	/*die Syntax f�r das Array spalte und eintrag is {spaltenid1,eintag1,...n}*/
	void tabellen_eintrag(string  tabelle, string  spalteundeintrag[],int spaltenzahl);

	/*In dieser Methode k�nnen belibige sqlbefele ausfgef�rt werden*/
	/*Parameter string sqlbefehl*/
	void sqlbefehl(string sqlbefehl);

	/*Diese Methode pr�ft, ob eine tabelle Existiert*/
	/*R�ckgabe: Die Methode gibt 0 zur�ck, wenn die tablelle nicht existier,1 falls doch*/
	/*Parameter: string name*/
	int tabelle_existiert(string name);

	/*Gibt die ergebnisse als JSON string aus*/
	string datenbank_abfrage(string tabelle);

	/*Setzt alle Instanzvariablen auf NULL*/
	~Datenbank();
};

