#include <GL/gl.h>

#include "position.h"

Position::Position(void) :
	_Position(Vector3f::CreateZero())
{
}

Position::Position(float X, float Y, float Z) :
	_Position(Vector3f::CreateFromComponents(X, Y, Z))
{
}

Position::Position(const Position & Position) :
	_Position(Position._Position)
{
}

Position::~Position(void)
{
}

void Position::Draw(void)
{
	glVertex3fv(_Position.GetPointer());
}

void Position::DrawSelection(void)
{
	glVertex3fv(_Position.GetPointer());
}
