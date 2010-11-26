#include <iostream>
using namespace std;
#include "Indice.h"

int main(int argc,char* argv[]){
	switch(argc){
		case(2):{
			if(strcmp(argv[1],"Generar")==0){
				cout<<"Se generara el indice"<<endl;
				list<string> nombresDeArchivo;
				string nombreIndice="TpDatos";
				indice indice(nombresDeArchivo,nombreIndice);
			}else{
				cout<<"Parametro incorrecto"<<endl;
			}
			break;
		}
		case(5):{
			indice indice("TpDatos");
			if(strcmp(argv[1],"--from")==0){
				if(strcmp(argv[3],"--to")==0){

				}else if(strcmp(argv[3],"--degree")==0){

				}else{
					cout<<"3er parametro incorrecto debe ser \"--to\" o \"--degree\""<<endl;
				}
			}else{
			      cout<<"El primer parametro no es \"--from\""<<endl;
			}
			break;
		}
		default:{
			cout<<"Cantidad de parametros incorrecta"<<endl;
		}
	}
	return 0;
}
