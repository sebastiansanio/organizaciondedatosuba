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

class Consulta {
	private:
	bool tieneArchivoPreConsulta;
	ArchivoPreConsulta* archivo;
		//indice index;
		//list<string> res_consulta;

	public:
		Consulta();

		list<peliculaHijo> staffHijos(int staffID);

		/* Calcula los grados de separación entre dos actores*/
		list<peliculaHijo> caminoMinimoActores(int staffOrigen, int staffDestino, int gradoMax);

		list<padrePeliculaHijo> actoresHijos(int staffOrigen, int gradoMax);

		bool armarArchivoPreConsulta(int gradoMax);

		virtual ~Consulta();
};

#endif /* CONSULTA_H_ */
