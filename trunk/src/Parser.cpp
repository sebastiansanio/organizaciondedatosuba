#include "Parser.h"

parser::parser(const char* nombre) {
	this->archivoPeliculas = new ifstream(nombre);
}

list<pelicula*> parser::getPeliculasDeArchivo() {

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
	char profesion;
	list<pelicula*> listaPeliculas;// = new list<pelicula>();

	char* buffer = strtok(xmlAux, "<>");
	while (strcmp(buffer, "/films") != 0 and buffer != NULL) {
		if (strcmp(buffer, "film") == 0) {
			while (strcmp(buffer, "id") != 0) {
				buffer = strtok(NULL, "<>");
			}
			buffer = strtok(NULL, "/");
			buffer = strtok(NULL, "/");
			buffer = strtok(NULL, "/");
			buffer = strtok(NULL, "/<");
			nombre = buffer;
			cantStaff = 0;
			buffer = strtok(NULL, "<>");
			pelicula* peli = new pelicula(nombre, cantStaff);
			while ((strcmp(buffer, "/film") != 0)) {
				if (strcmp(buffer, "director") == 0) {
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/<");
					cantStaff++;
					peli->addStaff(new staff(buffer, 'D'));
				} else if (strcmp(buffer, "writer") == 0) {
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/<");
					cantStaff++;
					peli->addStaff(new staff(buffer, 'W'));
				} else if (strcmp(buffer, "actor") == 0) {
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/<");
					cantStaff++;
					peli->addStaff(new staff(buffer, 'A'));
				}
				buffer = strtok(NULL, "<>");
			}
			if (cantStaff != 0) {
				peli->setCantStaff(cantStaff);
				listaPeliculas.push_back(peli);
			}
		}
		buffer = strtok(NULL, "<>");
	}
	delete xml;
	this->listaPeliculas = listaPeliculas;
	return listaPeliculas;
}

parser::~parser() {
	delete this->archivoPeliculas;
}
