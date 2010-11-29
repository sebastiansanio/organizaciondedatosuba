#include "Peliculas.h"

pelicula::pelicula(string nombre ,int cantStaff){
	this->nombre = nombre;
	this->cantStaff = cantStaff;
	this->listaStaff = new list<staff*>();
}

string pelicula::getNombre(){
	return this->nombre;
}

void pelicula::setCantStaff(int cantStaff){
	this->cantStaff = cantStaff;
}

void pelicula::addStaff(staff* staffAgregar){
	list<staff*>::iterator it = listaStaff->begin();
	bool encontrado = false;
	while(it != listaStaff->end() and !encontrado){
		if((*it)->getNombre() == staffAgregar->getNombre());
			encontrado = true;
		it++;
	}
	if(!encontrado)
		this->listaStaff->push_back(staffAgregar);
}

list<staff*>* pelicula::getStaff(){
	return this->listaStaff;
}
