/*
 * Indice.h
 *
 *  Created on: Oct 22, 2010
 *      Author: santiago
 */

#ifndef INDICE_H_
#define INDICE_H_

#include "Parser.h"
#include "Peliculas.h"

#include <string>

using namespace std;


typedef enum {error, exito}salidas;

class sta_indice{
	private:
		int offset;
		string nombre;

	public:
		sta_indice(string& nombre,int offset)
		{
			this->nombre = nombre;
			this->offset = offset;
		}

		int getOffset(){return offset;}
		const string& getNombre(){return nombre;}
};

class indice{
	private:
		string n_arch;

	public:

		/*Constructor para crear el indice a partir de varios XML*/
		indice(list<string>&);//lista de nombres de los archivos XML

		/*Constructor para el indice ya creado*/
		indice(string);


		/*retorna el id del staff, -1 sino lo encuentra*/
		salidas getID_staff(const string&, int&);//nombre del staff


		salidas getAllPeliculas(int ID_staff, list<int>& ID_peliculas);

		/* el id del staff del que se parte (no lo devuelve)*/
		salidas getAllStaff(int ID_pelicula,int Id_staff,list<int>& ID_staff );


		salidas getStaff(int ID_staff,staff&);
		salidas getNombrePelicula (string&, int ID_pelicula);


};


#endif /* INDICE_H_ */
