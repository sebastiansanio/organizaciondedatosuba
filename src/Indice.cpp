#include "Indice.h"

salidas indice::getID_staff(const string& nombre, int& id){

	//se realizaria la busqueda binaria en el indice con el nombre obtenido y se devuelve el offset en el indice mediante id y se retorna exito. Si no se encuentra se retorna error

	return error;
}

salidas indice::getAllPeliculas(int ID_staff, list<int>& ID_peliculas){

	//se accede mediante el ID_staff a la posicion del archivo principal y se levanta con la estructura correspondiente que debe ser 0"ID_staff"0000 y se levantan todos los ids de las peliculas en forma secuencial
	//hasta que aparezca denuevo algo de la forma 0"id"0000 que quiere decir que ya llego a otro actor

	return error;
}

salidas indice::getAllStaff(int ID_pelicula,int Id_staff,list<int>& ID_staff ){

	//se accede mediante Id_staff a esa posicion que seria un actor que actuó en esa pelicula
	// se toma con la estructura correspondiente y se va al siguiente con el "offset al siguiente que trabajo en esa pelicula"
	//se repite esto hasta que se llegue denuevo al Id_staff con el que se empezó

	return error;
}

salidas indice::getStaff(int ID_staff,staff& staff){

	//se accede mediante ID_staff al offset en el indice y se extrae la informacion

	return error;
}

salidas indice::getNombrePelicula (string&, int ID_pelicula){

	//se accede mediante el ID_peliculas al archivo de concat de strings y se extrae el nombre

	return error;
}
