#include "Consulta.h"

Consulta::Consulta(bool preConsulta){
	tieneArchivoPreConsulta=preConsulta;
	index= new indice("TpDatos");
	if(tieneArchivoPreConsulta){
		fstream* archivo2 = new fstream;
		archivo2->open("preconsulta.bin",ios::in|ios::out|ios::binary);
		bool existeArchivo=archivo2->good();
		archivo2->close();
		delete archivo2;
		if(!existeArchivo){
			armarArchivoPreConsulta(GRADOMAX);
		} else {
			this->archivoConsulta = new ArchivoPreConsulta(CANTVERTICES,false);
		}
	}
}

list<peliculaHijo> Consulta::staffHijos(int staffID){
	list<peliculaHijo> lista;
	list<int> peliculas;
	salidas as=index->getAllPeliculas(staffID,peliculas);
//	if(as==error)
//		cout<<"error uh"<<endl;
	list<int>::iterator iter = peliculas.begin();
	while(iter!=peliculas.end()){
		list<int> actores;
		salidas as=index->getAllStaff((*iter),staffID,actores);
//		if(as==error) cout<<"error inin"<<endl;
		list<int>::iterator iterador=actores.begin();
		while(iterador!=actores.end()){
			peliculaHijo aux;
			aux.pelicula=(*iter);
			aux.hijo=(*iterador);
			lista.push_back(aux);
			iterador++;
		}
		actores.clear();
		iter++;
	}
	peliculas.clear();
	return lista;
}

list<peliculaHijo> Consulta::caminoMinimoActores(int staffOrigen, int staffDestino, int gradoMax){
	datosStaff tabla[CANTVERTICES];
	for(int i=0; i<CANTVERTICES; i++){
		tabla[i].conocido=0;
		tabla[i].padre=0;
		tabla[i].pelicula=0;
		if(i==staffOrigen){
			tabla[i].distancia=0;
		} else {
			tabla[i].distancia=gradoMax+1;
		}
	}
	list<peliculaHijo> verticesHijos;
	for(int distancia=0; distancia<gradoMax; distancia++){
		for(int vert=0; vert<CANTVERTICES; vert++){
			if(tabla[vert].conocido==0 && tabla[vert].distancia==distancia){
				tabla[vert].conocido=1;
				verticesHijos=staffHijos(vert);
				list<peliculaHijo>::iterator iter;
				for(iter=verticesHijos.begin(); iter!=verticesHijos.end(); iter++){
					int numero=(*iter).hijo;
					if(tabla[numero].distancia>gradoMax){
						tabla[numero].distancia=distancia+1;
						tabla[numero].padre=vert;
						tabla[numero].pelicula=(*iter).pelicula;
					}
					if(numero==staffDestino){
						list<peliculaHijo> listaAux;
						peliculaHijo peliculaHijoAux;
						int num=numero;
						while(num!=staffOrigen){
							peliculaHijoAux.hijo=num;
							peliculaHijoAux.pelicula=tabla[num].pelicula;
							listaAux.push_front(peliculaHijoAux);
							num=tabla[num].padre;
						}
						peliculaHijoAux.hijo=num;
						peliculaHijoAux.pelicula=0;
						listaAux.push_front(peliculaHijoAux);
						return listaAux;
					}
				}
			}
		}
	}
	list<peliculaHijo> listaAux;
	return listaAux;
}

list<padrePeliculaHijo> Consulta::actoresHijos(int staffOrigen, int gradoMax){
	datosStaff tabla[CANTVERTICES];
	for(int i=0; i<CANTVERTICES; i++){
		tabla[i].conocido=0;
		tabla[i].padre=0;
		tabla[i].pelicula=0;
		if(i==staffOrigen){
			tabla[i].distancia=0;
		} else {
			tabla[i].distancia=gradoMax+1;
		}
	}
	list<peliculaHijo> verticesHijos;
	for(int distancia=0; distancia<gradoMax; distancia++){
		for(int vert=0; vert<CANTVERTICES; vert++){
			if(tabla[vert].conocido==0 && tabla[vert].distancia==distancia){
				tabla[vert].conocido=1;
				verticesHijos=staffHijos(vert);
				list<peliculaHijo>::iterator iter;
				for(iter=verticesHijos.begin(); iter!=verticesHijos.end(); iter++){
					int numero=(*iter).hijo;
					if(tabla[numero].distancia>gradoMax){
						tabla[numero].distancia=distancia+1;
						tabla[numero].padre=vert;
						tabla[numero].pelicula=(*iter).pelicula;
					}
				}
			}
		}
	}
	list<padrePeliculaHijo> listaAux;
	for(int i=0;i<CANTVERTICES;i++){
		if(tabla[i].distancia<=gradoMax){
			padrePeliculaHijo aux;
			aux.padre=tabla[i].padre;
			aux.pelicula=tabla[i].pelicula;
			aux.hijo=i;
			aux.distancia=tabla[i].distancia;
			listaAux.push_back(aux);
		}
	}
	return listaAux;
}

bool Consulta::armarArchivoPreConsulta(int gradoMax){
	this->archivoConsulta = new ArchivoPreConsulta(CANTVERTICES,true);
	for(int i=1;i<=CANTVERTICES;i++){
		cout << i << endl;
		if(!archivoConsulta->agregarHijosActor(i,actoresHijos(i,gradoMax))) return false;
	}
}

void Consulta::imprimirCaminoMinimoActores(int actorOrigen, int actorDestino){
	if(tieneArchivoPreConsulta){
		list<padrePeliculaHijo> lista = this->archivoConsulta->CaminoActorHijo(actorOrigen,actorDestino);
		list<padrePeliculaHijo>::iterator iter = lista.begin();
		if(lista.size()>0){
			staff actor("asd",'c');
			salidas as=index->getStaff((*iter).hijo,actor);
			cout << "Actor\t" << actor.getNombre() << endl;
			iter++;
			while(iter!=lista.end()){
				staff actor("asd",'c');
				salidas as=index->getStaff((*iter).hijo,actor);
				cout << "Pelicula\t" << (*iter).pelicula << endl << "Actor\t" << actor.getNombre() << endl;
				iter++;
			}
		}
	} else {
		list<peliculaHijo> lista= caminoMinimoActores(actorOrigen,actorDestino,GRADOMAX);
		list<peliculaHijo>::iterator iter = lista.begin();
		if(lista.size()>0){
			staff actor("asd",'c');
			salidas as=index->getStaff((*iter).hijo,actor);
			cout << "Actor\t" << actor.getNombre() << endl;
			iter++;
			while(iter!=lista.end()){
				as=index->getStaff((*iter).hijo,actor);
				string nombrePelicula;
				as=index->getNombrePelicula(nombrePelicula,(*iter).pelicula);
				cout << "Este trabajo en " << nombrePelicula << " con " << actor.getNombre() << " (" << actor.getProfesion() << ")" << endl;
				iter++;
			}
		}
	}
}

void Consulta::imprimirCaminoMinimoActoresNombre(string actorOrigen, string actorDestino){
	int origenID;
	int destinoID;
	bool cerrar=false;
	salidas as= index->getID_staff(actorOrigen,origenID);
	if(as==error){
		cout<<"El actor origen no existe"<<endl;
		cerrar=true;
	}
	as= index->getID_staff(actorDestino,destinoID);
	if(as==error){
		cout<<"El actor destino no existe"<<endl;
		cerrar=true;
	}
	if(cerrar) exit(0);
	imprimirCaminoMinimoActores(origenID,destinoID);
}

void Consulta::imprimirActoresADistancia(int actorOrigen, char distancia){
	if(tieneArchivoPreConsulta){
		list<padrePeliculaHijo> lista = this->archivoConsulta->ActoresHijosADistancia(actorOrigen,distancia);
		list<padrePeliculaHijo>::iterator iter = lista.begin();
		int i=0;
		while(iter!=lista.end()){
			staff actor("asd",'c');
			if((*iter).distancia==distancia){
				salidas as=index->getStaff((*iter).hijo,actor);
				if(as!=error){
					string nombrePelicula;
					as=index->getNombrePelicula(nombrePelicula,(*iter).pelicula);
					cout << i << "- Pelicula\t" << nombrePelicula << endl << "\tActor\t" << actor.getNombre() << " (" << actor.getProfesion() << ")" << endl;
					i++;
				}
			}
			iter++;
		}
	} else {
		list<padrePeliculaHijo> lista = actoresHijos(actorOrigen,distancia);
		list<padrePeliculaHijo>::iterator iter = lista.begin();
		int i=0;
		while(iter!=lista.end()){
			staff actor("asd",'c');
			if((*iter).distancia==distancia){
				salidas as=index->getStaff((*iter).hijo,actor);
				if(as!=error){
					string nombrePelicula;
					as=index->getNombrePelicula(nombrePelicula,(*iter).pelicula);
					cout << i <<" - "<< actor.getNombre() << " (" << actor.getProfesion() << ")" << endl;
					i++;
				}
			}
			iter++;
		}
	}
}

void Consulta::imprimirActoresADistanciaNombre(string actorOrigen, char distancia){
	int origenID;
	salidas as= index->getID_staff(actorOrigen,origenID);
	if(as==error){
		cout<<"El actor no existe"<<endl;
		exit(0);
	}
	imprimirActoresADistancia(origenID,distancia);
}

Consulta::~Consulta() {
	if(tieneArchivoPreConsulta) delete archivoConsulta;
	delete index;
}
