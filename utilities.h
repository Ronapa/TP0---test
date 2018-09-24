#ifndef UTILITIES_H
#define UTILITIES_H

#include <istream>

#define MSG_NO_DATA "NO_DATA"
#define MSG_BAD_QUERY "BAD_QUERY"
#define MSG_UNKNOWN_ID "UNKNOWN ID"
#define INVALID_TEMPERATURE -274

void _split(const string& s, const char c, Array<string>& v);

#endif