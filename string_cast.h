#ifndef STRING_CAST_H
#define STRING_CAST_H

template < typename Type >
Type string_cast(const std::string & String);

template < typename Type >
std::string to_string_cast(const Type & Value);

#endif
