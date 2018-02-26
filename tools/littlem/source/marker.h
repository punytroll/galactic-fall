#ifndef LITTLEM_MARKER_H
#define LITTLEM_MARKER_H

#include <string>

#include <algebra/vector3f.h>

class Marker
{
public:
	const std::string & GetIdentifier(void) const
	{
		return _Identifier;
	}
	
	const Vector3f & GetPosition(void) const
	{
		return _Position;
	}
	
	void SetIdentifier(const std::string & Identifier)
	{
		_Identifier = Identifier;
	}
	
	void SetPosition(const Vector3f & Position)
	{
		_Position = Position;
	}
private:
	std::string _Identifier;
	Vector3f _Position;
};

#endif
