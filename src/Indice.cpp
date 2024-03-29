#include "Indice.h"
#include <stdio.h>

void destruir(void * stream){
	delete (registroAuxiliar2*)stream;
}

void persistir(ofstream& stream, void* elemento){
	stream.write((char*)elemento,sizeof(registroAuxiliar2));
}

int comparar(void * elemento1, void * elemento2){
	registroAuxiliar2 * registro1=(registroAuxiliar2*)elemento1;
	registroAuxiliar2 * registro2=(registroAuxiliar2*)elemento2;
	if(strcmp(registro1->nombreDeActor,registro2->nombreDeActor)==0)
		return 0;
	if(strcmp(registro1->nombreDeActor,registro2->nombreDeActor)<0)
		return 1;
	return -1;
}

void * construir(char * stream){
	registroAuxiliar2* registro=new registroAuxiliar2;
	memcpy((void*)registro,(void*)stream,sizeof(registroAuxiliar2));
	return (void*)registro;
}

indice::indice(list<string>& listaDeArchivos,string nombre_arch){
	FILE * archivoDeStrings; //Archivo de concatenación de strings
	FILE * archivoIndice;
	FILE * archivoPrincipal;
	FILE * archivoAuxiliar; //Contendrá registros de la forma registroAuxiliar
	FILE * archivoAuxiliar2; //Contendrá registros de la forma registroAuxiliar2
	es_ppal registroPrincipal;
	es_indice registroIndice;
	char distancia_a_padre;
	int idAux;
	int offset;				//Servirá como variable auxiliar para guardar offsets
	int offsetAlPrincipal;
	registroAuxiliar registroAux;
	registroAuxiliar2 registroAux2;
	size_t tamAux;	//Almacenará tamaños de cadenas de caracteres.
	parser* prs;
	char *nombrePelicula;
	char *nombreActor;
	list<pelicula*> peliculasArchivo;
	list<staff*>* listaStaff;
	list<string>::iterator it=listaDeArchivos.begin();
	this->n_arch_indice=nombre_arch + ".idx";
	this->n_arch_conc_string=nombre_arch + "c" + ".conc";
	this->n_arch_principal=nombre_arch + "p" + ".ppal";

	//En esta primera etapa se crea un archivo auxiliar con registros de la forma
	//Nombre de actor|nombre de película|Profesión
	//El tamaño de los registros es de tamaño fijo.
	archivoAuxiliar=fopen("auxiliar","w+b");
	while (it!=listaDeArchivos.end()){
		cout<<*it<<endl;
		prs = new parser((it->c_str()));
		peliculasArchivo=prs->getPeliculasDeArchivo();
		list<pelicula*>::iterator iteradorPelicula=peliculasArchivo.begin();
		while(iteradorPelicula!=peliculasArchivo.end()){
			strcpy(registroAux.nombreDePelicula,((*iteradorPelicula)->getNombre()).c_str());
			listaStaff=(*iteradorPelicula)->getStaff();
			list<staff*>::iterator iteradorStaff=listaStaff->begin();
			while(iteradorStaff!=listaStaff->end()){
				strcpy(registroAux.nombreDeActor,((*iteradorStaff)->getNombre()).c_str());
				registroAux.profesion=((*iteradorStaff)->getProfesion());
				fwrite(&registroAux,sizeof(registroAuxiliar),1,archivoAuxiliar);
				iteradorStaff++;
			}
			iteradorPelicula++;
		}
		delete prs;
		it++;
	}
	fclose(archivoAuxiliar);

	//Se crea la concatenación de strings de nombres de película y se asignan los offset.
	archivoAuxiliar=fopen("auxiliar","r+b");
	if (feof(archivoAuxiliar)) return;
	archivoDeStrings=fopen(this->n_arch_conc_string.c_str(),"w+b");
	archivoAuxiliar2=fopen("auxiliar2","w+b");
	offset=0;
	fread(&registroAux,sizeof(registroAuxiliar),1,archivoAuxiliar);
	strcpy(registroAux2.nombreDeActor,registroAux.nombreDeActor);
	registroAux2.offsetPelicula=offset;
	registroAux2.profesion=registroAux.profesion;
	fwrite(&registroAux2,sizeof(registroAuxiliar2),1,archivoAuxiliar2);
	tamAux=strlen(registroAux.nombreDePelicula);
	nombrePelicula=(char*)malloc(tamAux+1);
	strcpy(nombrePelicula,registroAux.nombreDePelicula);
	fwrite(&tamAux,sizeof(size_t),1,archivoDeStrings);
	fwrite(nombrePelicula,tamAux,1,archivoDeStrings);

	while (!feof(archivoAuxiliar)){
		fread(&registroAux,sizeof(registroAuxiliar),1,archivoAuxiliar);
		if(feof(archivoAuxiliar)){
			break;
		}
		if (strcmp(registroAux.nombreDePelicula,nombrePelicula)==0){
			strcpy(registroAux2.nombreDeActor,registroAux.nombreDeActor);
			registroAux2.profesion=registroAux.profesion;
			registroAux2.offsetPelicula=offset;
			fwrite(&registroAux2,sizeof(registroAuxiliar2),1,archivoAuxiliar2);
		}
		else{
			offset=offset+sizeof(size_t)+tamAux;
			strcpy(registroAux2.nombreDeActor,registroAux.nombreDeActor);
			registroAux2.profesion=registroAux.profesion;
			tamAux=strlen(registroAux.nombreDePelicula);
			free(nombrePelicula);
			nombrePelicula=(char*)malloc(tamAux+1);
			strcpy(nombrePelicula,registroAux.nombreDePelicula);
			fwrite(&tamAux,sizeof(size_t),1,archivoDeStrings);
			fwrite(nombrePelicula,tamAux,1,archivoDeStrings);
			registroAux2.offsetPelicula=offset;
			fwrite(&registroAux2,sizeof(registroAuxiliar2),1,archivoAuxiliar2);
		}
	}
	offset=offset+sizeof(size_t)+tamAux;
	free(nombrePelicula);
	fclose(archivoAuxiliar2);

	//Acá hay que ordenar el archivo auxiliar 2 por actor
	//Genero particiones
	unsigned int cantArch;

	replac_selection metodo_ordenamiento(2500,sizeof(registroAuxiliar2),construir,destruir,comparar,persistir);
	cantArch=metodo_ordenamiento.ordenar(string("auxiliar2"));

	//Mergeo las particiones
	cantArch=cantArch+1;
	string* vectorString=new string[cantArch];

	for(unsigned int strCont=0;strCont<cantArch;strCont++){
		stringstream streamAux;
		streamAux<<"Temp_auxiliar2"<<strCont<<".bin";
		streamAux>> vectorString[strCont];
//		cout<<vectorString[strCont]<<endl;
	}
	one_way_merge merger(cantArch,vectorString,sizeof(registroAuxiliar2),construir,destruir,comparar,persistir);
	merger.mergear();


	//Se crea el índice de actores y la concatenación de strings de sus nombres.
	archivoAuxiliar2=fopen("Merge.bin","r+b");
	if (feof(archivoAuxiliar2)) return;
	archivoIndice=fopen(this->n_arch_indice.c_str(),"w+b");
	archivoPrincipal=fopen("archivoPrincipalAux","w+b");

	distancia_a_padre=0;
	idAux=0;
	offsetAlPrincipal=0;

	fread(&registroAux2,sizeof(registroAuxiliar2),1,archivoAuxiliar2); //Leo el archivo auxiliar
	registroPrincipal.distancia_a_padre=distancia_a_padre;	//Es un registro de actor por lo que vale 0
	registroPrincipal.id=idAux;	//El índice comienza con id 0
	registroPrincipal.offset_proximo=0;	//Este campo se llenará más adelante
	fwrite(&registroPrincipal,sizeof(es_ppal),1,archivoPrincipal);//Guardamos en el archivo ppal lo extraido

	distancia_a_padre=distancia_a_padre+1;

	//Guardamos la primer pelicula
	registroPrincipal.distancia_a_padre=distancia_a_padre;
	registroPrincipal.id=registroAux2.offsetPelicula;
	fwrite(&registroPrincipal,sizeof(es_ppal),1,archivoPrincipal);

	//Guardamos en el indice el primer actor
	registroIndice.offset_al_nombre=offset;
	registroIndice.offset_al_ppal=offsetAlPrincipal;
	registroIndice.profesion=registroAux2.profesion;
	fwrite(&registroIndice,sizeof(es_indice),1,archivoIndice);

	offsetAlPrincipal=offsetAlPrincipal+1;

	//Concatenamos el nombre del actor
	tamAux=strlen(registroAux2.nombreDeActor);
	nombreActor=(char*)malloc(tamAux+1);
	strcpy(nombreActor,registroAux2.nombreDeActor);
	fwrite(&tamAux,sizeof(size_t),1,archivoDeStrings);
	fwrite(nombreActor,tamAux,1,archivoDeStrings);

	while (!feof(archivoAuxiliar2)){
		fread(&registroAux2,sizeof(registroAuxiliar2),1,archivoAuxiliar2);
		if(feof(archivoAuxiliar2)){
			break;
		}
		if (strcmp(registroAux2.nombreDeActor,nombreActor)==0){ //Mismo actor, solo guardo la película
			distancia_a_padre=distancia_a_padre+1;
			offsetAlPrincipal=offsetAlPrincipal+1;
			registroPrincipal.distancia_a_padre=distancia_a_padre;
			registroPrincipal.id=registroAux2.offsetPelicula;
			fwrite(&registroPrincipal,sizeof(es_ppal),1,archivoPrincipal);
		}
		else{	//Otro actor
			offset=offset+sizeof(size_t)+tamAux;
			distancia_a_padre=0;
			idAux=idAux+1;
			offsetAlPrincipal=offsetAlPrincipal+1;

			//Guarda el actor en el ppal
			registroPrincipal.distancia_a_padre=distancia_a_padre;
			registroPrincipal.id=idAux;
			fwrite(&registroPrincipal,sizeof(es_ppal),1,archivoPrincipal);

			//Guarda la 1er pelicula en el ppal
			distancia_a_padre=distancia_a_padre+1;
			registroPrincipal.distancia_a_padre=distancia_a_padre;
			registroPrincipal.id=registroAux2.offsetPelicula;
			fwrite(&registroPrincipal,sizeof(es_ppal),1,archivoPrincipal);

			//Guarda el actor en el indice
			registroIndice.offset_al_nombre=offset;
			registroIndice.offset_al_ppal=offsetAlPrincipal;
			registroIndice.profesion=registroAux2.profesion;
			fwrite(&registroIndice,sizeof(es_indice),1,archivoIndice);

			offsetAlPrincipal=offsetAlPrincipal+1;

			//Guarda el nombre en el archivo de concatenacion
			tamAux=strlen(registroAux2.nombreDeActor);
			free(nombreActor);
			nombreActor=(char*)malloc(tamAux+1);
			strcpy(nombreActor,registroAux2.nombreDeActor);
			fwrite(&tamAux,sizeof(size_t),1,archivoDeStrings);
			fwrite(nombreActor,tamAux,1,archivoDeStrings);
		}
	}
	offsetAlPrincipal=offsetAlPrincipal+2;
	fclose(archivoDeStrings);
	fclose(archivoPrincipal);
	fclose(archivoIndice);
	fclose(archivoAuxiliar2);

	//Acá se agregan los offsets entre películas
	archivoAuxiliar=fopen("archivoPrincipalAux","r+b");
	archivoPrincipal=fopen(this->n_arch_principal.c_str(),"w+b");

	es_ppal regRead;
	es_ppal regBusq;
	int idBuscado;
	int posicion;
	cout<<"Va a empezar con los offset al proximo"<<endl;
	while(!feof(archivoAuxiliar)){
		posicion=ftell(archivoAuxiliar);
		fread(&regRead,sizeof(es_ppal),1,archivoAuxiliar);
		if(feof(archivoAuxiliar)){
			break;
		}
		if(regRead.distancia_a_padre!=0){
			idBuscado=regRead.id;
			offset=0;
			do{
				fread(&regBusq,sizeof(es_ppal),1,archivoAuxiliar);
				if(feof(archivoAuxiliar)){
					offset=offset-offsetAlPrincipal;
					fseek(archivoAuxiliar,0,SEEK_SET);
				}
				offset=offset+1;
				if(regBusq.distancia_a_padre==0){
					regBusq.id=-1;	//No es una película, entonces le asigno un id no válido
				}
			} while (regBusq.id!=idBuscado);
			regRead.offset_proximo=offset;
			fseek(archivoAuxiliar,posicion+sizeof(es_ppal),SEEK_SET); //Voy al siguiente registro
		}													//del que estaba leyendo
		fwrite(&regRead,sizeof(es_ppal),1,archivoPrincipal);	//Escribo el reg en el principal
	}															//con el offset a la prox película o 0
	fclose(archivoPrincipal);
	fclose(archivoAuxiliar);
}

indice::indice(string nombre_arch){
	this->n_arch_indice=nombre_arch + ".idx";
	this->n_arch_conc_string=nombre_arch + "c" + ".conc";
	this->n_arch_principal=nombre_arch + "p" + ".ppal";

	//Recuperamos cantidad de staff
	this->archivoIndice=fopen(this->n_arch_indice.c_str(),"r+b");
	fseek(archivoIndice,0,SEEK_END);
	long int offset=ftell(archivoIndice);
	this->cantidad_staff=offset/sizeof(es_indice) -1;
	fseek(archivoIndice,0,SEEK_SET);
	this->archivoConc=fopen(this->n_arch_conc_string.c_str(),"r+b");
	this->archivoPpal=fopen(this->n_arch_principal.c_str(),"r+b");
	remove("auxiliar");
	remove("auxiliar2");
	remove("archivoPrincipalAux");
	for(unsigned int strCont=0;strCont<13;strCont++){
		string nombre="Temp_auxiliar2";
		ostringstream s;
		s<<strCont;
		nombre+=s.str();
		nombre+=".bin";
		remove(nombre.c_str());
	}
	remove("Merge.bin");
}

salidas indice::getID_staff(const string& nombre, int& id){

	//se realizaria la busqueda binaria en el indice con el nombre obtenido y se devuelve el offset en el indice mediante id y se retorna exito. Si no se encuentra se retorna error
	es_indice registro;
	FILE * archivo=this->archivoIndice;
	FILE * archivo_nombres=this->archivoConc;
	fseek(archivo,0,SEEK_SET);
	fseek(archivo_nombres,0,SEEK_SET);
	char * nombreAux=new char[1];
	size_t tamAux;
	int inicio=0;
	int final=this->cantidad_staff;
	int medio=final/2;
	while(true){
		fseek(archivo,medio*sizeof(es_indice),SEEK_SET);
		fread(&registro,sizeof(es_indice),1,archivo);
		fseek(archivo_nombres,registro.offset_al_nombre,SEEK_SET);
		fread(&tamAux,sizeof(size_t),1,archivo_nombres);
		delete []nombreAux;
		nombreAux=new char[tamAux + 1];
		nombreAux[tamAux]='\0';
		fread(nombreAux,tamAux,1,archivo_nombres);
		if(inicio>final){
			delete []nombreAux;
			return error;
		}
		if(strcmp(nombreAux,nombre.c_str())==0){
			id=medio;
			delete []nombreAux;
			return exito;
		}else if(strcmp(nombreAux,nombre.c_str())<0){
			inicio=medio + 1;
			medio=(final + inicio)/2;
		}else{
			final=medio - 1;
			medio=(final + inicio)/2;
		}
	}
}

salidas indice::getAllPeliculas(int ID_staff, list<int>& ID_peliculas){

	//se accede mediante el ID_staff a la posicion en indice y se obtiene la posicion en el archivo principal y se levanta con la estructura correspondiente que debe ser 0"ID_staff"0000 y se levantan todos los ids de las peliculas en forma secuencial
	//hasta que aparezca denuevo algo de la forma 0"id"0000 que quiere decir que ya llego a otro actor

	FILE * archivo_indice=this->archivoIndice;
	fseek(archivo_indice,0,SEEK_SET);
//	archivo_indice = fopen(this->n_arch_indice.c_str(),"r+b");

	if((!archivo_indice)or(ID_staff>this->cantidad_staff))
		return error;

	fseek(archivo_indice,ID_staff*sizeof(es_indice),SEEK_SET);

	es_indice staff;

	fread((void*)&staff,sizeof(es_indice),1,archivo_indice);

	FILE * archivo_ppal=this->archivoPpal;
	fseek(archivo_ppal,0,SEEK_SET);
//	archivo_ppal = fopen(this->n_arch_principal.c_str(),"r+b");

	if(!archivo_ppal){
		return error;}

	fseek(archivo_ppal,staff.offset_al_ppal*sizeof(es_ppal),SEEK_SET);

	es_ppal staff_aux,pelicula;

	//Leo el actor
	fread((void*)&staff_aux,sizeof(es_ppal),1,archivo_ppal);

	if(staff_aux.distancia_a_padre!=0){
		return error;}//porque quiere decir que no empezo levantando el actor por lo que esta mal

	//Leo la pelicula
	fread((void*)&pelicula,sizeof(es_ppal),1,archivo_ppal);

	while(pelicula.distancia_a_padre!=0){//veo que no levante un actor nuevo
		ID_peliculas.push_front(pelicula.id);
		fread((void*)&pelicula,sizeof(es_ppal),1,archivo_ppal);
		if(feof(archivo_ppal)) break;
	}

	return exito;
}

salidas indice::getAllStaff(int ID_pelicula,int Id_staff,list<int>& ID_staff ){

	//se accede mediante Id_staff a esa posicion que seria un actor que actuó en esa pelicula
	// se toma con la estructura correspondiente y se va al siguiente con el "offset al siguiente que trabajo en esa pelicula"
	//se repite esto hasta que se llegue denuevo al Id_staff con el que se empezó

	FILE * archivo_indice=this->archivoIndice;
	fseek(archivo_indice,0,SEEK_SET);
//	archivo_indice = fopen(this->n_arch_indice.c_str(),"r+b");

	if((!archivo_indice)or(Id_staff>this->cantidad_staff))
		return error;

	fseek(archivo_indice,Id_staff*sizeof(es_indice),SEEK_SET);

	es_indice staff_in;

	fread((void*)&staff_in,sizeof(es_indice),1,archivo_indice);

	FILE * archivo_ppal=this->archivoPpal;
	fseek(archivo_ppal,0,SEEK_SET);
//	archivo_ppal = fopen(this->n_arch_principal.c_str(),"r+b");

	if(!archivo_ppal){
		return error;}

	fseek(archivo_ppal,staff_in.offset_al_ppal*sizeof(es_ppal),SEEK_SET);

	es_ppal staff_aux,pelicula;

	//Leo el actor en el ppal
	fread((void*)&staff_aux,sizeof(es_ppal),1,archivo_ppal);

	if((staff_aux.distancia_a_padre!=0)or(staff_aux.id!=Id_staff)){
		return error;}//porque quiere decir que no empezo levantando el actor por lo que esta mal o que no es el id del staff realmente buscado

	//Leo la primer pelicula del actor
	fread((void*)&pelicula,sizeof(es_ppal),1,archivo_ppal);

	//Busco la pelicula en ese actor
	while(pelicula.id!=ID_pelicula){
		if(feof(archivo_ppal)) return error;
		fread((void*)&pelicula,sizeof(es_ppal),1,archivo_ppal);
		if(pelicula.distancia_a_padre==0) return error; //quiere decir que no tiene la pelicula
	}
	es_ppal staff_levantado;
	int offset_proximo=(pelicula.offset_proximo - 1)*sizeof(es_ppal);
	//vaya yendo a todos los offset al proximo hasta que vuelva al primero
	while(staff_levantado.id!=Id_staff){
		fseek(archivo_ppal,offset_proximo,SEEK_CUR);
		fread(&pelicula,sizeof(es_ppal),1,archivo_ppal);

		int v=pelicula.distancia_a_padre;

		fseek(archivo_ppal,(-v - 1)*sizeof(es_ppal),SEEK_CUR);
		fread((void*)&staff_levantado,sizeof(es_ppal),1,archivo_ppal);
		fseek(archivo_ppal,(v)*sizeof(es_ppal),SEEK_CUR);

		if(staff_levantado.distancia_a_padre!=0){
			return error;//quiere decirº que no llego a un id de actor

		}
//		list<int>::iterator iterador=ID_staff.begin();
//		while(iterador!=ID_staff.end()){
//			if((*iterador)==staff_levantado.id){
//				return exito;
//			}
//			iterador++;
//		}
		if(staff_levantado.id!=Id_staff){
			ID_staff.push_front(staff_levantado.id);
		}

		offset_proximo=(pelicula.offset_proximo - 1)*sizeof(es_ppal);

	}

	return exito;
}

salidas indice::getStaff(int ID_staff,staff& staff_d){

	//se accede mediante ID_staff al offset en el indice y se extrae la informacion

	FILE * archivo_indice=this->archivoIndice;
	fseek(archivo_indice,0,SEEK_SET);
//	archivo_indice = fopen(this->n_arch_indice.c_str(),"r+b");

	if((!archivo_indice)or(ID_staff>this->cantidad_staff)){
		return error;
	}

	fseek(archivo_indice,ID_staff*sizeof(es_indice),SEEK_SET);

	es_indice staff_aux;

	fread((void*)&staff_aux,sizeof(es_indice),1,archivo_indice);

	FILE * archivo_conc=this->archivoConc;
	fseek(archivo_conc,0,SEEK_SET);
//	archivo_conc = fopen(this->n_arch_conc_string.c_str(),"r+b");

	if(!archivo_conc){
//		fclose(archivo_indice);
		return error;
	}

	fseek(archivo_conc,staff_aux.offset_al_nombre,SEEK_SET);

	es_conc_nom nombre;
	fread((void*)&nombre.longitud,sizeof(nombre.longitud),1,archivo_conc);
	nombre.string=new char[nombre.longitud + 1];
	nombre.string[nombre.longitud]='\0';
	fread((void*)nombre.string,nombre.longitud,1,archivo_conc);

	staff aux(nombre.string,staff_aux.profesion);

	staff_d=aux;

//	fclose(archivo_conc);
//	fclose(archivo_indice);

	return exito;
}

salidas indice::getNombrePelicula (string& nombre, int ID_pelicula){

	//se accede mediante el ID_peliculas al archivo de concat de strings y se extrae el nombre

	FILE * archivo_conc=this->archivoConc;
	fseek(archivo_conc,0,SEEK_SET);
//	archivo_conc = fopen(this->n_arch_conc_string.c_str(),"r+b");

	if(!archivo_conc)
		return error;

	fseek(archivo_conc,ID_pelicula,SEEK_SET);

	es_conc_nom nombre_pel;

	fread((void*)&nombre_pel.longitud,sizeof(nombre_pel.longitud),1,archivo_conc);
	nombre_pel.string=new char[nombre_pel.longitud + 1];
	nombre_pel.string[nombre_pel.longitud]='\0';
	fread((void*)nombre_pel.string,nombre_pel.longitud*sizeof(char),1,archivo_conc);

	nombre=nombre_pel.string;

//	fclose(archivo_conc);

	return exito;
}
