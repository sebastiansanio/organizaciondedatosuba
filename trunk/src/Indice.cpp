#include "Indice.h"

salidas indice::getID_staff(const string& nombre, int& id){

	//se realizaria la busqueda binaria en el indice con el nombre obtenido y se devuelve el offset en el indice mediante id y se retorna exito. Si no se encuentra se retorna error

	return error;
}

salidas indice::getAllPeliculas(int ID_staff, list<int>& ID_peliculas){

	//se accede mediante el ID_staff a la posicion en indice y se obtiene la posicion en el archivo principal y se levanta con la estructura correspondiente que debe ser 0"ID_staff"0000 y se levantan todos los ids de las peliculas en forma secuencial
	//hasta que aparezca denuevo algo de la forma 0"id"0000 que quiere decir que ya llego a otro actor

	FILE * archivo_indice;
	archivo_indice = fopen(this->n_arch_indice.c_str(),"r+b");

	if((!archivo_indice)or(ID_staff>this->cantidad_staff*sizeof(es_indice)))
		return error;

	fseek(archivo_indice,ID_staff,SEEK_SET);

	es_indice staff;

	fread((void*)&staff.offset_al_nombre,sizeof(staff.offset_al_nombre),1,archivo_indice);
	fread((void*)&staff.offset_al_ppal,sizeof(staff.offset_al_ppal),1,archivo_indice);
	fread((void*)&staff.profesion,sizeof(staff.profesion),1,archivo_indice);

	FILE * archivo_ppal;
	archivo_ppal = fopen(this->n_arch_principal.c_str(),"r+b");

	if(!archivo_ppal)
		return error;

	fseek(archivo_ppal,staff.offset_al_ppal,SEEK_SET);

	es_ppal staff_aux,pelicula;

	fread((void*)&staff_aux.distancia_a_padre,sizeof(staff_aux.distancia_a_padre),1,archivo_ppal);
	fread((void*)&staff_aux.id,sizeof(staff_aux.id),1,archivo_ppal);
	fread((void*)&staff_aux.offset_proximo,sizeof(staff_aux.offset_proximo),1,archivo_ppal);

	if(staff_aux.distancia_a_padre!=0)
		return error;//porque quiere decir que no empezo levantando el actor por lo que esta mal

	fread((void*)&pelicula.distancia_a_padre,sizeof(pelicula.distancia_a_padre),1,archivo_ppal);
	fread((void*)&pelicula.id,sizeof(pelicula.id),1,archivo_ppal);
	fread((void*)&pelicula.offset_proximo,sizeof(pelicula.offset_proximo),1,archivo_ppal);

	while(pelicula.distancia_a_padre!=0){//veo que no levante un actor nuevo
		ID_peliculas.push_front(pelicula.id);
		fread((void*)&pelicula.distancia_a_padre,sizeof(pelicula.distancia_a_padre),1,archivo_ppal);
		fread((void*)&pelicula.id,sizeof(pelicula.id),1,archivo_ppal);
		fread((void*)&pelicula.offset_proximo,sizeof(pelicula.offset_proximo),1,archivo_ppal);
	}

	return exito;
}

salidas indice::getAllStaff(int ID_pelicula,int Id_staff,list<int>& ID_staff ){

	//se accede mediante Id_staff a esa posicion que seria un actor que actuó en esa pelicula
	// se toma con la estructura correspondiente y se va al siguiente con el "offset al siguiente que trabajo en esa pelicula"
	//se repite esto hasta que se llegue denuevo al Id_staff con el que se empezó

	return error;
}

salidas indice::getStaff(int ID_staff,staff& staff_d){

	//se accede mediante ID_staff al offset en el indice y se extrae la informacion

	FILE * archivo_indice;
	archivo_indice = fopen(this->n_arch_indice.c_str(),"r+b");

	if((!archivo_indice)or(ID_staff>this->cantidad_staff*sizeof(es_indice)))
		return error;

	fseek(archivo_indice,ID_staff,SEEK_SET);

	es_indice staff_aux;

	fread((void*)&staff_aux.offset_al_nombre,sizeof(staff_aux.offset_al_nombre),1,archivo_indice);
	fread((void*)&staff_aux.offset_al_ppal,sizeof(staff_aux.offset_al_ppal),1,archivo_indice);
	fread((void*)&staff_aux.profesion,sizeof(staff_aux.profesion),1,archivo_indice);

	FILE * archivo_conc;
	archivo_conc = fopen(this->n_arch_conc_string.c_str(),"r+b");

	if(!archivo_conc)
		return error;

	fseek(archivo_conc,staff_aux.offset_al_nombre,SEEK_SET);

	es_conc_nom nombre;
	fread((void*)&nombre.longitud,sizeof(nombre.longitud),1,archivo_conc);
	fread((void*)&nombre.string,nombre.longitud*sizeof(char),1,archivo_conc);

	staff aux(nombre.string,staff_aux.profesion);

	staff_d=aux;

	return exito;
}

salidas indice::getNombrePelicula (string&, int ID_pelicula){

	//se accede mediante el ID_peliculas al archivo de concat de strings y se extrae el nombre

	return error;
}
