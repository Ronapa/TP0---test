#ifndef SYSTEM_CC
#define SYSTEM_CC

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include "Array.h"
#include "system.h"
#include "sensor.h"
#include "utilities.h"

using namespace std;

System::System()
{

}

System::~System()
{
	int i=0;
	while ( i < get_amount_of_sensors_in_system() )
	{
		delete sensor_array[i];
		i++;
	}
}

void System::add_new_sensor_to_system(const string & sensor_name)
{
	sensor_array.push_back(new sensor(sensor_name));
	sensor_names.push_back(sensor_name);
}

void System::load_sensors_from_csv(istream &in)
{
   Array<string> v;
   float measure;
   string tmp;

   if(!in.eof())
   {
      getline(in, tmp, '\n');
      _split(tmp, ',', v);
      for (int i = 0; i < v.size(); ++i)
      {
         if (i == v.size()-1)
         {
            v[i].erase(v[i].size()-1);
         }
         add_new_sensor_to_system(v[i]);
      }
      v.clear();
   }

   while (!in.eof()) 
   {
      getline(in, tmp, '\n');
      stringstream str_st (tmp);
      str_st>>*this;
      tmp.clear();
   }  
}

int System::get_amount_of_sensors_in_system()
{
	return sensor_array.size();
}

string System::get_sensor_in_system_at_index(const int &index)
{
	return sensor_array[index]->get_sensor_name();
}

float System::get_average_temperature_in_range_of_sensor_at_index(const int &index , const int &left , const int &right)
{
	return sensor_array[index]->get_average_temperature_in_range(left,right);
}

float System::get_min_temperature_in_range_of_sensor_at_index(const int & index, const int & left, const int &right)
{
	return sensor_array[index]->get_min_temperature_in_range(left,right);
}

float System::get_max_temperature_in_range_of_sensor_at_index(const int & index, const int & left, const int &right)
{
	return sensor_array[index]->get_max_temperature_in_range(left,right);
}

float System::get_temperature_at_of_sensor_at_index(const int & index , const int &temp_index)
{
	float pepe = sensor_array[index]->get_temperature_at(temp_index);
	return pepe;
}

void System::load_sensor_with_array(const Array<float> & arr, const string &name)
{
	size_t j=0 , k=0;

	for (j=0 ; j < get_amount_of_sensors_in_system() ; j++)
	{
		if (name == get_sensor_in_system_at_index(j))
		{
			for (k=0 ; k < arr.size() ; k++)
			{
				(sensor_array[j])->add_temperature_to_sensor( arr[k] );
			}
		}
	}
	
}

int System::get_amount_of_valid_temperatures_in_range_at_index(const int & index, const int & left, const int &right)
{
   if (index >= sensor_array.size())
   {
      return -1;
   }
   return sensor_array[index]->get_amount_of_valid_temperatures_in_range(left,right);
}

std::istream & operator>>(std::istream &in, System & system)
{
   float measure = 0;
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
/*
void System::load_querys_from_csv(istream & qin , Array<Query> & query_array)
{
   Array<string> v;
   float measure;
   string tmp;
   int i=0 , k=0;

      while (!qin.eof()) 
   {
         getline(qin, tmp, '\n');
         _split(tmp, ',', v);
         for (i = 0 ; i<v.size() ; i++)
         {
            if (i<(v.size() -2))
            {
               query_array[k].add_sensor_to_query(v[i]);
            }else
            {
               if ( i=(v.size()-2))
               {
                  stringstream str_st(v[i]);
                  if(!(str_st>>left_bound) || (left_bound < 0))
                  {
                     cout << "query rota izq" << endl;
                     left_bound = -1;
                  }
               }else
               {
                  stringstream str_st(v[i]);
                  if(!(str_st>>right_bound) || (right_bound < 0))
                  {
                     cout << "query rota der" << endl;
                     right_bound = -1;
                  }
               }
            }
         }
         
         v.clear();
      }
}

*/
#endif