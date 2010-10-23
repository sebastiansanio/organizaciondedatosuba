//============================================================================
// Name        : Tp.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "Ordenamiento.h"
#include <stdlib.h>


int comparador (void* nn1, void* nn2) {
	int* n1 = (int *) nn1;
	int* n2 = (int* ) nn2;

	if (*n1 == *n2)
		return 0;
	return (*n1 > *n2)?-1:1;
}

void pers(ofstream& salida,void* elem ) {
	int* i = (int*) elem;
	salida << *i << endl;
	cout <<"El numero guardado " << *i << endl;
}

void destructor (void* i) {
	int* j = (int*) i;
	free (j);
}

void * constructor (char* stream) {
	int* i = (int*) malloc (sizeof(int));
	*i = atoi(stream);
	return i;
}



int main() {
	ofstream out;


	// Pruebas al Selection Sort
	out.open ("Prueba.bin", ios::out | ios::binary);
	out << 2 << 1 << 3 << 5 << 7 << 8 << 2 << 6 << 4 << 1 << 2 << 0 << 3 << 5 ;

	out.close();
	string ss ("Prueba");

	replac_selection ord (3,1,constructor,destructor,comparador,pers);
	ord.ordenar(ss);

}



