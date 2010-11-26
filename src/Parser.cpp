#include "Parser.h"

parser::parser(const char* nombre) {
	this->archivoPeliculas = new ifstream(nombre);
}

list<pelicula>* parser::getPeliculasDeArchivo() {

	string* cadenaArchivo = new string;
	string* xml = new string;
	while (this->archivoPeliculas->eof() == false) {
		std::getline(*(archivoPeliculas), *cadenaArchivo);
		*xml += *cadenaArchivo;
	}
	delete cadenaArchivo;
	char* xmlAux = (char*) xml->c_str();
	this->archivoPeliculas->close();

	int cantStaff = 0;
	char* nombre;
//	char* nombreStaff;
//	char profesion;
	list<pelicula>* listaPeliculas = new list<pelicula>();
	list<staff>* listaStaff = new list<staff> ();

	char* buffer = strtok(xmlAux, "<>");
	while (strcmp(buffer, "/films") != 0 and buffer!=NULL) {
		if (strcmp(buffer, "film") == 0) {
			while (strcmp(buffer, "id") != 0) {
				buffer = strtok(NULL, "<>");
			}
			cantStaff = 0;
			buffer = strtok(NULL, "/");
			buffer = strtok(NULL, "/");
			buffer = strtok(NULL, "/");
			buffer = strtok(NULL, "/<");
//			cout << "nombre: " << buffer << endl;
			nombre = buffer;
			buffer = strtok(NULL, "<>");

			while ((strcmp(buffer, "/film") != 0)) {
				if (strcmp(buffer, "director") == 0) {
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/<");
					cantStaff++;
//					cout << "D: " << buffer << endl;
					listaStaff->push_back(staff(buffer,'D'));
				} else if (strcmp(buffer, "writer") == 0) {
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/<");
					cantStaff++;
//					cout << "W: " << buffer << endl;
					listaStaff->push_back(staff(buffer,'W'));
				} else if (strcmp(buffer, "actor") == 0) {
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/<");
					cantStaff++;
//					cout << "A: " << buffer << endl;
					listaStaff->push_back(staff(buffer,'A'));
				}
				buffer = strtok(NULL, "<>");
			}
			if (cantStaff != 0) {
				pelicula peli = pelicula(nombre, cantStaff);
				peli.setStaff(listaStaff);
				listaPeliculas->push_back(peli);
			}
		}
		buffer = strtok(NULL, "<>");
	}
//	cout << cantStaff << endl;
	delete xml;
	this->listaPeliculas = listaPeliculas;
	return listaPeliculas;
}

parser::~parser() {
	delete this->listaPeliculas;
	delete this->archivoPeliculas;
}
