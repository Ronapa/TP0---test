// Prueba de la clase cmdline: dado un factor entero pasado por la
// línea de comando, leemos una secuencia de números que ingresan
// por la entrada estándar, los multiplicamos por ese factor, y
// luego mostramos el resultado por std::cout.
//
// $Id: main.cc,v 1.5 2012/09/15 12:23:57 lesanti Exp $

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

static void opt_input(string const &);
static void opt_query(string const &);
static void opt_output(string const &);
static void opt_help(string const &);

// Tabla de opciones de línea de comando. El formato de la tabla
// consta de un elemento por cada opción a definir. A su vez, en
// cada entrada de la tabla tendremos:
//
// o La primera columna indica si la opción lleva (1) o no (0) un
//   argumento adicional.
//
// o La segunda columna representa el nombre corto de la opción.
//
// o Similarmente, la tercera columna determina el nombre largo.
//
// o La cuarta columna contiene el valor por defecto a asignarle
//   a esta opción en caso que no esté explícitamente presente
//   en la línea de comandos del programa. Si la opción no tiene
//   argumento (primera columna nula), todo esto no tiene efecto.
//
// o La quinta columna apunta al método de parseo de la opción,
//   cuyo prototipo debe ser siempre void (*m)(string const &arg);
//
// o La última columna sirve para especificar el comportamiento a
//   adoptar en el momento de procesar esta opción: cuando la
//   opción es obligatoria, deberá activarse OPT_MANDATORY.
//
// Además, la última entrada de la tabla debe contener todos sus
// elementos nulos, para indicar el final de la misma.
//

/**************** Elementos globales ******************/
static option_t options[] = {
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "q", "query", "-", opt_query, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{0, "h", "help", NULL, opt_help, OPT_DEFAULT},
	{0, },
};

static istream *iss = 0;	// Input Stream (clase para manejo de los flujos de entrada)
static ostream *oss = 0;	// Output Stream (clase para manejo de los flujos de salida)
static istream *qss = 0;	// Output Stream (clase para manejo de los flujos de salida)
static fstream ifs; 		// Input File Stream (derivada de la clase ifstream que deriva de istream para el manejo de archivos)
static fstream ofs;			// Output File Stream (derivada de la clase ofstream que deriva de ostream para el manejo de archivos)



/*****************************************************/

static void
opt_input(string const &arg)
{
	// Si el nombre del archivos es "-", usaremos la entrada
	// estándar. De lo contrario, abrimos un archivo en modo
	// de lectura.
	//
	if (arg == "-") {
		iss = &cin;		// Establezco la entrada estandar cin como flujo de entrada
	}
	else {
		ifs.open(arg.c_str(), ios::in); // c_str(): Returns a pointer to an array that contains a null-terminated
										// sequence of characters (i.e., a C-string) representing
										// the current value of the string object.
		iss = &ifs;
	}

	// Verificamos que el stream este OK.
	//
	if (!iss->good()) {
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void
opt_output(string const &arg)
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

static void opt_query(string const &arg)
{
	// Si el nombre del archivos es "-", usaremos la entrada
	// estándar. De lo contrario, abrimos un archivo en modo
	// de lectura.
	//
	if (arg == "-") {
		qss = &cin;		// Establezco la entrada estandar cin como flujo de entrada
	}
	else {
		ifs.open(arg.c_str(), ios::in); // c_str(): Returns a pointer to an array that contains a null-terminated
										// sequence of characters (i.e., a C-string) representing
										// the current value of the string object.
		qss = &ifs;
	}

	// Verificamos que el stream este OK.
	//
	if (!qss->good()) {
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void opt_help(string const &arg)
{
	cout << "cmdline -f factor [-i file] [-o file]"
	     << endl;
	exit(0);
}

int main(int argc, char * const argv[])
{
	cmdline cmdl(options);
	cmdl.parse(argc, argv);
	System system_CPU;

	system_CPU.load_sensors_from_csv(*iss);
	//Query query_test("CPU Package");
	//query_test.add_sensor_to_query("CPU");
	//query_test.add_sensor_to_query("Motherboard");
	//query_test.add_sensor_to_query("GPU Thermal Diode");
	
	Query query_test("Core #0 Usage [%]");
	query_test.add_sensor_to_query("Core #1 Usage [%]");
	query_test.add_sensor_to_query("Core #2 Usage [%]");
	query_test.add_sensor_to_query("Core #3 Usage [%]");
	query_test.add_sensor_to_query("Core #4 Usage [%]");
	query_test.add_sensor_to_query("Core #5 Usage [%]");
	query_test.add_sensor_to_query("Core #6 Usage [%]");
	//query_test.add_sensor_to_query("Core #7 Usage [%]");
	query_test.add_sensor_to_query(system_CPU.get_sensor_in_system_at_index(7));

	

	query_test.set_target_system(&system_CPU);
	query_test.set_left_bound(0);
	query_test.set_right_bound(1);

	query_test.execute_query();
	
	return 0;

}

/*
	//Ya implemente la funcion de ejecutar querys y creo q funciona bien. Lo unico que hay que mandarle es la query con la cantidad de sensores
	//sobre los que se tiene q ejecutar (los nombres) y hace todo en execute_query.

	//Tambien agregue la funcion para contar la cantidad de mediciones validas en un rango porq no lo teniamos hasta el momento.
	//Cuando subas la lectura de alguna de las cosas, ya sea sensores o no avisame asi me pongo y trato de hacer algunas pruebas. mañana me bajo
	//Valgrind y veo que onda

	//Agregue el casteo a size_t para q no joda el compilador pero son un monton y me parece medio feo. Mañana medito si conviene pasar todo a size_t o no.
	//porque lo q tiene de bueno usar int es que ponemos -1 para un rango no valido y fue. con el size_t se complica hacer eso.

	//Tambien habria que agregar el -273 a una macro y ponerle -274 u otra cosa.

	//Cuando subas los archivos metelos en un archivo aparte por las dudas. 
	Array<float> arr1;
	Array<float> arr2;
	arr1.push_back(2);
	arr1.push_back(4);
	arr1.push_back(6);
	arr1.push_back(10);
	arr1.push_back(-273);
	arr2.push_back(6);
	arr2.push_back(4);
	arr2.push_back(2);
	arr2.push_back(-2);
	arr2.push_back(8);

	cmdline cmdl(options);
	cmdl.parse(argc, argv);
	System system_test;

	Query query_test("pedro2");
	Query query_test2("pedro3");

	system_test.add_new_sensor_to_system("pedro2");
	system_test.add_new_sensor_to_system("pedro3");

	system_test.load_sensor_with_array(arr1,"pedro2");
	system_test.load_sensor_with_array(arr2,"pedro3");

	query_test.set_target_system(&system_test);
	query_test.set_left_bound(1);
	query_test.set_right_bound(4);

	query_test2.set_target_system(&system_test);
	query_test2.set_left_bound(0);
	query_test2.set_right_bound(3);

	query_test.execute_query();
	query_test2.execute_query();

	query_test.add_sensor_to_query("pedro3");
	query_test.execute_query();
*/