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

}

void MostrarMenuBuscarArchivos(int cri)
{
	cout << "Cantidad de criterios: 1  |  2 \n";
	do {
		cin >> cri;
	} while (cri > 2 || cri < 1);

	cout << "Opciones: \n";
	cout << "1 - Nombre \n";
	cout << "2 - Extension \n";
	cout << "3 - Fecha \n";
	cout << "4 - Tamano \n";

}

void MostrarMenuFiltrarArchivos()
{

	cout << "Opciones/filtros: \n";
	cout << "1 - Tamano: Mayor que... \n";
	cout << "2 - Tamano: Menor que... \n";
	cout << "3 - Tamano: Igual que... \n";
	cout << "4 - Nombre: Comienza con... \n";
	cout << "5 - Nombre: Comienza con... \n";
	cout << "6 - Nombre: Comienza con... \n";

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

		/*//PROBAR
		//	//extension
		//cout << entry.path().extension() << endl;			//recursive_directory_iterator->solo funciona con esto
		//	//nombre
		//cout << entry.path().filename() << endl;		//recursive_directory_iterator->solo funciona con esto
		//	//tamaño
		//cout << file_size(entry.path()) << endl;
		//	//fecha
		//auto ftime = last_write_time(entry.path());
		//time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
		//cout << asctime(localtime(&cftime)) << endl;*/

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
			<< a->getName() << " | "
			<< a->getExtension() << " | "
			<< a->getDate() << " | "
			<< a->getSize() << " bytes | "
			<< a->getFpath() << "} \n";
	};



	long long valorUserInput = 5;  //despues, con la creacion del menu, esto seria introducido por el usuario
	auto prntmenorsize = [valorUserInput](File* a) {
		if (a->getSize() < valorUserInput) {
			cout << "{ "
				<< a->getName() << " | "
				<< a->getExtension() << " | "
				<< a->getDate() << " | "
				<< a->getSize() << " bytes | "
				<< a->getFpath() << "} \n";
		}
	};

	auto prntmayorsize = [valorUserInput](File* a) {
		if (a->getSize() > valorUserInput) {
			cout << "{ "
				<< a->getName() << " | "
				<< a->getExtension() << " | "
				<< a->getDate() << " | "
				<< a->getSize() << " bytes | "
				<< a->getFpath() << "} \n";
		}
	};


	auto prntigualsize = [valorUserInput](File* a) {
		if (a->getSize() == valorUserInput) {
			cout << "{ "
				<< a->getName() << " | "
				<< a->getExtension() << " | "
				<< a->getDate() << " | "
				<< a->getSize() << " bytes | "
				<< a->getFpath() << "} \n";
		}
	};


	string valorUserInputSTR = "opi";
	auto prntcomienzacon = [valorUserInputSTR](File* a) {
		if (a->getName().rfind(valorUserInputSTR, 0) == 0) {
			cout << "{ "
				<< a->getName() << " | "
				<< a->getExtension() << " | "
				<< a->getDate() << " | "
				<< a->getSize() << " bytes | "
				<< a->getFpath() << "} \n";
		}
	};

	auto prntterminaen = [valorUserInputSTR](File* a) {
		if (a->getName().rfind(valorUserInputSTR) == (a->getName().size() - valorUserInputSTR.size())) {
			cout << "{ "
				<< a->getName() << " | "
				<< a->getExtension() << " | "
				<< a->getDate() << " | "
				<< a->getSize() << " bytes | "
				<< a->getFpath() << "} \n";
		}
	};

	auto prntcontiene = [valorUserInputSTR](File* a) {
		if (a->getName().find(valorUserInputSTR) != std::string::npos) {
			cout << "{ "
				<< a->getName() << " | "
				<< a->getExtension() << " | "
				<< a->getDate() << " | "
				<< a->getSize() << " bytes | "
				<< a->getFpath() << "} \n";
		}
	};

	// Menu

	// Preguntar si se desea ordenar ascendentemente o descendentemente antes de entregar cualquier resultado y en funcion de eso usar reverseinorder o inorder

	sizeTree->find2(180, prntcontiene); cout << "-----------------------==\n";
	sizeTree->find2(180, prntcomienzacon); cout << "-----------------------==\n";
	nameTree->find2("textoplano", prntmayorsize); cout << "-----------------------==\n";
	nameTree->inorder(prnt); cout << "-----------------------==\n";
	nameTree->reverseinorder(prnt); cout << "-----------------------==\n";
	extTree->inorder(prnt); cout << "-----------------------==\n";
	dateTree->inorder(prnt); cout << "-----------------------==\n";
	sizeTree->inorder(prnt); cout << "-----------------------==\n";
	sizeTree->inorder(prntmenorsize); cout << "-----------------------==\n";
	fpathTree->inorder(prnt); cout << "-----------------------==\n";

	//prnt(dateTree->find("Wed"));
	prnt(sizeTree->find(180));
	prnt(sizeTree->find(10));
	prnt(sizeTree->find(180));
	prnt(sizeTree->find(10));

	//delete nameTree;
	//delete extTree;
	//delete dateTree;
	//delete sizeTree;


#ifdef _MSC_VER
#pragma warning(pop)
#endif

	getchar();
	getchar();
	return 0;
}


