#include <iostream>
using namespace std;
#include "Consulta.h"

int main(int argc,char* argv[]){
	switch(argc){
		case(2):{
			if(strcmp(argv[1],"Generar")==0){
				cout<<"Se generara el indice"<<endl;
				list<string> nombresDeArchivo;
				nombresDeArchivo.push_front(string("data-0.xml"));
				string nombreIndice="TpDatos";
				indice indice(nombresDeArchivo,nombreIndice);
			}else if(strcmp(argv[1],"auxiliar")==0){
				FILE* file;
				registroAuxiliar reg;

				file=fopen("auxiliar","r+b");
				while (!feof(file)){
					fread(&reg,sizeof(registroAuxiliar),1,file);
					if(feof(file)){
						break;
					}
					printf("actor: %s pelicula: %s profesion: %c \n",reg.nombreDeActor,reg.nombreDePelicula,reg.profesion);
				}

				fclose(file);
				break;
			}else if(strcmp(argv[1],"auxiliar2")==0){
				FILE* file2;
				registroAuxiliar2 reg2;

				file2=fopen("auxiliar2","r+b");
				while (!feof(file2)){
					fread(&reg2,sizeof(registroAuxiliar2),1,file2);
					if(feof(file2)){
						break;
					}
					printf("actor: %s pelicula: %d profesion: %c \n",reg2.nombreDeActor,reg2.offsetPelicula,reg2.profesion);
				}

				fclose(file2);
				break;
			}else if(strcmp(argv[1],"concatenacion")==0){
				FILE* file3;
				size_t size;
				char* nombrePelicula;

				file3=fopen("TpDatosc.conc","r+b");
				while (!feof(file3)){
					fread(&size,sizeof(size_t),1,file3);
					if(feof(file3)){
						break;
					}
					nombrePelicula=(char*)malloc(size);
					fread(nombrePelicula,size,1,file3);
					printf("Cant caracteres: %d Nombre: %s \n",size,nombrePelicula);
				}
				printf("Tam size_t: %d\n",sizeof(size_t));
				fclose(file3);
				break;

			}else{
				cout<<"Parametro incorrecto"<<endl;
			}
			break;
		}
		case(3):{
			if(strcmp(argv[1],"1")==0){
				FILE* file2;
				registroAuxiliar2 reg2;

				file2=fopen(argv[2],"r+b");
				while (!feof(file2)){
					fread(&reg2,sizeof(registroAuxiliar2),1,file2);
					if(feof(file2)){
						break;
					}
					printf("actor: %s pelicula: %d profesion: %c \n",reg2.nombreDeActor,reg2.offsetPelicula,reg2.profesion);
				}

				fclose(file2);
				break;
			}else if(strcmp(argv[1],"2")==0){
				FILE* file2;
				es_indice reg2;

				file2=fopen(argv[2],"r+b");
				while (!feof(file2)){
					fread(&reg2,sizeof(es_indice),1,file2);
					if(feof(file2)){
						break;
					}
					printf("offsetalnombre: %d offsetalppal: %d profesion: %c \n",reg2.offset_al_nombre,reg2.offset_al_ppal,reg2.profesion);
				}

				fclose(file2);
				break;
			}else if(strcmp(argv[1],"3")==0){
				FILE* file2;
				es_ppal reg2;

				file2=fopen(argv[2],"r+b");
				while (!feof(file2)){
					fread(&reg2,sizeof(es_ppal),1,file2);
					if(feof(file2)){
						break;
					}
					printf("distanciaPadre: %d id: %d offsetProximo: %d \n",reg2.distancia_a_padre,reg2.id,reg2.offset_proximo);
				}

				fclose(file2);
				break;
			}else{
				cout<<"El parametro correspondiente al numero no es valido"<<endl;
				break;
			}
			break;
		}
		case(5):{
			if(strcmp(argv[1],"--from")==0){
				if(strcmp(argv[3],"--to")==0){
					Consulta consulta(false);
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
