#include <stdlib.h>
#include "Ordenamiento.h"

replac_selection::replac_selection(int buffTam, int dataTam,void* (*constructor)(char*),
				void (*destructor) (void*),int (*comp) (void*,void*),
				void (*persistencia) (ofstream&, void*)) {

	this->dataTam = dataTam;
	this->buffTam = buffTam;
	this->v_elem = new T_elem[buffTam];

	//Inicializo el vector
	for (int i=0;i < buffTam; i++) {
		v_elem[i].elem = NULL;
		v_elem[i].est = vacio;
	}

	this->persistencia = persistencia;
	this->destructor = destructor;
	this->comp = comp;
	this->constructor = constructor;
}

/* Param:
 * --string: nombre arch entrada y salida
 * */
void replac_selection::ordenar (const string& arch_e) {
	stringstream ss;
	ifstream entrada;
	ofstream salida;
	string n_arch = arch_e;
	entrada.open(n_arch.c_str(), ios::in | ios::binary);

	if (!entrada.is_open()){
		cout << "ARCHIVO NO ENCONTRADO" << endl;
		return;
	}


	int cantElem = completarArray(entrada);
	int cantCong = 0; // contador de elem congelados
	int contArch = 0;//contador de particiones
	void * aux,* min;

	aux = min = NULL;

	int posMin = 0;

	ss << "Temp_" << arch_e << contArch << ".bin";
	ss >> n_arch;
	cout << n_arch << endl;
	salida.open (n_arch.c_str(), ios::out | ios::binary);

	char stream [dataTam+1];
	entrada.read(stream,dataTam); // obtiene un char* del tamaño del elemento

		while (!entrada.eof() || cantElem != 0  ){

			if(cantCong == cantElem ) {// si se debe iniciar otra particion
				stringstream ss1;
				salida.close();
				contArch++;
				if (contArch > 15) exit(0);
				ss1 << "Temp_" << arch_e << contArch << ".bin";
				ss1 >> n_arch;
				cout << n_arch << endl;

				salida.open(n_arch.c_str(), ios::out | ios::binary);
				descongelarElem(buffTam);
				cantCong = 0;
			}

			aux = NULL;


			stream [dataTam]= '\0';
			if (!entrada.eof())
				aux = constructor (stream);

			posMin = elemMasPequenio(buffTam);

			//TODO ESTO HAY QUE BORRARLO <<<<<
			int k;
			cout << "\nVECTOR" << endl;
			if (!entrada.eof()) {
				int nro = atoi(stream);
				cout << "Numero para insertar: " << nro << endl;
			}
			else
				cout << "no hay mas numeros" << endl;
			for (int q =0 ; q < buffTam ; q++) {
				if ( v_elem [q].est != vacio)
					k = *((int *) v_elem [q].elem);
				if (v_elem [q].est == ocupado)
					cout << "El elem  n°" << q+1 << " es " << k << endl;
				else if (v_elem [q].est == congelado)
					cout << "El elem  n°" << q+1 << " es " << k << " (congelado)" << endl;
				else
					cout << "El elem  n°" << q+1 << " (vacio)" <<endl;
			}
			k = *((int *) v_elem [posMin].elem);
			cout << "El Minimo es: " << k << endl << endl;
			// >>>>>>

			min = v_elem [posMin].elem;
			persistencia(salida,min);// almaceno el elemento mas pequeño del vector

			if (aux == NULL) {
				v_elem[posMin].est = vacio;
				cantElem--;
			}
			else {//si el elemento tiene una clave menor que el ultimo insertado
				int j = comp (v_elem[posMin].elem, aux);
				v_elem[posMin].elem = aux;

				if (j < 1){ // aux menor a clave insertada
					v_elem[posMin].est = congelado;
					cantCong++;//aumenta la cantidad de congelados
				}
			}
			destructor(min);
			entrada.read(stream,dataTam); // obtiene un char* del tamaño del elemento



		}//while

		if (salida.is_open())
			salida.close();
		entrada.close();

}


void replac_selection::descongelarElem(int cant_elem) {
	for (int i = 0; i < cant_elem; i++) {
		if (v_elem[i].est == congelado)
			v_elem[i].est = ocupado;
	}
}


/*Retorna la posición en el vector del elemento mas pequeño*/
int replac_selection::elemMasPequenio(int cant_elem) {
	int cont = 0;
	bool enc = false;
	int posMin = 0;
	void * min = NULL;

	/* busco el primer elemento del vector */
	while (cont < cant_elem && !enc) {
		if (v_elem[cont].est == ocupado) {
			min = v_elem [cont].elem;
			posMin = cont;
			enc = true;
		}
		else
			cont++;
	}

	/*Encuentro el elemento mas pequeño que no está congelado*/
	for (int i = cont+1; i < cant_elem ; i++) {
		int j = 1;//para que no cambie el minimo en caso de que
				   //el actual este congelado o vacio
		if (v_elem[i].est == ocupado)
			j = comp(min,v_elem[i].elem);

		if (j == -1){// si el min actual es mas grande
			min = v_elem[i].elem;
			posMin = i;
		}
	}//for

	return posMin;
}

/*Busca completar lo maximo posible el vector, y retorna la cantidad de elementos*/
int replac_selection::completarArray(ifstream& e) {

	int cantElem = 0;

	char  stream [dataTam+1];




	while ( cantElem < buffTam && e.read(stream,dataTam)){

		stream [dataTam]= '\0';
		v_elem [cantElem].elem = constructor (stream);
		v_elem [cantElem].est = ocupado;
		cantElem ++;


	}
	cout << "sale con el numero " << atoi(stream) << endl;

	return cantElem;
}


