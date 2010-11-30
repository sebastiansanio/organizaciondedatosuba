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
	int id;
	salidas as=index->getID_staff("Nicolas Cage",id);
	if(as==error)
		cout<<"error"<<endl;
	cout<<"Id de Nicolas Cage " << id << endl;
	list<int> peliculas;
	as=index->getAllPeliculas(id,peliculas);
	if(as==error)
		cout<<"error"<<endl;
	list<int>::iterator iter = peliculas.begin();
	string nombre;
	cout<<"\nTrabajo en las siguientes peliculas:"<<endl;
	while(iter!=peliculas.end()){
		cout << "Codigo: "<<(*iter)<< " ";
		index->getNombrePelicula(nombre,(*iter));
		cout<<"Nombre: "<<nombre<<endl;
		iter++;
	}
	peliculas.clear();
	cout<<endl<<"\nTraemos todos los actores de una determinada pelicula:"<<endl;
	as=index->getAllStaff(4331,1060,peliculas);
	iter = peliculas.begin();
	while(iter!=peliculas.end()){
		cout << (*iter) << endl;
		iter++;
	}
	if(as==error){
		cout<<"error en traer todos los staff"<<endl;
	}
	cout<<"\nSe buscara una pelicula por id"<<endl;
	string nombrePelicula;
	index->getNombrePelicula(nombrePelicula,4331);
	cout<<"Pelicula: "<<nombrePelicula<<endl;
	cout<<"\nSe buscara un actor por id"<<endl;
	staff actor("asd",'c');
	index->getStaff(id,actor);
	cout<<"Actor id = "<<id<<" Nombre: "<<actor.getNombre()<<endl;
}

list<peliculaHijo> Consulta::staffHijos(int staffID){
	list<peliculaHijo> lista;
	peliculaHijo aux;
	if(staffID==1){
		aux={2,1};
		lista.push_back(aux);
		aux={3,4};
		lista.push_back(aux);
	} else if (staffID==2){
		aux={1,1};
		lista.push_back(aux);
		aux={3,5};
		lista.push_back(aux);
		aux={10,2};
		lista.push_back(aux);
		aux={0,3};
		lista.push_back(aux);
	} else if (staffID==3){
		aux={1,4};
		lista.push_back(aux);
		aux={2,5};
		lista.push_back(aux);
		aux={4,6};
		lista.push_back(aux);
		aux={5,16};
		lista.push_back(aux);
		aux={7,15};
		lista.push_back(aux);
	} else if (staffID==4){
		aux={3,6};
		lista.push_back(aux);
		aux={9,9};
		lista.push_back(aux);
		aux={10,7};
		lista.push_back(aux);
	} else if (staffID==5){
		aux={3,16};
		lista.push_back(aux);
		aux={7,13};
		lista.push_back(aux);
		aux={0,17};
		lista.push_back(aux);
	} else if (staffID==6){
		aux={9,10};
		lista.push_back(aux);
		aux={8,11};
		lista.push_back(aux);
	} else if (staffID==7){
		aux={3,15};
		lista.push_back(aux);
		aux={5,13};
		lista.push_back(aux);
		aux={9,14};
		lista.push_back(aux);
	} else if (staffID==8){
		aux={6,11};
		lista.push_back(aux);
		aux={10,8};
		lista.push_back(aux);
		aux={0,12};
		lista.push_back(aux);
	} else if (staffID==9){
		aux={4,9};
		lista.push_back(aux);
		aux={6,10};
		lista.push_back(aux);
		aux={7,14};
		lista.push_back(aux);
	} else if (staffID==10){
		aux={2,2};
		lista.push_back(aux);
		aux={4,7};
		lista.push_back(aux);
		aux={8,8};
		lista.push_back(aux);
	} else if (staffID==0){
		aux={2,3};
		lista.push_back(aux);
		aux={5,17};
		lista.push_back(aux);
		aux={8,12};
		lista.push_back(aux);
	}
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
		cout<<"Distancia: "<<distancia<<endl;
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
		cout<<"Distancia: "<<distancia<<endl;
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
		if(!archivoConsulta->agregarHijosActor(i,actoresHijos(i,gradoMax))) return false;
	}
}

void Consulta::imprimirCaminoMinimoActores(int actorOrigen, int actorDestino){
	if(tieneArchivoPreConsulta){
		list<padrePeliculaHijo> lista = this->archivoConsulta->CaminoActorHijo(actorOrigen,actorDestino);
		list<padrePeliculaHijo>::iterator iter = lista.begin();
		while(iter!=lista.end()){
			cout << "Pelicula " << (*iter).pelicula << " Actor " << (*iter).hijo << endl;
			iter++;
		}
	} else {
		list<peliculaHijo> lista= caminoMinimoActores(actorOrigen,actorDestino,6);
		list<peliculaHijo>::iterator iter = lista.begin();
		while(iter!=lista.end()){
			cout << "Pelicula " << (*iter).pelicula << " Actor " << (*iter).hijo << endl;
			iter++;
		}
	}
}

void Consulta::imprimirActoresADistancia(int actorOrigen, char distancia){
	if(tieneArchivoPreConsulta){
		list<padrePeliculaHijo> lista = this->archivoConsulta->ActoresHijosADistancia(actorOrigen,distancia);
		list<padrePeliculaHijo>::iterator iter = lista.begin();
		while(iter!=lista.end()){
			cout << "Pelicula " << (*iter).pelicula << " Actor " << (*iter).hijo << " Distancia " << (int)(*iter).distancia << endl;
			iter++;
		}
	}
}

Consulta::~Consulta() {
	if(tieneArchivoPreConsulta) delete archivoConsulta;
	delete index;
}
