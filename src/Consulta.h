#ifndef CONSULTA_H_
#define CONSULTA_H_

#include "Indice.h"
#include "ArchivoPreConsulta.h"

using namespace std;

#define CANTVERTICES 11

typedef struct {
	bool conocido;
	int distancia;
	int padre;
	int pelicula;
} datosStaff;

typedef struct {
	int hijo;
	int pelicula;
} peliculaHijo;

class consulta {
	private:
		indice index;
		list<string> res_consulta;

	public:
		consulta();

		list<peliculaHijo> staffHijos(int staffID);

		/* Calcula los grados de separación entre dos actores*/
		list<peliculaHijo> caminoMinimoActores(int staffOrigen, int staffDestino, int gradoMax);

		/* Calcula todos los actores que se encuentran en un cierto grado de
		 * separacion*/
		salidas actoresAGrados(int staffOrigen, int grado);

		void imprimir();
};

#endif /* CONSULTA_H_ */
