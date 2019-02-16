#ifndef LITTLEM_MARKER_H
#define LITTLEM_MARKER_H

#include <experimental/optional>
#include <string>

#include <algebra/quaternion.h>
#include <algebra/vector3f.h>

class Marker
{
public:
	const std::string & GetIdentifier(void) const
	{
		return _Identifier;
	}
	
	const Quaternion & GetOrientation(void) const
	{
		assert(_Orientation);
		
		return _Orientation.value();
	}
	
	const Vector3f & GetPosition(void) const
	{
		assert(_Position);
		
		return _Position.value();
	}
	
	bool HasOrientation(void) const
	{
		return (bool)_Orientation;
	}
	
	bool HasPosition(void) const
	{
		return (bool)_Position;
	}
	
	void SetIdentifier(const std::string & Identifier)
	{
		_Identifier = Identifier;
	}
	
	void SetOrientation(const Quaternion & Orientation)
	{
		_Orientation = Orientation;
	}
	
	void SetPosition(const Vector3f & Position)
	{
		_Position = Position;
	}
private:
	std::string _Identifier;
	std::experimental::optional< Quaternion > _Orientation;
	std::experimental::optional< Vector3f > _Position;
};

#endif
