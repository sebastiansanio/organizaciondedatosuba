/*
 * Peliculas.h
 *
 *  Created on: Oct 22, 2010
 *      Author: santiago
 */

#ifndef PELICULAS_H_
#define PELICULAS_H_


#include <list>
#include <string>
#include <string.h>

using namespace std;

class staff {
	private:
		char profesion;
		string nombre;
	public:
		staff(string nomb,char prof){this->profesion=prof;this->nombre=nomb;}
		char getProfesion (){return profesion;}
		string getNombre(){return nombre;}

};

class pelicula {
private:
		int cantStaff;
		string nombre;
		list<staff*>* listaStaff;

	public:
		pelicula(){}
		pelicula(string nombre ,int cantStaff);
		string getNombre();
		void addStaff(staff* staff);
		void setCantStaff(int cantStaff);
		list<staff*>* getStaff();
		~pelicula(){delete listaStaff;}
};



#endif /* PELICULAS_H_ */
