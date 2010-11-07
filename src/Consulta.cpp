#include "Consulta.h"

//consulta::consulta(){
//}

list<peliculaHijo> consulta::staffHijos(int staffID){
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

list<peliculaHijo> consulta::caminoMinimoActores(int staffOrigen, int staffDestino, int gradoMax){
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
