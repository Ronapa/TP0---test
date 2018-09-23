#ifndef QUERY_H
#define QUERY_H

#include <iostream>
#include <string>
#include "Array.h"
#include "system.h"
#include "sensor.h"
#include "utilities.h"

using namespace std;

typedef enum 
{
	MONO_SENSOR,
	MULTI_SENSOR
}query_tipe_t;

typedef enum 
{
	FALSE,
	TRUE
}bool_t;


class Query
{
	private:
		Array<string> sensors_in_query;
		System * target_system;
		int left_bound , right_bound;
		query_tipe_t type_of_query;
		void _validate_query();
		bool_t valid_query;

	public:
		Query();
		Query(const string &);
		~Query();

		void set_left_bound(const int &);
		void set_right_bound(const int &);
		void set_left_bound(const string &);
		void set_right_bound(const string &);
		void set_target_system(System *);

		void add_sensor_to_query(const string &);
		
		size_t get_amount_of_sensors_in_query();
		string get_sensor_in_query_at_index(const int &);
		int get_left_bound();
		int get_right_bound();

		void execute_query();

		friend istream & operator>>(std::istream &in, Query & query);
		static void load_querys_from_csv(istream& , Array<Query *>& );

};

#endif