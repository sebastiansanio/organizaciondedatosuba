#ifndef CONSULTA_H_
#define CONSULTA_H_

#include "Indice.h"
#include "ArchivoPreConsulta.h"

using namespace std;

#define CANTVERTICES 11

typedef struct {
	bool conocido;
	char distancia;
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
	ArchivoPreConsulta* archivoConsulta;
	bool armarArchivoPreConsulta(int gradoMax);
	indice* index;
		//list<string> res_consulta;

	public:
		Consulta(bool preConsulta);

		list<peliculaHijo> staffHijos(int staffID);

		/* Calcula los grados de separación entre dos actores*/
		list<peliculaHijo> caminoMinimoActores(int staffOrigen, int staffDestino, int gradoMax);

		list<padrePeliculaHijo> actoresHijos(int staffOrigen, int gradoMax);

		void imprimirCaminoMinimoActores(int actorOrigen, int actorDestino);

		void imprimirActoresADistancia(int actorOrigen, char distancia);

		virtual ~Consulta();
};

#endif /* CONSULTA_H_ */
