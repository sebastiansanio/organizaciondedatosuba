/*
 * Parser.h
 *
 *  Created on: Oct 22, 2010
 *      Author: santiago
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "Peliculas.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <stdlib.h>

class parser{
	private:
	ifstream* archivoPeliculas;
	list<pelicula*> listaPeliculas;
	string decodificador;

	public:
	parser(const char* nombre);
	list<pelicula*> getPeliculasDeArchivo();
	string limpiarNombre(string nombreALimpiar);
	string decodificar(string palabraADecodificar);
	virtual ~parser();

};

#endif /* PARSER_H_ */
