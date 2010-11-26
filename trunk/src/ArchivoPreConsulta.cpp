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
	int pos = archivo->tellg();
	archivo->seekg((actor-1)*4,ios::beg);
	archivo->write((char*)&pos,4);
	archivo->seekg(pos,ios::beg);
	list<padrePeliculaHijo>::iterator iter = hijos.begin();
	while(iter!=hijos.end()){
		if((*iter).hijo!=actor){
			archivo->write((char*)&(*iter),sizeof(padrePeliculaHijo));
		}
		iter++;
	}
	if(actor==cantidadActores){
		construido=true;
		archivo->flush();
	}
	return true;
}

padrePeliculaHijo ArchivoPreConsulta::ActorHijo(int actorOrigen, int actorDestino){
		int ini;
		int fin;
		archivo->seekg((actorOrigen-1)*4,ios::beg);
		archivo->read((char*)&ini,4);
		archivo->read((char*)&fin,4);
		int registros=(fin-ini)/sizeof(padrePeliculaHijo);
		int pivote = ini + ((registros/2)*sizeof(padrePeliculaHijo));
		padrePeliculaHijo aux;
		archivo->seekg(pivote,ios::beg);
		archivo->read((char*)&aux,sizeof(padrePeliculaHijo));
		int actorPivote=aux.hijo;
		while(actorPivote!=actorDestino and pivote!=ini and pivote!=fin){
			if(actorPivote>actorDestino){
				fin=pivote;
			} else {
				ini=pivote;
			}
			registros=(fin-ini)/sizeof(padrePeliculaHijo);
			pivote = ini + ((registros/2)*sizeof(padrePeliculaHijo));
			archivo->seekg(pivote,ios::beg);
			archivo->read((char*)&aux,sizeof(padrePeliculaHijo));
			actorPivote=aux.hijo;
		}
		return aux;
}

list<padrePeliculaHijo> ArchivoPreConsulta::CaminoActorHijo(int actorOrigen, int actorDestino){
	int actor=actorDestino;
	padrePeliculaHijo caminoActor;
	list<padrePeliculaHijo> listaCamino;
	while(actor!=actorOrigen){
		caminoActor=ActorHijo(actorOrigen,actor);
		listaCamino.push_back(caminoActor);
		actor=caminoActor.padre;
	}
	return listaCamino;
}

list<padrePeliculaHijo> ArchivoPreConsulta::ActoresHijos(int actor){
	int posicionActor;
	int posicionActorSiguiente;
	archivo->seekg((actor-1)*4,ios::beg);
	archivo->read((char*)&posicionActor,4);
	archivo->read((char*)&posicionActorSiguiente,4);
	archivo->seekg(posicionActor,ios::beg);
	list<padrePeliculaHijo> lista;
	while(archivo->tellg()<posicionActorSiguiente){
		padrePeliculaHijo aux;
		archivo->read((char*)&aux,sizeof(padrePeliculaHijo));
		lista.push_back(aux);
	}
	archivo->seekg(0,ios::end);
	return lista;
}

void ArchivoPreConsulta::imprimirPosicionesActores(){
	if(!construido) return;
	archivo->seekg(0,ios::beg);
	for(int i=0;i<cantidadActores;i++){
		int posicionAux;
		archivo->read((char*)&posicionAux,4);
		cout << "El actor " << i+1 << " comienza en la posicion " << posicionAux << endl;
	}
	archivo->seekg(0,ios::end);
}

ArchivoPreConsulta::~ArchivoPreConsulta() {
	if(archivo->is_open()) archivo->close();
	delete archivo;
}
