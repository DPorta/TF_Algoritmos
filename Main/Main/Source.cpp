#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include "Arbol.h"

using namespace std;
using namespace std::experimental::filesystem;
using namespace std::chrono_literals;

class File {
	string name, extension, date, fpath;
	long long size;
public:
	File(string name, string extension, string date, long long size, string fpath)
		: name(name), extension(extension), date(date), size(size), fpath(fpath) {}

	string getName() { return name; }
	string getExtension() { return extension; }
	string getDate() { return date; }
	string getFpath() { return fpath; }  //para poder retornar el absolute path/full path de los archivos
	long long getSize() { return size; }


	void setname(string name) { this->name = name; }
	void setextension(string extension) { this->extension = extension; }
	void setdate(string date) { this->date = date; }
	void setfpath(string fpath) { this->fpath = fpath; }
	void setsize(long long size) { this->size = size; }
};

void MostrarMenu()
{
	cout << "Menu\n";
	cout << "Opciones: \n";
	cout << "1 - Buscar archivos\n";
	cout << "2 - Filtrar archivos\n";
	cout << "3 - Ordenar archivos\n";
	cout << "0 - Salir\n";
	cout << "Ingrese su opcion: ";
}

void MostrarMenuBuscarArchivos()
{
	cout << "Opciones: \n";
	cout << "1 - Nombre \n";
	cout << "2 - Extension \n";
	cout << "3 - Tamano \n";
	cout << "0 - Volver al MENU \n";

}

void MostrarMenuFiltrarArchivos()
{

	cout << "Opciones/filtros: \n";
	cout << "1 - Nombre igual a X y tamano mayor que Y \n";
	cout << "2 - Nombre igual a X y tamano menor que Y \n";
	cout << "3 - Nombre igual a X y tamano igual que Y \n";
	cout << "4 - Extension igual a X y tamano mayor que Y \n";
	cout << "5 - Extension igual a X y tamano menor que Y \n";
	cout << "6 - Extension igual a X y tamano igual que Y \n";
	cout << "7 - Tamanio igual a X y el nombre comienza en Y \n";
	cout << "8 - Tamanio igual a X y el nombre termina en Y \n";
	cout << "9 - Tamanio igual a X y el nombre contiene Y \n";
	cout << "10 - Tamaño mayores a (ascendentemente):  \n";
	cout << "11 - Tamaño menores a (ascendentemente): \n";
	cout << "12 - Tamaño igual a (ascendentemente):  \n";
	cout << "13 - Nombre comienza (ascendentemente):  \n";
	cout << "14 - Nombre contiene (ascendentemente):  \n";
	cout << "15 - Nombre termina (ascendentemente):  \n";
	cout << "16 - Tamaño mayores a (descendentemente):  \n";
	cout << "17 - Tamaño menores a (descendentemente): \n";
	cout << "18 - Tamaño igual a (descendentemente):  \n";
	cout << "19 - Nombre comienza (descendentemente):  \n";
	cout << "20 - Nombre contiene (descendentemente):  \n";
	cout << "21 - Nombre termina (descendentemente):  \n";
	cout << "0 - Volver al MENU \n";

}

void MostrarMenuOrdenarArchivos() {
	cout << "ASCENDENTEMENTE\n\n";
	cout << "1 - Ordenar por nombre\n";
	cout << "2 - Ordenar por extension\n";
	cout << "3 - Ordenar por fecha\n";
	cout << "4 - Ordenar por tamanio\n";
	cout << "5 - Ordenar por ruta\n\n";
	cout << "DESCENDENTEMENTE\n\n";
	cout << "6 - Ordenar por nombre\n";
	cout << "7 - Ordenar por extension\n";
	cout << "8 - Ordenar por fecha\n";
	cout << "9 - Ordenar por tamanio\n";
	cout << "10 - Ordenar por ruta\n";
	cout << "0 - Volver al MENU\n\n";
}

int main() {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
	typedef Tree<File*, string, nullptr> TreeName;
	typedef Tree<File*, string, nullptr> TreeExt;
	typedef Tree<File*, string, nullptr> TreeDate;
	typedef Tree<File*, long long, nullptr> TreeSize;
	typedef Tree<File*, string, nullptr> TreeFpath;

	vector<File*> files;

	string path = "C:\\Users\\Hugo\\Desktop\\CarpetaPrueba";
	cout << "Para comenzar ingrese una ruta de carpetas" << endl;
	cout << "Ingrese la ruta: \n";
	getline(cin, path);

	for (const auto & entry : recursive_directory_iterator(path))
	{
		if (is_regular_file(entry.path())) {
			auto ftime = last_write_time(entry.path());
			time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
			istringstream iss(entry.path().filename().string());
			string auxname;
			getline(iss, auxname, '.');
			files.push_back(new File(auxname,
				entry.path().extension().string(),
				asctime(localtime(&cftime)),
				file_size(entry.path()), entry.path().string()));
		}

	}

	auto l1 = [](File* a) { return a->getName(); };
	auto l2 = [](File* a) { return a->getExtension(); };
	auto l3 = [](File* a) { return a->getDate(); };
	auto l4 = [](File* a) { return a->getSize(); };
	auto l5 = [](File* a) { return a->getFpath(); };

	TreeName* nameTree = new TreeName(l1);
	TreeExt* extTree = new TreeExt(l2);
	TreeDate* dateTree = new TreeDate(l3);
	TreeSize* sizeTree = new TreeSize(l4);
	TreeFpath* fpathTree = new TreeFpath(l5);

	for (auto file : files) {
		nameTree->add(file);
		extTree->add(file);
		dateTree->add(file);
		sizeTree->add(file);
		fpathTree->add(file);
	}
	auto prnt = [](File* a) {
		cout << "{ "
			<< "Nombre: " << a->getName() << " \n "
			<< "Extension: " << a->getExtension() << " \n "
			<< "Fecha: " << a->getDate()
			<< "Tamanio: " << a->getSize() << " bytes \n "
			<< "Ruta: " << a->getFpath() << "} \n\n";
	};

	long long valorUserInput = 14;  //despues, con la creacion del menu, esto seria introducido por el usuario
	auto prntmenorsize = [&valorUserInput](File* a) {
		if (a->getSize() < valorUserInput) {
			cout << "{ "
				<< "Nombre: " << a->getName() << " \n "
				<< "Extension: " << a->getExtension() << " \n "
				<< "Fecha: " << a->getDate()
				<< "Tamanio: " << a->getSize() << " bytes \n "
				<< "Ruta: " << a->getFpath() << "} \n\n";
		}
	};

	auto prntmayorsize = [&valorUserInput](File* a) {
		if (a->getSize() > valorUserInput) {
			cout << "{ "
				<< "Nombre: " << a->getName() << " \n "
				<< "Extension: " << a->getExtension() << " \n "
				<< "Fecha: " << a->getDate()
				<< "Tamanio: " << a->getSize() << " bytes \n "
				<< "Ruta: " << a->getFpath() << "} \n\n";
		}
	};

	auto prntigualsize = [&valorUserInput](File* a) {
		if (a->getSize() == valorUserInput) {
			cout << "{ "
				<< "Nombre: " << a->getName() << " \n "
				<< "Extension: " << a->getExtension() << " \n "
				<< "Fecha: " << a->getDate()
				<< "Tamanio: " << a->getSize() << " bytes \n "
				<< "Ruta: " << a->getFpath() << "} \n\n";
		}
	};

	string valorUserInputSTR = "del";
	auto prntcomienzacon = [&valorUserInputSTR](File* a) {
		if (a->getName().rfind(valorUserInputSTR, 0) == 0) {
			cout << "{ "
				<< "Nombre: " << a->getName() << " \n "
				<< "Extension: " << a->getExtension() << " \n "
				<< "Fecha: " << a->getDate()
				<< "Tamanio: " << a->getSize() << " bytes \n "
				<< "Ruta: " << a->getFpath() << "} \n\n";
		}
	};

	auto prntterminaen = [&valorUserInputSTR](File* a) {
		if (a->getName().rfind(valorUserInputSTR) == (a->getName().size() - valorUserInputSTR.size())) {
			cout << "{ "
				<< "Nombre: " << a->getName() << " \n "
				<< "Extension: " << a->getExtension() << " \n "
				<< "Fecha: " << a->getDate()
				<< "Tamanio: " << a->getSize() << " bytes \n "
				<< "Ruta: " << a->getFpath() << "} \n\n";
		}
	};

	auto prntcontiene = [&valorUserInputSTR](File* a) {
		if (a->getName().find(valorUserInputSTR) != std::string::npos) {
			cout << "{ "
				<< "Nombre: " << a->getName() << " \n "
				<< "Extension: " << a->getExtension() << " \n "
				<< "Fecha: " << a->getDate()
				<< "Tamanio: " << a->getSize() << " bytes \n "
				<< "Ruta: " << a->getFpath() << "} \n\n";
		}
	};

	// Menu

	// Preguntar si se desea ordenar ascendentemente o descendentemente antes de entregar cualquier resultado y en funcion de eso usar reverseinorder o inorder
	int option = -1, busCrit;
	string valorS;
	int valorI = -1;
	do
	{
		MostrarMenu();
		cin >> option;
		switch (option)
		{

		case 1: {
			system("CLS");
			MostrarMenuBuscarArchivos();

			cout << "Ingrese el criterio: ";
			cin >> busCrit;
			cout << "Ingrese valor: ";
			switch (busCrit)
			{
			case 0: {system("cls"); MostrarMenu(); }break;
			case 1: {cin >> valorS;
				nameTree->find2(valorS, prnt); }break;
			case 2: {cin >> valorS;
				extTree->find2(valorS, prnt); }break;
			case 3: {cin >> valorI;
				sizeTree->find2(valorI, prnt); }break;
			}
			getchar();
			getchar();

		}break;
		case 2: {
			system("CLS");
			MostrarMenuFiltrarArchivos();

			cout << "Ingrese el criterio: ";
			cin >> busCrit;

			switch (busCrit)
			{
			case 0: {system("cls"); MostrarMenu(); }break;
			case 1: {cout << "Ingrese nombre: "; cin >> valorUserInputSTR;
				cout << "Ingrese tamanio: "; cin >> valorUserInput;
				nameTree->find2(valorUserInputSTR, prntmayorsize); }break;

			case 2: {cout << "Ingrese nombre: "; cin >> valorUserInputSTR;
				cout << "Ingrese tamanio: "; cin >> valorUserInput;
				nameTree->find2(valorUserInputSTR, prntmenorsize); }break;

			case 3: {cout << "Ingrese nombre: "; cin >> valorUserInputSTR;
				cout << "Ingrese tamanio: "; cin >> valorUserInput;
				nameTree->find2(valorUserInputSTR, prntigualsize); }break;

			case 4: {cout << "Ingrese nombre: "; cin >> valorUserInputSTR;
				cout << "Ingrese tamanio: "; cin >> valorUserInput;
				extTree->find2(valorUserInputSTR, prntmayorsize); }break;

			case 5: {cout << "Ingrese nombre: "; cin >> valorUserInputSTR;
				cout << "Ingrese tamanio: "; cin >> valorUserInput;
				extTree->find2(valorUserInputSTR, prntmenorsize); }break;

			case 6: {cout << "Ingrese nombre: "; cin >> valorUserInputSTR;
				cout << "Ingrese tamanio: "; cin >> valorUserInput;
				extTree->find2(valorUserInputSTR, prntigualsize); }break;

			case 7: {cout << "Ingrese tamanio: "; cin >> valorUserInput;
				cout << "Ingrese nombre: "; cin >> valorUserInputSTR;
				sizeTree->find2(valorUserInput, prntcomienzacon); }break;

			case 8: {cout << "Ingrese tamanio: "; cin >> valorUserInput;
				cout << "Ingrese nombre: "; cin >> valorUserInputSTR;
				sizeTree->find2(valorUserInput, prntterminaen); }break;

			case 9: {cout << "Ingrese tamanio: "; cin >> valorUserInput;
				cout << "Ingrese nombre: "; cin >> valorUserInputSTR;
				sizeTree->find2(valorUserInput, prntcontiene); }break;
			case 10: {cout << "Ingrese valor: "; cin >> valorUserInput;
				sizeTree->inorder(prntmayorsize); }break;
			case 11: {cout << "Ingrese valor: "; cin >> valorUserInput;
				sizeTree->inorder(prntmenorsize); }break;
			case 12: {cout << "Ingrese valor: "; cin >> valorUserInput;
				sizeTree->inorder(prntigualsize); }break;
			case 13: {cout << "Ingrese valor: "; cin >> valorUserInputSTR;
				nameTree->inorder(prntcomienzacon); }break;
			case 14: {cout << "Ingrese valor: "; cin >> valorUserInputSTR;
				nameTree->inorder(prntcontiene); }break;
			case 15: {cout << "Ingrese valor: "; cin >> valorUserInputSTR;
				nameTree->inorder(prntterminaen); }break;
			case 16: {cout << "Ingrese valor: "; cin >> valorUserInput;
				sizeTree->reverseinorder(prntmayorsize); }break;
			case 17: {cout << "Ingrese valor: "; cin >> valorUserInput;
				sizeTree->reverseinorder(prntmenorsize); }break;
			case 18: {cout << "Ingrese valor: "; cin >> valorUserInput;
				sizeTree->reverseinorder(prntigualsize); }break;
			case 19: {cout << "Ingrese valor: "; cin >> valorUserInputSTR;
				nameTree->reverseinorder(prntcomienzacon); }break;
			case 20: {cout << "Ingrese valor: "; cin >> valorUserInputSTR;
				nameTree->reverseinorder(prntcontiene); }break;
			case 21: {cout << "Ingrese valor: "; cin >> valorUserInputSTR;
				nameTree->reverseinorder(prntterminaen); }break;

			}
			getchar();
			getchar();
		}break;
		case 3: {
			system("CLS");
			MostrarMenuOrdenarArchivos();

			cout << "Ingrese el criterio: ";
			cin >> busCrit;

			switch (busCrit)
			{
			case 0: {system("cls"); MostrarMenu(); }break;
			case 1: {nameTree->inorder(prnt); }break;
			case 2: {extTree->inorder(prnt); }break;
			case 3: {dateTree->inorder(prnt); }break;
			case 4: {sizeTree->inorder(prnt); }break;
			case 5: {fpathTree->inorder(prnt); }break;
			case 6: {nameTree->reverseinorder(prnt); }break;
			case 7: {extTree->reverseinorder(prnt); }break;
			case 8: {dateTree->reverseinorder(prnt); }break;
			case 9: {sizeTree->reverseinorder(prnt); }break;
			case 10: {fpathTree->reverseinorder(prnt); }break;
			}
			getchar();
			getchar();
		}break;
		}

		system("cls");
	} while (option != 0);

	delete nameTree;
	delete extTree;
	delete dateTree;
	delete sizeTree;


#ifdef _MSC_VER
#pragma warning(pop)
#endif

	return 0;
}


