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
#include <list>

using namespace std;

typedef struct {
	int padre;
	int hijo;
	int pelicula;
} padrePeliculaHijo;


class ArchivoPreConsulta {
private:
	int ultimoActorConstruido;
	int cantidadActores;
	bool construido;
	fstream* archivo;


public:
	ArchivoPreConsulta(int cantActores);
	bool agregarHijosActor(int actor,list<padrePeliculaHijo> hijos);
	virtual ~ArchivoPreConsulta();

};

#endif /* ARCHIVOPRECONSULTA_H_ */
