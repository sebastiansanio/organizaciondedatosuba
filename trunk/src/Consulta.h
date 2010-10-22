/*
 * Consulta.h
 *
 *  Created on: Oct 22, 2010
 *      Author: santiago
 */

#ifndef CONSULTA_H_
#define CONSULTA_H_

#include "Indice.h"

using namespace std;

class consulta {
	private:
		typedef enum {degree_entre_actores,degree_hasta_actor}tipo;
		indice index;
		list<string> res_consulta;

	public:
		consulta (string& );

		/* Calcula los grados de separación entre dos actores*/
		salidas degree(const string&,const string&);

		/* Calcula todos los actores que se encuentran en un cierto grado de
		 * separacion*/
		salidas degree(const string&,int);

		void imprimir(tipo);

};

#endif /* CONSULTA_H_ */
