#ifndef QUERY_CC
#define QUERY_CC


#include <iostream>
#include <string>
#include "Array.h"
#include "query.h"
#include "system.h"
#include "sensor.h"
#include "utilities.h"

using namespace std;

Query::Query(const string & query_name)
{
	sensors_in_query.push_back(query_name);
	type_of_query = MONO_SENSOR;
	left_bound = -1;
	right_bound = -1;
}

Query::Query()
{
	type_of_query = MONO_SENSOR;
	left_bound = -1;
	right_bound = -1;
}

Query::~Query()
{

}

void Query::load_querys_from_csv(istream& in, Array<Query *>& querys) 
{
   string tmp;
   Array<string> v;
   Query * query;

   while(getline(in, tmp, '\n'))
   {
      query = new Query();
      stringstream str_st (tmp);
      str_st >> (*query);
      querys.push_back(query);
      tmp.clear();
   }
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

void Query::set_left_bound(const string & left)
{
   stringstream str_st(left);
   if(!(str_st>>left_bound) || (left_bound < 0))
      left_bound = -1;
}

void Query::set_right_bound(const string & right)
{
   stringstream str_st(right);
   if(!(str_st>>right_bound) || (right_bound < 0))
      right_bound = -1;
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
						cout << target_system->get_average_temperature_in_range_of_sensor_at_index(j,left_bound,right_bound) <<  " ";
						cout << target_system->get_min_temperature_in_range_of_sensor_at_index(j,left_bound,right_bound) << " " ;
						cout << target_system->get_max_temperature_in_range_of_sensor_at_index(j,left_bound,right_bound) << " ";
						cout << target_system->get_amount_of_valid_temperatures_in_range_at_index(j,left_bound,right_bound) << endl;
					}
				}
			}
		}else
		{
			sensor *aux_sensor = new sensor("aux_sensor");
			float accum =0;
			float aux=0;
			size_t valid_meassures = 0;
			for (k = left_bound ; k<right_bound ; k++)
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
			
			cout << aux_sensor->get_average_temperature_in_range(0,aux_sensor->get_amount_of_temperature_measures()) <<  " ";
			cout << aux_sensor->get_min_temperature_in_range(0,aux_sensor->get_amount_of_temperature_measures()) <<  " ";
			cout << aux_sensor->get_max_temperature_in_range(0,aux_sensor->get_amount_of_temperature_measures()) << " ";
			cout << aux_sensor->get_amount_of_valid_temperatures_in_range(0,aux_sensor->get_amount_of_temperature_measures()) << endl;

			delete aux_sensor;
		}
	}else
	{
		if (left_bound < 0 || right_bound < 0)
		{
			cout << "BAD QUERY" << endl;
		}else
		{
			cout << "UNKNOWN ID" << endl;
		}
	}	
}

void Query::_validate_query()
{
	size_t i=0 , j=0;
	valid_query = FALSE;
	int valid_sensors =0;
	for (i=0 ;  i<get_amount_of_sensors_in_query() ; i++)
	{
		if (sensors_in_query[i] == "")
		{
			for (j=0 ; j < target_system->get_amount_of_sensors_in_system() ; j++)
			{
				(*this).add_sensor_to_query(target_system->get_sensor_in_system_at_index(j));
			}
			valid_sensors ++;
		}else
		{
			for (j=0 ; j < target_system->get_amount_of_sensors_in_system() ; j++)
			{
				if (sensors_in_query[i] == target_system->get_sensor_in_system_at_index(j))
				{
					valid_sensors ++;
				}
			}
		}
		
	}
	if (valid_sensors == get_amount_of_sensors_in_query())
	{
		valid_query = TRUE;
	}
	if (left_bound < 0 || right_bound < 0)
	{
		valid_query = FALSE;
	}
}

istream & operator>>(std::istream &in, Query & query)
{
   Array<string> v;
   int left_bound, right_bound;
   string tmp;

   getline(in, tmp, '\n');
   _split(tmp, ',', v);
   if(v.size() != 3)
      query.left_bound = query.right_bound = -1;

   query.add_sensor_to_query(v[0]);
   query.set_left_bound(v[1]);
   query.set_right_bound(v[2]);

   return in;
}

/*
std::istream & operator>>(std::istream &in, System & system)
{
   string aux;
   char ch = 0;
   int i=0;
   
   if( !(in >> measure) )
   {
      measure = -273;
   }
   system.sensor_array[i]->add_temperature_to_sensor(measure);
   while( (in >> ch) && (ch == ',') )
   {  
      i++; 
      if( !(in >> measure) )
      {
         measure = -273;
      }
      system.sensor_array[i]->add_temperature_to_sensor(measure);
   } 
    return in;
}
*/
#endif