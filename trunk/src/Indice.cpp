#include "Indice.h"



indice::indice(list<string>& listaDeArchivos,string nombre_arch){
	//Declaración de variables y estructuras locales
	struct registroAuxiliar{
		char nombreDeActor[100];
		char nombreDePelicula[100];
		char profesion;
	};
	struct registroAuxiliar2{
		char nombreDeActor[100];
		int offsetPelicula;
		char profesion;
	};

	FILE * archivoDeStrings; //Archivo de concatenación de strings
	FILE * archivoAuxiliar; //Contendrá registros de la forma registroAuxiliar
	FILE * archivoAuxiliar2; //Contendrá registros de la forma registroAuxiliar2
	int offset;				//Servirá como variable auxiliar para guardar offsets
	registroAuxiliar registroAux;
	registroAuxiliar2 registroAux2;
	size_t tamAux;	//Almacenará tamaños de cadenas de caracteres.
	parser* prs;
	pelicula* peli;
	char nombrePelicula[100];
	list<staff> listaStaff;
	list<string>::iterator it=listaDeArchivos.begin();
	this->n_arch_indice=nombre_arch + ".idx";
	this->n_arch_conc_string=nombre_arch + "c" + ".conc";
	this->n_arch_principal=nombre_arch + "p" + ".ppal";

	//En esta primera etapa se crea un archivo auxiliar con registros de la forma
	// Nombre de actor|nombre de película|Profesión
	//El tamaño de los registros es de tamaño fijo.
	archivoAuxiliar=fopen("auxiliar","w+b");
	while (it!=listaDeArchivos.end()){
		prs = new parser((it->c_str()));
		while(!(prs->getPelicula(*peli))){
			strcpy(registroAux.nombreDePelicula,(peli->getNombre()).c_str());
			listaStaff=peli->getStaff();
			list<staff>::iterator itStaff=listaStaff.begin();
			while(itStaff!=listaStaff.end()){
				strcpy(registroAux.nombreDeActor,(itStaff->getNombre()));
				registroAux.profesion=itStaff->getProfesion();
				fwrite(&registroAux,sizeof(registroAuxiliar),1,archivoAuxiliar);
				itStaff++;
			}
		}
		delete prs;
		it++;
	}
	fclose(archivoAuxiliar);

	//Acá hay que ordenar el archivo por película

	//Se crea la concatenación de strings de nombres de película y se asignan los offset.
	archivoAuxiliar=fopen("auxiliar","r+b");
	if (feof(archivoAuxiliar)) return;
	archivoDeStrings=fopen(this->n_arch_conc_string.c_str(),"w+b");
	archivoAuxiliar2=fopen("auxiliar2","w+b");

	offset=0;
	fread(&registroAux,sizeof(registroAuxiliar),1,archivoAuxiliar);
	strcpy(nombrePelicula,registroAux.nombreDePelicula);
	tamAux=strlen(nombrePelicula);
	fwrite(&tamAux,sizeof(size_t),1,archivoDeStrings);
	offset=offset+sizeof(size_t);
	fwrite(nombrePelicula,strlen(nombrePelicula),1,archivoDeStrings);
	offset=offset+strlen(nombrePelicula);


	while (!feof(archivoAuxiliar)){
			fread(&registroAux,sizeof(registroAuxiliar),1,archivoAuxiliar);



	}

	//Acá hay que ordenar el archivo por actor
	//Se crea la concatenación de nombres de actores

}

indice::indice(string nombre_arch){
	this->n_arch_indice=nombre_arch + ".idx";
	this->n_arch_conc_string=nombre_arch + "c" + ".conc";
	this->n_arch_principal=nombre_arch + "p" + ".ppal";



	//falta ver como recuperamos cuantos actores tiene el indice
}

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

	fclose(archivo_indice);
	fclose(archivo_ppal);
	return exito;
}

salidas indice::getAllStaff(int ID_pelicula,int Id_staff,list<int>& ID_staff ){

	//se accede mediante Id_staff a esa posicion que seria un actor que actuó en esa pelicula
	// se toma con la estructura correspondiente y se va al siguiente con el "offset al siguiente que trabajo en esa pelicula"
	//se repite esto hasta que se llegue denuevo al Id_staff con el que se empezó

	FILE * archivo_indice;
	archivo_indice = fopen(this->n_arch_indice.c_str(),"r+b");

	if((!archivo_indice)or(Id_staff>this->cantidad_staff*sizeof(es_indice)))
		return error;

	fseek(archivo_indice,Id_staff,SEEK_SET);

	es_indice staff_in;

	fread((void*)&staff_in.offset_al_nombre,sizeof(staff_in.offset_al_nombre),1,archivo_indice);
	fread((void*)&staff_in.offset_al_ppal,sizeof(staff_in.offset_al_ppal),1,archivo_indice);
	fread((void*)&staff_in.profesion,sizeof(staff_in.profesion),1,archivo_indice);

	FILE * archivo_ppal;
	archivo_ppal = fopen(this->n_arch_principal.c_str(),"r+b");

	if(!archivo_ppal)
		return error;

	fseek(archivo_ppal,staff_in.offset_al_ppal,SEEK_SET);

	es_ppal staff_aux,pelicula;

	fread((void*)&staff_aux.distancia_a_padre,sizeof(staff_aux.distancia_a_padre),1,archivo_ppal);
	fread((void*)&staff_aux.id,sizeof(staff_aux.id),1,archivo_ppal);
	fread((void*)&staff_aux.offset_proximo,sizeof(staff_aux.offset_proximo),1,archivo_ppal);

	if((staff_aux.distancia_a_padre!=0)or(staff_aux.id!=Id_staff))
		return error;//porque quiere decir que no empezo levantando el actor por lo que esta mal o que no es el id del staff realmente buscado

	fread((void*)&pelicula.distancia_a_padre,sizeof(pelicula.distancia_a_padre),1,archivo_ppal);
	fread((void*)&pelicula.id,sizeof(pelicula.id),1,archivo_ppal);
	fread((void*)&pelicula.offset_proximo,sizeof(pelicula.offset_proximo),1,archivo_ppal);

	while(pelicula.id!=ID_pelicula){
		if(feof(archivo_ppal)) return error;
		fread((void*)&pelicula.distancia_a_padre,sizeof(pelicula.distancia_a_padre),1,archivo_ppal);
		fread((void*)&pelicula.id,sizeof(pelicula.id),1,archivo_ppal);
		fread((void*)&pelicula.offset_proximo,sizeof(pelicula.offset_proximo),1,archivo_ppal);
		if(pelicula.distancia_a_padre==0) return error; //quiere decir que no tiene la pelicula
	}

	es_ppal staff_levantado;
	int offset_proximo=pelicula.offset_proximo;

	//vaya yendo a todos los offset al proximo hasta que vuelva al primero
	while(staff_levantado.id!=Id_staff){
		fseek(archivo_ppal,offset_proximo,SEEK_SET);
		fread((void*)&pelicula.distancia_a_padre,sizeof(pelicula.distancia_a_padre),1,archivo_ppal);
		fread((void*)&pelicula.id,sizeof(pelicula.id),1,archivo_ppal);
		fread((void*)&pelicula.offset_proximo,sizeof(pelicula.offset_proximo),1,archivo_ppal);

		fseek(archivo_ppal,offset_proximo - (int)pelicula.distancia_a_padre,SEEK_SET);
		fread((void*)&staff_levantado.distancia_a_padre,sizeof(staff_levantado.distancia_a_padre),1,archivo_ppal);
		fread((void*)&staff_levantado.id,sizeof(staff_levantado.id),1,archivo_ppal);

		if(staff_levantado.distancia_a_padre!=0)
			return error;//quiere decir que no llego a un id de actor

		if(staff_levantado.id!=Id_staff)
			ID_staff.push_front(staff_levantado.id);

		offset_proximo=pelicula.offset_proximo;

	}

	fclose(archivo_indice);
	fclose(archivo_ppal);

	return exito;
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

	fclose(archivo_conc);
	fclose(archivo_indice);

	return exito;
}

salidas indice::getNombrePelicula (string& nombre, int ID_pelicula){

	//se accede mediante el ID_peliculas al archivo de concat de strings y se extrae el nombre

	FILE * archivo_conc;
	archivo_conc = fopen(this->n_arch_conc_string.c_str(),"r+b");

	if(!archivo_conc)
		return error;

	fseek(archivo_conc,ID_pelicula,SEEK_SET);

	es_conc_nom nombre_pel;

	fread((void*)&nombre_pel.longitud,sizeof(nombre_pel.longitud),1,archivo_conc);
	fread((void*)&nombre_pel.string,nombre_pel.longitud*sizeof(char),1,archivo_conc);

	nombre=nombre_pel.string;

	fclose(archivo_conc);

	return exito;
}
