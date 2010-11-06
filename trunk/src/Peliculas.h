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


using namespace std;



class staff {
	private:
		char profesion;
		char * nombre;

	public:
		staff(char *nomb,char prof){this->profesion=prof;this->nombre=nomb;}
		char getProfesion (){return profesion;}
		char* getNombre(){return nombre;}

};

class pelicula {
	private:
		int cantStaff;
		string nombre;
		list<staff> listaStaff;

	public:
		pelicula(string,int);
		string getNombre();
		void addStaff(staff);
		list<staff> getStaff();
};



#endif /* PELICULAS_H_ */
