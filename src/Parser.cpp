#include "Parser.h"

parser::parser(const char* nombre) {
	this->archivoPeliculas = new ifstream(nombre);
	this->decodificador = "%01\x01%02\x02%03\x03%04\x04%05\x05%06\x06%07\x07%08\x08%09\x09%0A\x0A%0B\x0B%0C\x0C%0D\x0D%0E\x0E%0F\x0F"
			"%10\x10%11\x11%12\x12%13\x13%14\x14%15\x15%16\x16%17\x17%18\x18%19\x19%1A\x1A%1B\x1B%1C\x1C%1D\x1D%1E\x1E%1F\x1F"
			"%20\x20%21\x21%22\x22%23\x23%24\x24%25\x25%26\x26%27\x27%28\x28%29\x29%2A\x2A%2B\x2B%2C\x2C%2D\x2D%2E\x2E%2F\x2F"
			"%30\x30%31\x31%32\x32%33\x33%34\x34%35\x35%36\x36%37\x37%38\x38%39\x39%3A\x3A%3B\x3B%3C\x3C%3D\x3D%3E\x3E%3F\x3F"
			"%40\x40%41\x41%42\x42%43\x43%44\x44%45\x45%46\x46%47\x47%48\x48%49\x49%4A\x4A%4B\x4B%4C\x4C%4D\x4D%4E\x4E%4F\x4F"
			"%50\x50%51\x51%52\x52%53\x53%54\x54%55\x55%56\x56%57\x57%58\x58%59\x59%5A\x5A%5B\x5B%5C\x5C%5D\x5D%5E\x5E%5F\x5F"
			"%60\x60%61\x61%62\x62%63\x63%64\x64%65\x65%66\x66%67\x67%68\x68%69\x69%6A\x6A%6B\x6B%6C\x6C%6D\x6D%6E\x6E%6F\x6F"
			"%70\x70%71\x71%72\x72%73\x73%74\x74%75\x75%76\x76%77\x77%78\x78%79\x79%7A\x7A%7B\x7B%7C\x7C%7D\x7D%7E\x7E%7F\x7F"
			"%80\x80%81\x81%82\x82%83\x83%84\x84%85\x85%86\x86%87\x87%88\x88%89\x89%8A\x8A%8B\x8B%8C\x8C%8D\x8D%8E\x8E%8F\x8F"
			"%90\x90%91\x91%92\x92%93\x93%94\x94%95\x95%96\x96%97\x97%98\x98%99\x99%9A\x9A%9B\x9B%9C\x9C%9D\x9D%9E\x9E%9F\x9F"
			"%A0\xA0%A1\xA1%A2\xA2%A3\xA3%A4\xA4%A5\xA5%A6\xA6%A7\xA7%A8\xA8%A9\xA9%AA\xAA%AB\xAB%AC\xAC%AD\xAD%AE\xAE%AF\xAF"
			"%B0\xB0%B1\xB1%B2\xB2%B3\xB3%B4\xB4%B5\xB5%B6\xB6%B7\xB7%B8\xB8%B9\xB9%BA\xBA%BB\xBB%BC\xBC%BD\xBD%BE\xBE%BF\xBF"
			"%C0\xC0%C1\xC1%C2\xC2%C3\xC3%C4\xC4%C5\xC5%C6\xC6%C7\xC7%C8\xC8%C9\xC9%CA\xCA%CB\xCB%CC\xCC%CD\xCD%CE\xCE%CF\xCF"
			"%D0\xD0%D1\xD1%D2\xD2%D3\xD3%D4\xD4%D5\xD5%D6\xD6%D7\xD7%D8\xD8%D9\xD9%DA\xDA%DB\xDB%DC\xDC%DD\xDD%DE\xDE%DF\xDF"
			"%E0\xE0%E1\xE1%E2\xE2%E3\xE3%E4\xE4%E5\xE5%E6\xE6%E7\xE7%E8\xE8%E9\xE9%EA\xEA%EB\xEB%EC\xEC%ED\xED%EE\xEE%EF\xEF"
			"%F0\xF0%F1\xF1%F2\xF2%F3\xF3%F4\xF4%F5\xF5%F6\xF6%F7\xF7%F8\xF8%F9\xF9%FA\xFA%FB\xFB%FC\xFC%FD\xFD%FE\xFE%FF\xFF";
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
	found = nombre2.find("%23");
	if (found < nombre2.size() and found != string::npos)
		nombre2 = nombre2.substr(0, found);
	found = nombre2.find("%");
	if(found < nombre2.size() and found != string::npos)
		nombre2 = this->decodificar(nombre2);
	return nombre2;
}
string parser::decodificar(string palabraADecodificar) {
	string resultado = palabraADecodificar;
	string clave = "%";
	string parcial;
	size_t t, t1, t2;
	t = resultado.find(clave);
	while (t < resultado.size() and t != string::npos) {
		if (resultado[t + 1] == 'C' or resultado[t + 1] == 'D') {
			clave = resultado.substr(t, 3);
			t1 = decodificador.find(clave);
			clave = resultado.substr(t + 3, 3);
			t2 = decodificador.find(clave);
			parcial = decodificador.substr(t1 + 3, 1);
			parcial += decodificador.substr(t2 + 3, 1);
			resultado.replace(t, 6, parcial);
		} else {
			clave = resultado.substr(t, 3);
			t1 = decodificador.find(clave);
			parcial = decodificador.substr(t1 + 3, 1);
			resultado.replace(t, 3, parcial);
		}
		t = resultado.find_first_of("%", t + 1);
	}
	return resultado;
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
