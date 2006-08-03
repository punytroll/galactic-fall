#include <sstream>

#include "string_cast.h"

template < >
unsigned long string_cast< unsigned long >(const std::string & String)
{
	std::istringstream StringStream(String);
	unsigned long Result;
	
	StringStream >> Result;
	
	return Result;
}

template < >
float string_cast< float >(const std::string & String)
{
	std::istringstream StringStream(String);
	float Result;
	
	StringStream >> Result;
	
	return Result;
}

template < >
bool string_cast< bool >(const std::string & String)
{
	return String == "true";
}

template < >
std::string to_string_cast< unsigned int >(const unsigned int & Value)
{
	std::ostringstream StringStream;
	
	StringStream << Value;
	
	return StringStream.str();
}

template < >
std::string to_string_cast< float >(const float & Value)
{
	std::ostringstream StringStream;
	
	StringStream << Value;
	
	return StringStream.str();
}

