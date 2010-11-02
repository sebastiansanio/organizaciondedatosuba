#ifndef INDICE_H_
#define INDICE_H_

#include "Parser.h"
#include "Peliculas.h"

#include <string>

using namespace std;

struct es_indice{
	long int offset_al_nombre;
	long int offset_al_ppal;
	char profesion;
};

struct es_ppal{
	char distancia_a_padre;
	long int id;
	long int offset_proximo;
};

struct es_conc_nom{
	int longitud;
	char * string;
};

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
		string n_arch_indice;
		string n_arch_principal;
		string n_arch_conc_string;
		long int cantidad_staff; //para la busqueda binaria

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


/* Para manejar los indices vamos a tener 3 archivos:
 *
 * el indice --> n_arch_indice
 * el principal --> n_arch_principal
 * el de concatenacion de strings --> n_arch_conc_string
 *
 * ¿Que tiene cada uno?
 *
 * el indice:
 *
 * offset al nombre; //serai del archivo de concatenacion de strings
 * profesion;
 * offset a donde empieza en archivo principal;
 *
 *el principal:
 *
 *		0-A1-0000	//A1 es el offset en el indice
 *		1-P1-"offset al Proximo que aparece P1" //Pi es el id de la pelicula que es la posicion en el archivo de concat de strings, el 1 es cuanto de offset para encontrar el id del actor..
 *		2-P2-"offset al Proximo que aparece P2"
 *		3-P4-"offset al Proximo que aparece P3"
 *		4-P4-"offset al Proximo que aparece P4"
 *		0-A2-0000
 *		.
 *		..
 *		...
 *		....
 *
 *	el concatenacion:
 *
 *		4casa5casas6casita.....
 *
 *	Esto lo escribir para que los 3 estemos de acuerdo aca y sigamos esa idea cualquier cosa digan y lo hablamos
 * */

#endif /* INDICE_H_ */
