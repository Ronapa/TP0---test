#ifndef SENSOR_CC
#define SENSOR_CC

#include <iostream>
#include <string>
#include "Array.h"
#include "sensor.h"

sensor::sensor()
{

}

sensor::sensor(const string &name)
{
	sensor_name = name;
}

sensor::~sensor()
{
	cout << "destrui el sensor: " << sensor_name << endl;
}


void sensor::set_sensor_name(const string &name)
{
	sensor_name = name;
}

void sensor::add_temperature_to_sensor(const float & value)
{
	temperature_values.push_back(value);
}

size_t sensor::get_amount_of_temperature_measures()
{
	return temperature_values.size();
}

float sensor::get_average_temperature_in_range(const int & left, const int & right)
{
	if(left < 0 || right < 0)
	{
		return -273;
	}
	float accum=0;
	int i=left;
	int amount_of_values = 0;
	while (i <= right && (size_t)i < temperature_values.size())
	{
		if (temperature_values[i] != -273)
		{
			accum += temperature_values[i];
			amount_of_values++;
		}
		i++;
	}
	if (accum == 0)
	{
		return -273;
	}
	return accum/amount_of_values;
}

float sensor::get_min_temperature_in_range(const int &left , const int &right)
{
	if(left < 0 || right < 0 || left > temperature_values.size())
	{
		return -273;
	}
	float aux;
	int amount_of_values = 0;
	if ((aux = temperature_values[left]) != -273)
	{
		amount_of_values++;
	}
	int i=left+1;
	while (i <= right && (size_t)i < temperature_values.size())
	{
		if (temperature_values[i] < aux && temperature_values[i] != -273)
		{
			aux = temperature_values[i];
			amount_of_values++;
		}
		i++;
	}
	if (amount_of_values == 0)
	{
		return -273;
	}else
	{
		return aux;		
	}

}

float sensor::get_max_temperature_in_range(const int &left , const int &right)
{
	if(left < 0 || right < 0 || left > temperature_values.size())
	{
		return -273;
	}
	float aux;
	int amount_of_values = 0;
	if ((aux = temperature_values[left]) != -273)
	{
		amount_of_values++;
	}
	int i=left+1;
	while (i <= right && (size_t)i < temperature_values.size())
	{
		if (temperature_values[i] > aux && temperature_values[i] != -273)
		{
			aux = temperature_values[i];
			amount_of_values++;
		}
		i++;
	}
	if (amount_of_values == 0)
	{
		return -273;
	}else
	{
		return aux;		
	}
}

float sensor::get_temperature_at(const int &index)
{
	if (index < 0 || (size_t)index >= temperature_values.size())
	{
		return -273;
	}
	return temperature_values[index];
	
}

string sensor::get_sensor_name()
{
	return sensor_name;
}

int sensor::get_amount_of_valid_temperatures_in_range(const int &left , const int &right)
{
	int aux = 0 , i=left;
	if (left < 0 || right <0)
	{
		return -1;
	}
	while (i <= right && (size_t)i<temperature_values.size())
	{
		if(temperature_values[i] != -273)
		{
			aux++;
		}
		i++;
	}
	return aux;
}


#endif
