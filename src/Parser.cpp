#include "Parser.h"

parser::parser(const char* nombre) {
	this->archivoPeliculas = new ifstream(nombre);
}

string parser::limpiarNombre(string nombreALimpiar){
	size_t found, found2;
	string nombre = nombreALimpiar.substr(0, nombreALimpiar.size()) + "\0";
	found2 = nombre.find("_");
	while (found2 < nombre.size() and found2 != string::npos) {
		nombre.replace(found2, 1, " ");
		found2 = nombre.find_first_of("_", found2 + 1);
	}
	string nombre2 = nombre;
	found = nombre2.find("%28");
	if (found < nombre2.size() and found != string::npos)
		nombre2 = nombre2.substr(0, found - 1);
	return nombre2;
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
			cantStaff = 0;
			string nombrepeli(buffer);
			nombrepeli = this->limpiarNombre(nombrepeli);
			pelicula* peli = new pelicula(nombrepeli, cantStaff);
			buffer = strtok(NULL, "<>");
			while ((strcmp(buffer, "/film") != 0)) {
				if (strcmp(buffer, "director") == 0) {
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/<");
					cantStaff++;
					string nombrestaff(buffer);
					nombrestaff = this->limpiarNombre(nombrestaff);
					peli->addStaff(new staff(nombrestaff, 'D'));
				} else if (strcmp(buffer, "writer") == 0) {
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/<");
					cantStaff++;
					string nombrestaff(buffer);
					nombrestaff = this->limpiarNombre(nombrestaff);
					peli->addStaff(new staff(nombrestaff, 'W'));
				} else if (strcmp(buffer, "actor") == 0) {
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/");
					buffer = strtok(NULL, "/<");
					cantStaff++;
					string nombrestaff(buffer);
					nombrestaff = this->limpiarNombre(nombrestaff);
					peli->addStaff(new staff(nombrestaff, 'A'));
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
