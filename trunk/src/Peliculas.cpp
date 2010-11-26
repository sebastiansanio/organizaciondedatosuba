#include "Peliculas.h"

pelicula::pelicula(string nombre ,int cantStaff){
	this->nombre = nombre;
	this->cantStaff = cantStaff;
}

string pelicula::getNombre(){
	return this->nombre;
}

void pelicula::setStaff(list<staff>* listaStaff){
	this->listaStaff = listaStaff;
}

void pelicula::addStaff(staff staffAgregar){
	this->listaStaff->push_back(staffAgregar);
}

list<staff>* pelicula::getStaff(){
	return this->listaStaff;
}
