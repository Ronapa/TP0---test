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

void System::load_sensors_from_csv(istream &i)
{
   string tmp;
   Array<string> v;
   float temperature;

   if(!i.eof())
   {
      getline(i, tmp, '\n');
      _split(tmp, ',', v);
      for (int i = 0; i < v.size(); ++i)
      {
         add_new_sensor_to_system(v[i]);
      }
      v.clear();
   }
   while (!i.eof()) {
      getline(i, tmp, '\n');

      _split(tmp, ',', v);
      for (int i = 0; i < v.size(); ++i)
      {
         stringstream str_st (v[i]);
         if(v[i]=="" || !(str_st>>temperature))
            temperature=-273;
         str_st.str(string());
         sensor_array[i]->add_temperature_to_sensor(temperature);
      }
      v.clear();
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

void System::_split(const string& s, const char c,
           Array<string>& v) {
   int i = 0;
   int j = s.find(c);

   while (j >= 0) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j < 0) {
      	j = s.find('\0',j);
         v.push_back(s.substr(i, j-i));
      }
   }
}

#endif