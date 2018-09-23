#ifndef QUERY_CC
#define QUERY_CC


#include <iostream>
#include <string>
#include "Array.h"
#include "query.h"
#include "system.h"
#include "sensor.h"

using namespace std;

Query::Query(const string & query_name)
{
	sensors_in_query.push_back(query_name);
	type_of_query = MONO_SENSOR;
	left_bound = -1;
	right_bound = -1;
}

Query::~Query()
{

}


void Query::set_target_system(System * target)
{
	if (target == NULL)
	{
		cerr << "NULL POINTER";
		return;
	}
	target_system = target;
}

void Query::set_left_bound(const int & left)
{
	if (left < 0)
	{
		return;
	}
	left_bound = left;
}

void Query::set_right_bound(const int & right)
{
	if (right < left_bound)
	{
		return;
	}
	right_bound = right;
}

void Query::add_sensor_to_query(const string &target_of_query)
{
	sensors_in_query.push_back(target_of_query);
}

size_t Query::get_amount_of_sensors_in_query()
{
	return sensors_in_query.size();
}

string Query::get_sensor_in_query_at_index(const int &index)
{
	return sensors_in_query[index];
}

void Query::execute_query()
{
	size_t i=0 , j=0 , k=0;
	_validate_query();
	if (valid_query == TRUE)
	{
		if (get_amount_of_sensors_in_query() > 1)
		{
			type_of_query = MULTI_SENSOR;
		}
		if (type_of_query == MONO_SENSOR)
		{
			for (i=0 ;  i<get_amount_of_sensors_in_query() ; i++)
			{
				for (j=0 ; j < target_system->get_amount_of_sensors_in_system() ; j++)
				{
					if (sensors_in_query[i] == target_system->get_sensor_in_system_at_index(j))
					{
						cout << "Promedio: ";
						cout << target_system->get_average_temperature_in_range_of_sensor_at_index(j,left_bound,right_bound) << endl;
						cout << "Mínimo: ";
						cout << target_system->get_min_temperature_in_range_of_sensor_at_index(j,left_bound,right_bound) << endl;
						cout << "Máximo: ";
						cout << target_system->get_max_temperature_in_range_of_sensor_at_index(j,left_bound,right_bound) << endl;
					}
				}
			}
		}else
		{
			sensor *aux_sensor = new sensor("aux_sensor");
			float accum =0;
			float aux=0;
			size_t valid_meassures = 0;
			for (k = left_bound ; k<=right_bound ; k++)
			{
				for (i=0 ;  i<get_amount_of_sensors_in_query() ; i++)
				{
					for (j=0 ; j < target_system->get_amount_of_sensors_in_system() ; j++)
					{
						if (sensors_in_query[i] == target_system->get_sensor_in_system_at_index(j))
						{
							if((aux = target_system->get_temperature_at_of_sensor_at_index(j,k)) != -273)
							{
								accum += aux;
								valid_meassures++;
							}else
							{

							}
						}
					}
				}
				if (valid_meassures == 0)
				{
					aux_sensor->add_temperature_to_sensor(-273);
				}else 
				{
					aux_sensor->add_temperature_to_sensor(accum/valid_meassures);
				}
				accum =0;
				valid_meassures = 0;
			}
			
			cout << "Promedio: ";
			cout << aux_sensor->get_average_temperature_in_range(0,aux_sensor->get_amount_of_temperature_measures()) << endl;
			cout << "Mínimo: ";
			cout << aux_sensor->get_min_temperature_in_range(0,aux_sensor->get_amount_of_temperature_measures()) << endl;
			cout << "Máximo: ";
			cout << aux_sensor->get_max_temperature_in_range(0,aux_sensor->get_amount_of_temperature_measures()) << endl;
			cout << "Mediciones: ";
			cout << aux_sensor->get_amount_of_valid_temperatures_in_range(0,aux_sensor->get_amount_of_temperature_measures()) << endl;
		}
	}else
	{
		cout << "UNKNOWN ID" << endl;
	}	
}

void Query::_validate_query()
{
	size_t i=0 , j=0;
	valid_query = FALSE;
	int valid_sensors =0;
	for (i=0 ;  i<get_amount_of_sensors_in_query() ; i++)
	{
		for (j=0 ; j < target_system->get_amount_of_sensors_in_system() ; j++)
		{
			if (sensors_in_query[i] == target_system->get_sensor_in_system_at_index(j))
			{
				valid_sensors ++;
			}
		}
	}
	if (valid_sensors == get_amount_of_sensors_in_query())
	{
		valid_query = TRUE;
	}
}



#endif