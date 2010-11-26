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

using namespace std;

class ArchivoPreConsulta {
private:
	int ultimoActorConstruido;
	bool construido;
	fstream* archivo;


public:
	ArchivoPreConsulta(int cantActores);
	virtual ~ArchivoPreConsulta();

};

#endif /* ARCHIVOPRECONSULTA_H_ */
