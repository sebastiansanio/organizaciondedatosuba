/*
 * ArchivoPreConsulta.h
 *
 *  Created on: 26/11/2010
 *      Author: damian
 */

#ifndef ARCHIVOPRECONSULTA_H_
#define ARCHIVOPRECONSULTA_H_

#include <iostream>
#include <fstream>
#include <ostream>
#include <list>

using namespace std;

#define GRADOMAX 6

typedef struct {
	int hijo;
	int padre;
	int pelicula;
	char distancia;
} padrePeliculaHijo;


class ArchivoPreConsulta {
private:
	int ultimoActorConstruido;
	int cantidadActores;
	bool construido;
	fstream* archivo;


public:
	ArchivoPreConsulta(int cantActores, bool construirArchivo);
	bool agregarHijosActor(int actor,list<padrePeliculaHijo> hijos);
	void imprimirCamino(int actorOrigen, int actorDestino);
	void imprimirPosicionesActores();
	list<padrePeliculaHijo> ActoresHijosADistancia(int actor, char distancia);
	padrePeliculaHijo ActorHijo(int actorOrigen, int actorDestino);
	list<padrePeliculaHijo> CaminoActorHijo(int actorOrigen, int actorDestino);
	virtual ~ArchivoPreConsulta();

};

#endif /* ARCHIVOPRECONSULTA_H_ */
