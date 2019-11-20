#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <Windows.h>
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

	string path = "D:\\Work & Travel\\Listo";

	for (const auto & entry : directory_iterator(path))
	{

		auto ftime = last_write_time(entry.path());
		time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
		istringstream iss(entry.path().filename().string());
		string auxname;
		getline(iss, auxname, '.');

		files.push_back(new File(auxname,
			entry.path().extension().string(),
			asctime(localtime(&cftime)),
			file_size(entry.path()), entry.path().string()));

		//PROBAR
		//	//extension
		//cout << entry.path().extension() << endl;			//recursive_directory_iterator->solo funciona con esto
		//	//nombre
		//cout << entry.path().filename() << endl;		//recursive_directory_iterator->solo funciona con esto
		//	//tamaño 
		//cout << file_size(entry.path()) << endl;
		//	//fecha
		//auto ftime = last_write_time(entry.path());
		//time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
		//cout << asctime(localtime(&cftime)) << endl;

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

	TreeExt* extTreeFound = new TreeExt(l2);

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

	TreeExt* extTreecpy = extTree;

	auto foundExttree = [&extTreeFound, &extTreecpy](File* a) {

		extTreeFound->add(extTreecpy->find(".txt"));  // ir borrando lo que se va encontrando y agregando al arbol de encontrados para que no repita siempre el mismo archivo con la misma extension
													  // esto no es un problema para path ni nombre ya que estos son necesariamente unicos. Para tamaño, es poco probable que dos archivos pesen exactamente lo mismo, pero no estaria mal usar la misma solucion que para la extension

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



	nameTree->inorder(prnt); cout << "-----------------------==\n";
	nameTree->reverseinorder(prnt); cout << "-----------------------==\n";
	extTree->inorder(prnt); cout << "-----------------------==\n";
	extTree->inorder(foundExttree);
	extTreeFound->inorder(prnt);
	cout << "-----------------------==\n";
	dateTree->inorder(prnt); cout << "-----------------------==\n";
	sizeTree->inorder(prnt); cout << "-----------------------==\n";
	sizeTree->inorder(prntmenorsize); cout << "-----------------------==\n";
	fpathTree->inorder(prnt); cout << "-----------------------==\n";

	//prnt(dateTree->find("Wed"));
	prnt(sizeTree->find(197754));
	nameTree->find2("DNI", prnt);
	//prnt(nameTree->find2("DNI.docx"));
	/*prnt(sizeTree->find(10));
	prnt(sizeTree->find(180));
	prnt(sizeTree->find(10));*/

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

//Por hacer (retomamos el lunes (yo puedo desde las 7:30 pm hasta el dia siguiente)):
//crear nuevos arboles con lambdas void de acuerdo a los requisitos del filtro o los criterios de busqueda(usar find() del string para el "contiene") para poder combinar criterios
//quiza la consulta filtrando de acuerdo a lo mencionado en D. de la rubrica 
//crear menu

//Basicamente falta la parte de combinar criterios del punto C (ya que ya podemos buscar y ordenar de acuerdo a un solo criterio si vamos al arbol que corresponde) y parte del punto D (ya filtra por tamaño), aunque ya hay posibles soluciones si se encuentra la manera de evitar ciertos errores (como el find que deja de buscar con la primera coincidencia del criterio). 
//Ademas para algunas cosas podriamos usar prnt especiales como prntmenorsize

