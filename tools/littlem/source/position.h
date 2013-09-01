#ifndef LITTLEM_POSITION_H
#define LITTLEM_POSITION_H

#include <algebra/vector3f.h>

#include "object.h"

class Position : public Object
{
public:
	Position(void);
	Position(float X, float Y, float Z);
	Position(const Position & Position);
	virtual ~Position(void);
	virtual void Draw(void);
	virtual void DrawSelection(void);
	
	const Vector3f & GetPosition(void) const
	{
		return _Position;
	}
	
	float GetX(void) const
	{
		return _Position[0];
	}
	
	float GetY(void) const
	{
		return _Position[1];
	}
	
	float GetZ(void) const
	{
		return _Position[2];
	}
	
	void SetPosition(float X, float Y, float Z)
	{
		_Position[0] = X;
		_Position[1] = Y;
		_Position[2] = Z;
	}
	
	void SetPosition(const Vector3f & Position)
	{
		_Position = Position;
	}
	
	void SetX(float X)
	{
		_Position[0] = X;
	}
	
	void SetY(float Y)
	{
		_Position[1] = Y;
	}
	
	void SetZ(float Z)
	{
		_Position[2] = Z;
	}
private:
	Vector3f _Position;
};

#endif
