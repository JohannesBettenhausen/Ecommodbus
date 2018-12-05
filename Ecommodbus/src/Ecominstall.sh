# Installiert die nötigen kompnenten für das Ecommodbus Projekt
# Die aktuelle Version sollte bei alle Ubuntuderivaten funktionieren 
# Gegebenenfalls müssen sie das Script mit chmod +x Ecominstall.sh ausfürbar machen
#Das Script muss mit sudo ausgeführt werden.
#Die Ausführbare datei befindet sich im Oberordner (Parentnode) 

apt-get update
apt-get install g++
apt-get install libmodbus-dev
apt-get install boost
apt-get install libcpprest-dev

# Starte Ubersetzung der Ecommodbus.cpp

g++ -c Ecommodbus.cpp -O0 -g3 -Wall -std=c++11 -D__unix__ -I../ecomModbusSimulator/ -lnsl -lmodbus  -pthread -Wl,--no-as-needed -lcpprest -lboost_system -lssl -lcrypto

# Starte_Ubersetzung_der_Rest.cpp

g++ -c Rest.cpp -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -D__unix__ -I../ecomModbusSimulator/ -lnsl -lmodbus  -pthread -Wl,--no-as-needed -lcpprest -lboost_system -lssl -lcrypto

#Starte_Ubersetzung_der_Steuerung.cpp

g++ -c Steuerung.cpp -O0 -g3 -Wall -std=c++11 -D__unix__ -I../ecomModbusSimulator/ -lnsl -lmodbus  -pthread -Wl,--no-as-needed -lcpprest -lboost_system -lssl -lcrypto

#Starte_verbindung_der_.o_Dateien

g++ -o Exec Ecommodbus.o Rest.o Steuerung.o -lcpprest -lboost_system -lcrypto -lssl -std=c++11 -D__unix__ -lnsl -lmodbus  -pthread -Wl,--no-as-needed 

mv Exec ..
