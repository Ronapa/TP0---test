#include <istream>
#include <string>
#include "Array.h"


void _split(const string& s, const char c , Array<string>& v) 
{
   	int i = 0;
   	int j = s.find(c);

  	while (j >= 0) 
  	{
      	v.push_back(s.substr(i, j-i));
      	i = ++j;
      	j = s.find(c, j);

      	if (j < 0) 
      	{
         	v.push_back(s.substr(i, s.length()));
     	}
   	}
}