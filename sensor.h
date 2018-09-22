#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>
#include "Array.h"

using namespace std;

class sensor
{
	private:

		Array<float> temperature_values;
		string sensor_name;
		
	public:
		
		sensor();
		sensor(const string &);
		~sensor();

		void add_temperature_to_sensor(const float &);
		void set_sensor_name(const string &);

		string get_sensor_name();
		size_t get_amount_of_temperature_measures();
		float get_average_temperature_in_range(const int & , const int &);
		float get_min_temperature_in_range(const int & , const int &);
		float get_max_temperature_in_range(const int & , const int &);
		float get_temperature_at(const int &);
		int get_amount_of_valid_temperatures_in_range(const int & , const int &);
	
};

#endif
