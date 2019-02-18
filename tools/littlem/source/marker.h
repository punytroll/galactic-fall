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
	
	float GetLength(void) const
	{
		assert(_Length);
		
		return _Length.value();
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
	
	bool HasLength(void) const
	{
		return (bool)_Length;
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
	
	void SetLength(float Length)
	{
		_Length = Length;
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
	std::experimental::optional< float > _Length;
	std::experimental::optional< Quaternion > _Orientation;
	std::experimental::optional< Vector3f > _Position;
};

#endif
