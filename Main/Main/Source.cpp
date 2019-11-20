#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include "Arbol.h"
#include <sstream>

using namespace std;
using namespace std::experimental::filesystem;
using namespace std::chrono_literals;

class File {
	string name, extension, date;
	long long size;
public:
	File(string name, string extension, string date, long long size)
		: name(name), extension(extension), date(date), size(size) {}

	string getName() { return name; }
	string getExtension() { return extension; }
	string getDate() { return date; }
	long long getSize() { return size; }


	void setname(string name) { this->name = name; }
	void setextension(string extension) { this->extension = extension; }
	void setdate(string date) { this->date = date; }
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

	vector<File*> files;
	string path = "D:\\Work & Travel\\Listo";
	for (const auto & entry : directory_iterator(path))
	{
		auto ftime = last_write_time(entry.path());
		time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
		istringstream iss(entry.path().filename().string());
		string aux;
		getline(iss, aux, '.');
		files.push_back(new File(aux,
			entry.path().extension().string(),
			asctime(localtime(&cftime)),
			file_size(entry.path())));

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


	auto l1 = [](File* a) {	return a->getName(); };
	auto l2 = [](File* a) { return a->getExtension(); };
	auto l3 = [](File* a) { return a->getDate(); };
	auto l4 = [](File* a) { return a->getSize(); };

	TreeName* nameTree = new TreeName(l1);
	TreeExt* extTree = new TreeExt(l2);
	TreeDate* dateTree = new TreeDate(l3);
	TreeSize* sizeTree = new TreeSize(l4);


	for (auto file : files) {
		nameTree->add(file);
		extTree->add(file);
		dateTree->add(file);
		sizeTree->add(file);
	}

	auto prnt = [](File* a) {
		cout << "{"
			<< a->getName() << ", "
			<< a->getExtension() << ", "
			<< a->getDate() << ", "
			<< a->getSize() << "}\n";
	};
	nameTree->inorder(prnt); cout << "-----------------------==\n";
	extTree->inorder(prnt); cout << "-----------------------==\n";
	dateTree->inorder(prnt); cout << "-----------------------==\n";
	sizeTree->inorder(prnt); cout << "-----------------------==\n";

	prnt(nameTree->find("DNI"));
	prnt(extTree->find(".pdf"));
	//prnt(dateTree->find("Wed May  8 20:43:52 2019"));
	prnt(sizeTree->find(125));


	delete nameTree;
	delete extTree;
	delete dateTree;
	delete sizeTree;
#ifdef _MSC_VER
#pragma warning(pop)
#endif
	system("pause");
	return 0;
}