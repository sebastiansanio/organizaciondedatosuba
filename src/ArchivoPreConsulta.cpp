/*
 * ArchivoPreConsulta.cpp
 *
 *  Created on: 26/11/2010
 *      Author: damian
 */

#include "ArchivoPreConsulta.h"

ArchivoPreConsulta::ArchivoPreConsulta(int cantActores) {
	fstream* archivo2 = new fstream;
	archivo2->open("hola.bin",ios::in|ios::out|ios::binary);
	archivo=archivo2;
	cantidadActores=cantActores;
	ultimoActorConstruido=0;
	construido=false;
	//Guarda para cada actor -1, o sea que para ese actor todavía no hay hijos
	int aux = -1;
	if(archivo->good()){
		for(int i=0;i<cantidadActores;i++){
			archivo->write((char*)&aux,4);
		}
	}
}

bool ArchivoPreConsulta::agregarHijosActor(int actor,list<padrePeliculaHijo> hijos){
	if(construido) return false;
	if(actor!=ultimoActorConstruido+1) return false;
	ultimoActorConstruido++;
	list<padrePeliculaHijo>::iterator iter = hijos.begin();
	while(iter!=hijos.end()){
		archivo->write((char*)&(*iter),sizeof(padrePeliculaHijo));
	}
	if(actor==cantidadActores) construido=true;
	return true;
}

ArchivoPreConsulta::~ArchivoPreConsulta() {
	if(archivo->is_open()) archivo->close();
	delete archivo;
}
