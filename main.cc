#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "cmdline.h"
#include "system.h"
#include "query.h"
#include "sensor.h"

using namespace std;

static void opt_data(string const &);
static void opt_input(string const &);
static void opt_output(string const &);
static void opt_help(string const &);

/**************** Elementos globales ******************/
static option_t options[] = {
	{1, "d", "data", "-", opt_data, OPT_MANDATORY},
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{0, "h", "help", NULL, opt_help, OPT_DEFAULT},
	{0, },
};

static istream *iss = 0;	// Input Stream (clase para manejo de los flujos de entrada)
static ostream *oss = 0;	// Output Stream (clase para manejo de los flujos de salida)
static istream *qss = 0;	// Output Stream (clase para manejo de los flujos de salida)
static fstream ifs; 		// Input File Stream (derivada de la clase ifstream que deriva de istream para el manejo de archivos)
static fstream ifs2;		// Input File Stream (derivada de la clase ifstream que deriva de istream para el manejo de archivos)
static fstream ofs;			// Output File Stream (derivada de la clase ofstream que deriva de ostream para el manejo de archivos)
/*****************************************************/

static void opt_data(string const &arg)
{
	if (arg == "-") 
	{
		iss = &cin;		// Establezco la entrada estandar cin como flujo de entrada
	}
	else 
	{
		ifs.open(arg.c_str(), ios::in); 
		iss = &ifs;
	}

	if (!iss->good()) 
	{
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void opt_input(string const &arg)
{
	if (arg == "-") 
	{
		qss = &cin;		// Establezco la entrada estandar cin como flujo de entrada
	}
	else 
	{
		ifs2.open(arg.c_str(), ios::in); 
		qss = &ifs2;
	}

	if (!qss->good()) {
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void opt_output(string const &arg)
{
	// Si el nombre del archivos es "-", usaremos la salida
	// estándar. De lo contrario, abrimos un archivo en modo
	// de escritura.
	//
	if (arg == "-") {
		oss = &cout;	// Establezco la salida estandar cout como flujo de salida
	} else {
		ofs.open(arg.c_str(), ios::out);
		oss = &ofs;
	}

	// Verificamos que el stream este OK.
	//
	if (!oss->good()) {
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);		// EXIT: Terminación del programa en su totalidad
	}
}

static void opt_help(string const &arg)
{
	cout << "cmdline [-d file] [-i file] [-o file]"
	     << endl;
	exit(0);
}

int main(int argc, char * const argv[])
{
	Array<Query *> query_array;
	size_t i=0;
	cmdline cmdl(options);
	cmdl.parse(argc, argv);
	System system;

	system.load_sensors_from_csv(*iss);

	Query::load_querys_from_csv(*qss,query_array);

	for (i=0 ; i<query_array.size() ; i++)
	{
		query_array[(size_t)i]->set_target_system(&system);
		query_array[(size_t)i]->execute_query(*oss);
	}
	for (i=0 ; i<query_array.size() ; i++)
	{
		delete query_array[i];
	}
	return 0;

}
