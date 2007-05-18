#include "buffer_reading.h"

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Color & Color)
{
	float Red;
	float Green;
	float Blue;
	float Alpha;
	
	BufferReader >> Red >> Green >> Blue >> Alpha;
	Color.Set(Red, Green, Blue, Alpha);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, math3d::vector2f & Vector)
{
	float x;
	float y;
	
	BufferReader >> x >> y;
	Vector.set(x, y);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, math3d::vector3f & Vector)
{
	float x;
	float y;
	float z;
	
	BufferReader >> x >> y >> z;
	Vector.set(x, y, z);
	
	return BufferReader;
}
