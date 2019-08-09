#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <algorithm>
#include <experimental/optional>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

#include <algebra/matrix3f.h>
#include <algebra/matrix4f.h>
#include <algebra/quaternion.h>
#include <algebra/vector2f.h>
#include <algebra/vector3f.h>
#include <algebra/vector4f.h>

#include <string_cast/string_cast.h>

#include <xml_parser/xml_parser.h>

#include <xml_stream/xml_stream.h>

// types of objects
#define LITTLEM_CAMERA 0
#define LITTLEM_LIGHT 1
#define LITTLEM_POINT 2
#define LITTLEM_TRIANGLE 3
#define LITTLEM_MARKER 4

#ifndef NDEBUG
#define ON_DEBUG(A) (A)
#else
#define ON_DEBUG(A)
#endif

enum class FixedAxis
{
	NegativeX,
	NegativeY,
	NegativeZ,
	PositiveX,
	PositiveY,
	PositiveZ
};

std::string GetAxisString(FixedAxis Axis)
{
	switch(Axis)
	{
	case FixedAxis::NegativeX:
		{
			return "-X";
		}
	case FixedAxis::NegativeY:
		{
			return "-Y";
		}
	case FixedAxis::NegativeZ:
		{
			return "-Z";
		}
	case FixedAxis::PositiveX:
		{
			return "+X";
		}
	case FixedAxis::PositiveY:
		{
			return "+Y";
		}
	case FixedAxis::PositiveZ:
		{
			return "+Z";
		}
	}
	assert(false);
	exit(1);
}

enum class FixedDirection
{
	Backward,
	Down,
	Forward,
	Left,
	Right,
	Up
};

const Vector3f & GetDirectionVector(FixedDirection Direction)
{
	static auto Backward(Vector3f::CreateTranslationZ(1.0f));
	static auto Down(Vector3f::CreateTranslationY(-1.0f));
	static auto Forward(Vector3f::CreateTranslationZ(-1.0f));
	static auto Left(Vector3f::CreateTranslationX(-1.0f));
	static auto Right(Vector3f::CreateTranslationX(1.0f));
	static auto Up(Vector3f::CreateTranslationY(1.0f));
	
	switch(Direction)
	{
	case FixedDirection::Backward:
		{
			return Backward;
		}
	case FixedDirection::Down:
		{
			return Down;
		}
	case FixedDirection::Forward:
		{
			return Forward;
		}
	case FixedDirection::Left:
		{
			return Left;
		}
	case FixedDirection::Right:
		{
			return Right;
		}
	case FixedDirection::Up:
		{
			return Up;
		}
	}
	assert(false);
	exit(1);
}

enum class FixedView
{
	Front,
	Left,
	Back,
	Right,
	Top,
	Bottom
};

std::string GetFixedViewString(FixedView View)
{
	switch(View)
	{
	case FixedView::Back:
		{
			return "Back";
		}
	case FixedView::Bottom:
		{
			return "Bottom";
		}
	case FixedView::Front:
		{
			return "Front";
		}
	case FixedView::Left:
		{
			return "Left";
		}
	case FixedView::Right:
		{
			return "Right";
		}
	case FixedView::Top:
		{
			return "Top";
		}
	}
	assert(false);
	exit(1);
}

enum class MouseButton
{
	Undefined,
	Left,
	Middle,
	Right,
	WheelDown,
	WheelUp
};

enum class ModelerView
{
	Camera,
	Model,
	Point,
	Triangle,
	Marker
};

std::string GetModelerViewString(ModelerView View)
{
	switch(View)
	{
	case ModelerView::Camera:
		{
			return "Camera";
		}
	case ModelerView::Model:
		{
			return "Model";
		}
	case ModelerView::Point:
		{
			return "Point";
		}
	case ModelerView::Triangle:
		{
			return "Triangle";
		}
	case ModelerView::Marker:
		{
			return "Marker";
		}
	}
	assert(false);
	exit(1);
}

class Keyboard
{
public:
	Keyboard(void)
	{
		for(auto & IsDown : _Keys)
		{
			IsDown = false;
		}
	}
	
	bool IsAnyShiftActive(void) const
	{
		return (_Keys[50] == true) || (_Keys[62] == true);
	}

	bool IsAnyControlActive(void) const
	{
		return (_Keys[37] == true) || (_Keys[105] == true);
	}

	bool IsAltActive(void) const
	{
		return _Keys[64] == true;
	}
	
	bool IsNoModifierKeyActive(void) const
	{
		return (_Keys[50] == false) && (_Keys[62] == false) && (_Keys[37] == false) && (_Keys[105] == false) && (_Keys[64] == false);
	}
	
	void SetKey(int KeyCode, bool IsDown)
	{
		assert((KeyCode >= 0) && (KeyCode < 256));
		_Keys[KeyCode] = IsDown;
	}
private:
	bool _Keys[256];
};

FixedAxis g_UpAxis;
FixedAxis g_ForwardAxis;
ModelerView g_ModelerView;
float g_Width(800.0f);
float g_Height(800.0f);
Keyboard g_Keyboard;
bool g_CullPoints(false);

#include "marker.h"
#include "point.h"
#include "triangle.h"
#include "triangle_point.h"

XMLStream & operator<<(XMLStream & XMLStream, const Marker & Marker)
{
	XMLStream << element << "marker";
	XMLStream << attribute << "identifier" << value << Marker.GetIdentifier();
	if(Marker.HasLength() == true)
	{
		XMLStream << element << "length";
		XMLStream << attribute << "value" << value << Marker.GetLength();
		XMLStream << end;
	}
	if(Marker.HasOrientation() == true)
	{
		XMLStream << element << "orientation";
		XMLStream << attribute << "w" << value << Marker.GetOrientation()[0];
		XMLStream << attribute << "x" << value << Marker.GetOrientation()[1];
		XMLStream << attribute << "y" << value << Marker.GetOrientation()[2];
		XMLStream << attribute << "z" << value << Marker.GetOrientation()[3];
		XMLStream << end;
	}
	if(Marker.HasPosition() == true)
	{
		XMLStream << element << "position";
		XMLStream << attribute << "x" << value << Marker.GetPosition()[0];
		XMLStream << attribute << "y" << value << Marker.GetPosition()[1];
		XMLStream << attribute << "z" << value << Marker.GetPosition()[2];
		XMLStream << end;
	}
	
	return XMLStream;
}

class MeshReader : public XMLParser
{
public:
	MeshReader(std::istream & InputStream) :
		XMLParser(InputStream),
		_InMesh(false),
		_CurrentMarker(nullptr),
		_CurrentTrianglePoint(nullptr),
		_CurrentTriangle(nullptr),
		_TrianglePoint(0)
	{
	}
	
	virtual ~MeshReader(void)
	{
	}
	
	virtual void ElementStart(const std::string & ElementName, const std::map< std::string, std::string > & Attributes) override
	{
		// safe-guard: only accept geometry input if we entered a mesh definition
		if(ElementName == "mesh")
		{
			assert(_InMesh == false);
			_InMesh = true;
		}
		else
		{
			if(_InMesh == false)
			{
				return;
			}
		}
		// now we have confirmed that we in a model definition
		if(ElementName == "point")
		{
			if(_CurrentTrianglePoint == nullptr)
			{
				auto CurrentPoint(new Point());
				
				assert(Attributes.find("position-x") != Attributes.end());
				assert(Attributes.find("position-y") != Attributes.end());
				assert(Attributes.find("position-z") != Attributes.end());
				assert(Attributes.find("identifier") != Attributes.end());
				CurrentPoint->SetPosition(Vector3f::CreateFromComponents(from_string_cast< float >(Attributes.find("position-x")->second), from_string_cast< float >(Attributes.find("position-y")->second), from_string_cast< float >(Attributes.find("position-z")->second)));
				_Points[from_string_cast< unsigned long >(Attributes.find("identifier")->second)] = CurrentPoint;
			}
			else
			{
				_CurrentTrianglePoint->SetPoint(_Points[from_string_cast< unsigned long >(Attributes.find("point-identifier")->second)]);
			}
		}
		else if(ElementName == "triangle-point")
		{
			if(_CurrentTriangle == nullptr)
			{
				assert(_CurrentTrianglePoint == nullptr);
				_CurrentTrianglePoint = new TrianglePoint();
				_TrianglePoints[from_string_cast< unsigned long >(Attributes.find("identifier")->second)] = _CurrentTrianglePoint;
				_CurrentTrianglePoint->_Normal.Set(from_string_cast< float >(Attributes.find("normal-x")->second), from_string_cast< float >(Attributes.find("normal-y")->second), from_string_cast< float >(Attributes.find("normal-z")->second));
			}
			else
			{
				if(_TrianglePoint == 0)
				{
					_CurrentTriangle->SetTrianglePoint<0>(_TrianglePoints[from_string_cast< unsigned long >(Attributes.find("triangle-point-identifier")->second)]);
				}
				else if(_TrianglePoint == 1)
				{
					_CurrentTriangle->SetTrianglePoint<1>(_TrianglePoints[from_string_cast< unsigned long >(Attributes.find("triangle-point-identifier")->second)]);
				}
				else if(_TrianglePoint == 2)
				{
					_CurrentTriangle->SetTrianglePoint<2>(_TrianglePoints[from_string_cast< unsigned long >(Attributes.find("triangle-point-identifier")->second)]);
				}
				++_TrianglePoint;
			}
		}
		else if(ElementName == "triangle")
		{
			assert(_CurrentTriangle == nullptr);
			_CurrentTriangle = new Triangle();
			_Triangles[from_string_cast< unsigned long >(Attributes.find("identifier")->second)] = _CurrentTriangle;
		}
		else if(ElementName == "marker")
		{
			assert(_CurrentMarker == nullptr);
			_CurrentMarker = new Marker{};
			assert(Attributes.find("identifier") != Attributes.end());
			_CurrentMarker->SetIdentifier(Attributes.find("identifier")->second);
			_Markers.push_back(_CurrentMarker);
		}
		else if(ElementName == "orientation")
		{
			assert(_CurrentMarker != nullptr);
			assert(Attributes.find("w") != Attributes.end());
			assert(Attributes.find("x") != Attributes.end());
			assert(Attributes.find("y") != Attributes.end());
			assert(Attributes.find("z") != Attributes.end());
			_CurrentMarker->SetOrientation(Quaternion{from_string_cast< float >(Attributes.find("w")->second), from_string_cast< float >(Attributes.find("x")->second), from_string_cast< float >(Attributes.find("y")->second), from_string_cast< float >(Attributes.find("z")->second)});
		}
		else if(ElementName == "position")
		{
			assert(_CurrentMarker != nullptr);
			assert(Attributes.find("x") != Attributes.end());
			assert(Attributes.find("y") != Attributes.end());
			assert(Attributes.find("z") != Attributes.end());
			_CurrentMarker->SetPosition(Vector3f::CreateFromComponents(from_string_cast< float >(Attributes.find("x")->second), from_string_cast< float >(Attributes.find("y")->second), from_string_cast< float >(Attributes.find("z")->second)));
		}
		else if(ElementName == "length")
		{
			assert(_CurrentMarker != nullptr);
			assert(Attributes.find("value") != Attributes.end());
			_CurrentMarker->SetLength(from_string_cast< float >(Attributes.find("value")->second));
		}
	}
	
	virtual void ElementEnd(const std::string & ElementName) override
	{
		if(ElementName == "mesh")
		{
			assert(_InMesh == true);
			_InMesh = false;
			std::cout << "Read " << _Points.size() << " points from mesh." << std::endl;
			std::cout << "Read " << _TrianglePoints.size() << " triangle points from mesh." << std::endl;
			std::cout << "Read " << _Triangles.size() << " triangles from mesh." << std::endl;
			std::cout << "Read " << _Markers.size() << " markers from mesh." << std::endl;
		}
		else if(ElementName == "triangle")
		{
			_CurrentTriangle = nullptr;
			_TrianglePoint = 0;
		}
		else if(ElementName == "triangle-point")
		{
			_CurrentTrianglePoint = nullptr;
		}
		else if(ElementName == "marker")
		{
			_CurrentMarker = nullptr;
		}
	}
	
	std::vector< Point * > GetPoints(void)
	{
		std::vector< Point * > Result;
		std::map< unsigned long, Point * >::iterator Point(_Points.begin());
		
		while(Point != _Points.end())
		{
			Result.push_back(Point->second);
			++Point;
		}
		
		return Result;
	}
	
	std::vector< Triangle * > GetTriangles(void)
	{
		std::vector< Triangle * > Result;
		std::map< unsigned long, Triangle * >::iterator Triangle(_Triangles.begin());
		
		while(Triangle != _Triangles.end())
		{
			Result.push_back(Triangle->second);
			++Triangle;
		}
		
		return Result;
	}
	
	std::vector< Marker * > & GetMarkers(void)
	{
		return _Markers;
	}
private:
	bool _InMesh;
	Marker * _CurrentMarker;
	TrianglePoint * _CurrentTrianglePoint;
	Triangle * _CurrentTriangle;
	int _TrianglePoint;
	std::map< unsigned long, Point * > _Points;
	std::map< unsigned long, Triangle * > _Triangles;
	std::map< unsigned long, TrianglePoint * > _TrianglePoints;
	std::vector< Marker * > _Markers;
};

class Light;

Light * g_HoveredLight = nullptr;
Light * g_SelectedLight = nullptr;

class Light
{
public:
	Light(void) :
		_LightNumber(0)
	{
		_Lights.push_back(this);
	}
	
	~Light(void)
	{
		if(g_HoveredLight == this)
		{
			g_HoveredLight = nullptr;
		}
		if(g_SelectedLight == this)
		{
			g_SelectedLight = nullptr;
		}
		Disable();
		_Lights.erase(std::find(_Lights.begin(), _Lights.end(), this));
	}
	
	static void InitializeFreeLights(int NumberOfLights)
	{
		for(int LightIndex = 0; LightIndex < NumberOfLights; ++LightIndex)
		{
			Light::_FreeLights.push_back(GL_LIGHT0 + LightIndex);
		}
	}
	
	void DrawLight(void)
	{
		Vector4f LightPosition(_Position[0], _Position[1], _Position[2], 1.0f);
		
		glLightfv(_LightNumber, GL_POSITION, LightPosition.GetPointer());
	}
	
	void DrawPoint(void)
	{
	}
	
	const Vector4f & GetDiffuseColor(void) const
	{
		return _DiffuseColor;
	}
	
	static const std::vector< Light * > & GetEnabledLights(void)
	{
		return _EnabledLights;
	}
	
	static const std::vector< Light * > & GetLights(void)
	{
		return _Lights;
	}
	
	const Vector3f & GetPosition(void) const
	{
		return _Position;
	}
	
	void SetDiffuseColor(float Red, float Green, float Blue, float Alpha = 0.0f)
	{
		_DiffuseColor[0] = Red;
		_DiffuseColor[1] = Green;
		_DiffuseColor[2] = Blue;
		_DiffuseColor[3] = Alpha;
		if(_LightNumber != 0)
		{
			glLightfv(_LightNumber, GL_DIFFUSE, _DiffuseColor.GetPointer());
		}
	}
	
	void SetPosition(const Vector3f & Position)
	{
		_Position = Position;
	}
	
	bool IsEnabled(void) const
	{
		return _LightNumber != 0;
	}
	
	void Enable(void)
	{
		if(IsEnabled() == false)
		{
			if(_FreeLights.size() > 0)
			{
				auto LightNumber{_FreeLights.front()};
				
				_FreeLights.pop_front();
				_EnabledLights.push_back(this);
				_LightNumber = LightNumber;
				glEnable(_LightNumber);
				glLightfv(_LightNumber, GL_DIFFUSE, _DiffuseColor.GetPointer());
			}
		}
	}
	
	void Disable(void)
	{
		if(IsEnabled() == true)
		{
			auto EnabledLightIterator(std::find(_EnabledLights.begin(), _EnabledLights.end(), this));
			
			_EnabledLights.erase(EnabledLightIterator);
			glDisable(_LightNumber);
			_FreeLights.push_back(_LightNumber);
			_LightNumber = 0;
		}
	}
	
	static void DeleteLights(void)
	{
		while(_Lights.size() > 0)
		{
			delete _Lights.back();
		}
	}
private:
	static std::deque< int > _FreeLights;
	static std::vector< Light * > _Lights;
	static std::vector< Light * > _EnabledLights;
	Vector4f _DiffuseColor;
	GLenum _LightNumber;
	Vector3f _Position;
};

std::deque< int > Light::_FreeLights;
std::vector< Light * > Light::_Lights;
std::vector< Light * > Light::_EnabledLights;

class Camera
{
public:
	Camera(void) :
		_FieldOfViewY(1.0f),
		_Orientation(Quaternion::CreateIdentity()),
		_Position(Vector3f::CreateZero())
	{
	}
	
	float GetFieldOfViewY(void) const
	{
		return _FieldOfViewY;
	}
	
	const Quaternion & GetOrientation(void) const
	{
		return _Orientation;
	}
	
	const Vector3f & GetPosition(void) const
	{
		return _Position;
	}
	
	Matrix4f GetProjectionMatrix(void) const
	{
		float Aspect(static_cast< float >(g_Width) / static_cast< float >(g_Height));
		float NearClippingPlane(0.0001f);
		float FarClippingPlane(1000.0f);
		Matrix4f Result;
		float Top(NearClippingPlane * tan(_FieldOfViewY / 2.0f));
		float Right(Top * Aspect);
		
		Result[0] = NearClippingPlane / Right;
		Result[1] = 0.0f;
		Result[2] = 0.0f;
		Result[3] = 0.0f;
		Result[4] = 0.0f;
		Result[5] = NearClippingPlane / Top;
		Result[6] = 0.0f;
		Result[7] = 0.0f;
		Result[8] = 0.0f;
		Result[9] = 0.0f;
		Result[10] = -(FarClippingPlane + NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
		Result[11] = -1.0f;
		Result[12] = 0.0f;
		Result[13] = 0.0f;
		Result[14] = -(2.0f * FarClippingPlane * NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
		Result[15] = 0.0f;
		
		return Result;
	}
	
	Matrix4f GetSpacialMatrix(void)
	{
		return Matrix4f::CreateTranslation(_Position).Rotate(_Orientation);
	}
	
	void MoveBackward(float Amount)
	{
		_Position.Translate(Vector3f::CreateTranslationZ(Amount).Rotate(_Orientation));
	}
	
	void MoveDown(float Amount)
	{
		_Position.Translate(Vector3f::CreateTranslationY(-Amount).Rotate(_Orientation));
	}
	
	void MoveForward(float Amount)
	{
		_Position.Translate(Vector3f::CreateTranslationZ(-Amount).Rotate(_Orientation));
	}
	
	void MoveLeft(float Amount)
	{
		_Position.Translate(Vector3f::CreateTranslationX(-Amount).Rotate(_Orientation));
	}
	
	void MoveRight(float Amount)
	{
		_Position.Translate(Vector3f::CreateTranslationX(Amount).Rotate(_Orientation));
	}
	
	void MoveUp(float Amount)
	{
		_Position.Translate(Vector3f::CreateTranslationY(Amount).Rotate(_Orientation));
	}
	
	void SetFieldOfViewY(float FieldOfViewY)
	{
		_FieldOfViewY = FieldOfViewY;
	}
	
	void SetOrientation(const Quaternion & Orientation)
	{
		_Orientation = Orientation;
	}
	
	void SetPosition(const Vector3f & Position)
	{
		_Position = Position;
	}
	
	void TurnDown(float Amount)
	{
		_Orientation.RotateX(-Amount);
	}
	
	void TurnLeft(float Amount)
	{
		_Orientation.RotateY(Amount);
	}
	
	void TurnRight(float Amount)
	{
		_Orientation.RotateY(-Amount);
	}
	
	void TurnUp(float Amount)
	{
		_Orientation.RotateX(Amount);
	}
private:
	float _FieldOfViewY;
	Quaternion _Orientation;
	Vector3f _Position;
};

class CameraDescription
{
public:
	CameraDescription(void)
	{
	}
	
	CameraDescription(Camera * Camera) :
		Position(Camera->GetPosition()),
		Orientation(Camera->GetOrientation()),
		FieldOfViewY(Camera->GetFieldOfViewY())
	{
	}
	
	Vector3f Position;
	Quaternion Orientation;
	float FieldOfViewY;
};

CameraDescription camera(Camera * Camera)
{
	return CameraDescription(Camera);
}

XMLStream & operator<<(XMLStream & XMLStream, const CameraDescription & CameraDescription)
{
	XMLStream << element << "camera";
	XMLStream << attribute << "position-x" << value << CameraDescription.Position[0];
	XMLStream << attribute << "position-y" << value << CameraDescription.Position[1];
	XMLStream << attribute << "position-z" << value << CameraDescription.Position[2];
	XMLStream << attribute << "orientation-w" << value << CameraDescription.Orientation[0];
	XMLStream << attribute << "orientation-x" << value << CameraDescription.Orientation[1];
	XMLStream << attribute << "orientation-y" << value << CameraDescription.Orientation[2];
	XMLStream << attribute << "orientation-z" << value << CameraDescription.Orientation[3];
	XMLStream << attribute << "field-of-view" << value << CameraDescription.FieldOfViewY;
	
	return XMLStream;
}

class LightDescription
{
public:
	LightDescription(void)
	{
	}
	
	LightDescription(Light * pLight) :
		m_Position(pLight->GetPosition()),
		m_DiffuseColor(pLight->GetDiffuseColor()),
		m_bEnabled(pLight->IsEnabled())
	{
	}
	
	Vector3f m_Position;
	Vector4f m_DiffuseColor;
	bool m_bEnabled;
};

LightDescription light(Light * pLight)
{
	return LightDescription(pLight);
}

XMLStream & operator<<(XMLStream & XMLStream, const LightDescription & LightDescription)
{
	XMLStream << element << "light";
	XMLStream << attribute << "enabled" << value << LightDescription.m_bEnabled;
	XMLStream << attribute << "position-x" << value << LightDescription.m_Position[0] << attribute << "position-y" << value << LightDescription.m_Position[1] << attribute << "position-z" << value << LightDescription.m_Position[2];
	XMLStream << attribute << "color-red" << value << LightDescription.m_DiffuseColor[0] << attribute << "color-green" << value << LightDescription.m_DiffuseColor[1] << attribute << "color-blue" << value << LightDescription.m_DiffuseColor[2] << attribute << "color-alpha" << value << LightDescription.m_DiffuseColor[3];
	
	return XMLStream;
}

class SceneReader : public MeshReader
{
public:
	SceneReader(std::istream & InputStream) :
		MeshReader(InputStream)
	{
	}
	
	virtual void ElementStart(const std::string & sElementName, const std::map< std::string, std::string > & Attributes) override
	{
		if(sElementName == "light")
		{
			LightDescription LightDescription;
			
			LightDescription.m_Position = Vector3f::CreateFromComponents(from_string_cast< float >(Attributes.find("position-x")->second), from_string_cast< float >(Attributes.find("position-y")->second), from_string_cast< float >(Attributes.find("position-z")->second));
			LightDescription.m_DiffuseColor = Vector4f(from_string_cast< float >(Attributes.find("color-red")->second), from_string_cast< float >(Attributes.find("color-green")->second), from_string_cast< float >(Attributes.find("color-blue")->second), from_string_cast< float >(Attributes.find("color-alpha")->second));
			LightDescription.m_bEnabled = from_string_cast< bool >(Attributes.find("enabled")->second);
			_LightDescriptions.push_back(LightDescription);
		}
		else if(sElementName == "camera")
		{
			CameraDescription CameraDescription;
			
			CameraDescription.Position = Vector3f::CreateFromComponents(from_string_cast< float >(Attributes.find("position-x")->second), from_string_cast< float >(Attributes.find("position-y")->second), from_string_cast< float >(Attributes.find("position-z")->second));
			CameraDescription.Orientation = Quaternion(from_string_cast< float >(Attributes.find("orientation-w")->second), from_string_cast< float >(Attributes.find("orientation-x")->second), from_string_cast< float >(Attributes.find("orientation-y")->second), from_string_cast< float >(Attributes.find("orientation-z")->second));
			CameraDescription.FieldOfViewY = from_string_cast< float >(Attributes.find("field-of-view")->second);
			_CameraDescriptions.push_back(CameraDescription);
		}
		else
		{
			MeshReader::ElementStart(sElementName, Attributes);
		}
	}
	
	virtual void ElementEnd(const std::string & sElementName) override
	{
		MeshReader::ElementEnd(sElementName);
	}
	
	const std::vector< CameraDescription > & GetCameraDescriptions(void) const
	{
		return _CameraDescriptions;
	}
	
	const std::vector< LightDescription > & GetLightDescriptions(void) const
	{
		return _LightDescriptions;
	}
private:
	std::vector< CameraDescription > _CameraDescriptions;
	std::vector< LightDescription > _LightDescriptions;
};

Display * g_Display;
GLXContext g_GLXContext;
Window g_Window;
Colormap g_ColorMap;
std::vector< Marker * > g_Markers;
std::vector< Point * > g_Points;
std::vector< Triangle * > g_Triangles;
std::vector< Point * > g_SelectedPoints;
std::vector< Triangle * > g_SelectedTriangles;
Point * g_HoveredPoint = 0;
Triangle * g_HoveredTriangle = 0;
std::vector< Camera * > g_Cameras;
Camera * g_HoveredCamera = 0;
Camera * g_SelectedCamera = 0;
Camera * g_CurrentCamera = 0;
Marker * g_HoveredMarker = nullptr;
Marker * g_SelectedMarker = nullptr;
bool g_bMoved = false;
bool g_Quit(false);
GLuint g_puiSelectionBuffer[1024];
bool g_Snapping = false;
float g_SnapFactor = 0.1;
MouseButton g_MouseButton(MouseButton::Undefined);
Vector2f g_MousePosition;
Vector2f g_LastMousePosition;

#define CHARACTERS 95
#define CHARACTEROFFSET 32

const GLubyte g_ppuLetters[][12] = 
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 00 => ' '
	{ 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00 }, // 01 => '!'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x50, 0x50, 0x00 }, // 02 => '"'
	{ 0x00, 0x00, 0x00, 0x50, 0x50, 0xF8, 0x50, 0xF8, 0x50, 0x50, 0x00, 0x00 }, // 03 => '#'
	{ 0x00, 0x00, 0x00, 0x20, 0xF0, 0x28, 0x70, 0xA0, 0x78, 0x20, 0x00, 0x00 }, // 04 => '$'
	{ 0x00, 0x00, 0x00, 0x18, 0x98, 0x40, 0x20, 0x10, 0xC8, 0xC0, 0x00, 0x00 }, // 05 => '%'
	{ 0x00, 0x00, 0x00, 0x68, 0x90, 0xA8, 0x40, 0x80, 0x80, 0x70, 0x00, 0x00 }, // 06 => '&'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x00 }, // 07 => '''
	{ 0x00, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x08 }, // 08 => '('
	{ 0x00, 0x40, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x40 }, // 09 => ')'
	{ 0x00, 0x00, 0x00, 0x00, 0x20, 0xA8, 0x70, 0xA8, 0x20, 0x00, 0x00, 0x00 }, // 10 => '*'
	{ 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x00, 0x00, 0x00 }, // 11 => '+'
	{ 0x00, 0x40, 0x20, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 12 => ','
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 13 => '-'
	{ 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 14 => '.'
	{ 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08 }, // 15 => '/'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0xC8, 0xA8, 0x98, 0x88, 0x70, 0x00 }, // 16 => '0'
	{ 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x10, 0x00 }, // 17 => '1'
	{ 0x00, 0x00, 0x00, 0xF8, 0x80, 0x40, 0x20, 0x10, 0x08, 0x88, 0x70, 0x00 }, // 18 => '2'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x08, 0x08, 0x30, 0x08, 0x88, 0x70, 0x00 }, // 19 => '3'
	{ 0x00, 0x00, 0x00, 0x38, 0x10, 0xF8, 0x50, 0x50, 0x30, 0x30, 0x10, 0x00 }, // 20 => '4'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x08, 0x08, 0xF0, 0x80, 0x80, 0xF8, 0x00 }, // 21 => '5'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x40, 0x30, 0x00 }, // 22 => '6'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x88, 0xF8, 0x00 }, // 23 => '7'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x88, 0x88, 0x70, 0x00 }, // 24 => '8'
	{ 0x00, 0x00, 0x00, 0x60, 0x10, 0x08, 0x78, 0x88, 0x88, 0x88, 0x70, 0x00 }, // 25 => '9'
	{ 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00 }, // 26 => ':'
	{ 0x00, 0x40, 0x20, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00 }, // 27 => ';'
	{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0xC0, 0x30, 0x0C, 0x00, 0x00, 0x00 }, // 28 => '<'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00 }, // 29 => '='
	{ 0x00, 0x00, 0x00, 0x00, 0xC0, 0x30, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00 }, // 30 => '>'
	{ 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x20, 0x10, 0x08, 0x88, 0x70, 0x00 }, // 31 => '?'
	{ 0x00, 0x00, 0x00, 0x70, 0x80, 0xB0, 0xB8, 0xB8, 0x88, 0x70, 0x00, 0x00 }, // 32 => '@'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x50, 0x20, 0x00, 0x00 }, // 33 => 'A'
	{ 0x00, 0x00, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0x88, 0x88, 0xF0, 0x00, 0x00 }, // 34 => 'B'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x80, 0x80, 0x80, 0x88, 0x70, 0x00, 0x00 }, // 35 => 'C'
	{ 0x00, 0x00, 0x00, 0xE0, 0x90, 0x88, 0x88, 0x88, 0x90, 0xE0, 0x00, 0x00 }, // 36 => 'D'
	{ 0x00, 0x00, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x00, 0x00 }, // 37 => 'E'
	{ 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x00, 0x00 }, // 38 => 'F'
	{ 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0x98, 0x80, 0x88, 0x70, 0x00, 0x00 }, // 39 => 'G'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x00, 0x00 }, // 40 => 'H'
	{ 0x00, 0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00 }, // 41 => 'I'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00 }, // 42 => 'J'
	{ 0x00, 0x00, 0x00, 0x88, 0x90, 0xA0, 0xC0, 0xA0, 0x90, 0x88, 0x00, 0x00 }, // 43 => 'K'
	{ 0x00, 0x00, 0x00, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00 }, // 44 => 'L'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0xA8, 0xA8, 0xD8, 0x88, 0x00, 0x00 }, // 45 => 'M'
	{ 0x00, 0x00, 0x00, 0x88, 0x98, 0x98, 0xA8, 0xC8, 0xC8, 0x88, 0x00, 0x00 }, // 46 => 'N'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00 }, // 47 => 'O'
	{ 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xF0, 0x88, 0x88, 0xF0, 0x00, 0x00 }, // 48 => 'P'
	{ 0x00, 0x00, 0x18, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00 }, // 49 => 'Q'
	{ 0x00, 0x00, 0x00, 0x88, 0x90, 0xA0, 0xF0, 0x88, 0x88, 0xF0, 0x00, 0x00 }, // 50 => 'R'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x08, 0x70, 0x80, 0x88, 0x70, 0x00, 0x00 }, // 51 => 'S'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00 }, // 52 => 'T'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00 }, // 53 => 'U'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x50, 0x50, 0x88, 0x88, 0x88, 0x00, 0x00 }, // 54 => 'V'
	{ 0x00, 0x00, 0x00, 0x88, 0xD8, 0xA8, 0xA8, 0x88, 0x88, 0x88, 0x00, 0x00 }, // 55 => 'W'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x50, 0x20, 0x50, 0x88, 0x88, 0x00, 0x00 }, // 56 => 'X'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x50, 0x88, 0x88, 0x00, 0x00 }, // 57 => 'Y'
	{ 0x00, 0x00, 0x00, 0xF8, 0x80, 0x40, 0x20, 0x10, 0x08, 0xF8, 0x00, 0x00 }, // 58 => 'Z'
	{ 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x38 }, // 59 => '['
	{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80 }, // 60 => '\'
	{ 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70 }, // 61 => ']'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x50, 0x20, 0x00 }, // 62 => '^'
	{ 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 63 => '_'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00 }, // 64 => '`'
	{ 0x00, 0x00, 0x00, 0x68, 0x98, 0x88, 0x88, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 65 => 'a'
	{ 0x00, 0x00, 0x00, 0xF0, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x80, 0x00 }, // 66 => 'b'
	{ 0x00, 0x00, 0x00, 0x78, 0x80, 0x80, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 67 => 'c'
	{ 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0x88, 0x78, 0x08, 0x08, 0x08, 0x00 }, // 68 => 'd'
	{ 0x00, 0x00, 0x00, 0x70, 0x80, 0xF8, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00 }, // 69 => 'e'
	{ 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0xF0, 0x40, 0x40, 0x38, 0x00 }, // 70 => 'f'
	{ 0x70, 0x08, 0x08, 0x78, 0x88, 0x88, 0x88, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 71 => 'g'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x80, 0x00 }, // 72 => 'h'
	{ 0x00, 0x00, 0x00, 0x70, 0x20, 0x20, 0x20, 0x60, 0x00, 0x20, 0x20, 0x00 }, // 73 => 'i'
	{ 0x70, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x08, 0x08, 0x00 }, // 74 => 'j'
	{ 0x00, 0x00, 0x00, 0x48, 0x50, 0x60, 0x50, 0x48, 0x40, 0x40, 0x40, 0x00 }, // 75 => 'k'
	{ 0x00, 0x00, 0x00, 0x70, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x00 }, // 76 => 'l'
	{ 0x00, 0x00, 0x00, 0x88, 0xA8, 0xA8, 0xA8, 0xD0, 0x00, 0x00, 0x00, 0x00 }, // 77 => 'm'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0xC8, 0xB0, 0x00, 0x00, 0x00, 0x00 }, // 78 => 'n'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00 }, // 79 => 'o'
	{ 0x80, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0xF0, 0x00, 0x00, 0x00, 0x00 }, // 80 => 'p'
	{ 0x08, 0x08, 0x08, 0x78, 0x88, 0x88, 0x88, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 81 => 'q'
	{ 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x60, 0x58, 0x00, 0x00, 0x00, 0x00 }, // 82 => 'r'
	{ 0x00, 0x00, 0x00, 0xF0, 0x08, 0x70, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 83 => 's'
	{ 0x00, 0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x70, 0x20, 0x20, 0x20, 0x00 }, // 84 => 't'
	{ 0x00, 0x00, 0x00, 0x68, 0x98, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00 }, // 85 => 'u'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x50, 0x50, 0xD8, 0x00, 0x00, 0x00, 0x00 }, // 86 => 'v'
	{ 0x00, 0x00, 0x00, 0x50, 0xA8, 0xA8, 0xA8, 0x88, 0x00, 0x00, 0x00, 0x00 }, // 87 => 'w'
	{ 0x00, 0x00, 0x00, 0x88, 0x50, 0x20, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00 }, // 88 => 'x'
	{ 0x70, 0x08, 0x08, 0x78, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00 }, // 89 => 'y'
	{ 0x00, 0x00, 0x00, 0xF8, 0x40, 0x20, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00 }, // 90 => 'z'
	{ 0x00, 0x08, 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08 }, // 91 => '{'
	{ 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, // 92 => '|'
	{ 0x00, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x40 }, // 93 => '}'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xA8, 0x40, 0x00, 0x00, 0x00, 0x00 }  // 94 => '~'
};

void vDrawText(const std::string & sString)
{
	glRasterPos2f(0.0f, 12.0f);
	for(std::string::size_type stI = 0; stI < sString.length(); ++stI)
	{
		glBitmap(6, 12, 0, 0, 6, 0, g_ppuLetters[sString[stI] - CHARACTEROFFSET]);
	}
}

std::vector< std::string > SplitString(const std::string & String, char Delimiter)
{
	std::vector< std::string > Result;
	size_t Begin(0);
	size_t End(String.find(Delimiter, Begin));
	
	while(End != std::string::npos)
	{
		Result.push_back(String.substr(Begin, End - Begin));
		Begin = End + 1;
		End = String.find(Delimiter, Begin);
	}
	
	return Result;
}

void vCheckGLError(const char * pcFile, int iLine)
{
	GLenum Error = glGetError();
	
	if(Error != GL_NO_ERROR)
	{
		std::cerr << " *** OpenGL Error: " << Error << " in " << pcFile << ':' << iLine << std::endl;
	}
}

void vSetupViewport(void)
{
	glViewport(0, 0, static_cast< GLsizei >(g_Width), static_cast< GLsizei >(g_Height));
}

void vSetupProjection(bool bInitialize = true)
{
	glMatrixMode(GL_PROJECTION);
	if(bInitialize == true)
	{
		glLoadIdentity();
	}
	if(g_CurrentCamera != nullptr)
	{
		glMultMatrixf(g_CurrentCamera->GetProjectionMatrix().GetPointer());
	}
}

void vStartPicking(const Vector2f & MousePosition)
{
	glSelectBuffer(1024, g_puiSelectionBuffer);
	glRenderMode(GL_SELECT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef((g_Width - 2.0f * MousePosition[0]) / 10.0f, (g_Height - 2.0f * (g_Height - MousePosition[1])) / 10.0f, 0.0f);
	glScalef(g_Width / 10.0f, g_Height / 10.0f, 1.0);
	vSetupProjection(false);
	glMatrixMode(GL_MODELVIEW);
	glInitNames();
}

void vStopPicking(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	
	int NumberOfHits = glRenderMode(GL_RENDER);
	GLuint uiNearestDepth = 0xFFFFFFFF;
	GLuint uiNearestObject = 0;
	GLuint uiNearestObjectType = 0;
	GLuint * puiHitPointer = g_puiSelectionBuffer;
	bool bHover = false;
	
	g_HoveredPoint = nullptr;
	g_HoveredTriangle = nullptr;
	g_HoveredLight = nullptr;
	g_HoveredCamera = nullptr;
	g_HoveredMarker = nullptr;
	for(int HitIndex = 0; HitIndex < NumberOfHits; ++HitIndex)
	{
		// number of names on the name stack is always 2: type of the object, index in the global vector of the index
		assert(puiHitPointer[0] == 2);
		if((puiHitPointer[1] < uiNearestDepth) || ((uiNearestObjectType == LITTLEM_TRIANGLE) && ((puiHitPointer[3] == LITTLEM_CAMERA) || (puiHitPointer[3] == LITTLEM_POINT) || (puiHitPointer[3] == LITTLEM_LIGHT) || (puiHitPointer[3] == LITTLEM_MARKER))))
		{
			uiNearestDepth = puiHitPointer[1];
			uiNearestObjectType = puiHitPointer[3];
			uiNearestObject = puiHitPointer[4];
			bHover = true;
		}
		puiHitPointer += puiHitPointer[0] + 3;
	}
	if(bHover == true)
	{
		switch(uiNearestObjectType)
		{
		case LITTLEM_CAMERA:
			{
				g_HoveredCamera = g_Cameras[uiNearestObject];
				
				break;
			}
		case LITTLEM_LIGHT:
			{
				g_HoveredLight = Light::GetLights()[uiNearestObject];
				
				break;
			}
		case LITTLEM_POINT:
			{
				g_HoveredPoint = g_Points[uiNearestObject];
				
				break;
			}
		case LITTLEM_TRIANGLE:
			{
				g_HoveredTriangle = g_Triangles[uiNearestObject];
				
				break;
			}
		case LITTLEM_MARKER:
			{
				g_HoveredMarker = g_Markers[uiNearestObject];
				
				break;
			}
		default:
			{
				std::cout << "Unknown type in selection buffer." << std::endl;
				
				break;
			}
		}
	}
}

void vDrawTextAt(GLfloat fPositionX, GLfloat fPositionY, const std::string & sString)
{
	glPushMatrix();
	glTranslatef(fPositionX, fPositionY, 0.0f);
	vDrawText(sString);
	glPopMatrix();
}

void vDisplayTexts(void)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, g_Width, 0.0, g_Height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(0.0f, -g_Height, 0.0f);
	glColor3f(0.4f, 1.0f, 0.4f);
	vDrawTextAt(0.0f, 0.0f, "#Points: " + to_string_cast(g_Points.size()) + "   #Triangles: " + to_string_cast(g_Triangles.size()) + "   #Cameras: " + to_string_cast(g_Cameras.size()) + "   #Lights: " + to_string_cast(Light::GetLights().size()));
	vDrawTextAt(0.0f, 12.0f, "#Selected Points: " + to_string_cast(g_SelectedPoints.size()) + "   #Selected Triangles: " + to_string_cast(g_SelectedTriangles.size()));
	glColor3f(1.0f, 0.4f, 0.4f);
	if(g_SelectedPoints.size() == 1)
	{
		auto SelectedPoint{g_SelectedPoints.front()};
		
		std::stringstream ssPointInformationText;
		
		ssPointInformationText << "Point:  X: " << SelectedPoint->GetPosition()[0] << "   Y: " << SelectedPoint->GetPosition()[1] << "   Z: " << SelectedPoint->GetPosition()[2] << "   #TrianglePoints: " << SelectedPoint->GetTrianglePoints().size();
		vDrawTextAt(0.0f, 24.0f, ssPointInformationText.str());
	}
	if(g_SelectedCamera != nullptr)
	{
		std::stringstream ssCameraInformationText;
		
		ssCameraInformationText << "Camera:  X: " << g_SelectedCamera->GetPosition()[0] << "   Y: " << g_SelectedCamera->GetPosition()[1] << "   Z: " << g_SelectedCamera->GetPosition()[2];
		vDrawTextAt(0.0f, 36.0f, ssCameraInformationText.str());
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	vDrawTextAt(0.0f, 48.0f, "View: " + GetModelerViewString(g_ModelerView));
	if(g_CullPoints == true)
	{
		vDrawTextAt(0.0f, g_Height - 72.0f, "Culling points: on");
	}
	else
	{
		vDrawTextAt(0.0f, g_Height - 72.0f, "Culling points: off");
	}
	if(glIsEnabled(GL_CULL_FACE) == GL_TRUE)
	{
		vDrawTextAt(0.0f, g_Height - 60.0f, "Culling faces: on");
	}
	else
	{
		vDrawTextAt(0.0f, g_Height - 60.0f, "Culling faces: off");
	}
	if(glIsEnabled(GL_LIGHTING) == GL_TRUE)
	{
		vDrawTextAt(0.0f, g_Height - 48.0f, "Lighting: on");
	}
	else
	{
		vDrawTextAt(0.0f, g_Height - 48.0f, "Lighting: off");
	}
	
	GLint iFrontFace;
	
	glGetIntegerv(GL_FRONT_FACE, &iFrontFace);
	if(iFrontFace == GL_CCW)
	{
		vDrawTextAt(0.0f, g_Height - 36.0f, "Front Face: Counter Clock Wise");
	}
	else
	{
		vDrawTextAt(0.0f, g_Height - 36.0f, "Front Face: Clock Wise");
	}
	if(g_Snapping == true)
	{
		vDrawTextAt(0.0f, g_Height - 24.0f, "Snapping: on");
	}
	else
	{
		vDrawTextAt(0.0f, g_Height - 24.0f, "Snapping: off");
	}
	vDrawTextAt(0, g_Height - 12, "SnapFactor: " + to_string_cast(g_SnapFactor));
	glFlush();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

bool IsPointDrawable(Point * Point, Camera * Camera)
{
	auto Result{true};
	
	if(g_CullPoints == true)
	{
		auto Triangles{Point->GetTriangles()};
		
		if(Triangles.size() > 0)
		{
			auto CameraToPoint{Point->GetPosition() - Camera->GetPosition()};
			
			Result = false;
			for(auto Triangle : Point->GetTriangles())
			{
				if(Triangle->GetTriangleNormal().Dot(CameraToPoint) < 0.0f)
				{
					Result = true;
					
					break;
				}
			}
		}
	}
	
	return Result;
}

Point * CreatePoint(const Vector3f & Position)
{
	auto Result{new Point{}};
	
	Result->SetPosition(Position);
	g_Points.push_back(Result);
	
	return Result;
}

TrianglePoint * CreateTrianglePoint(Point * Point)
{
	auto Result{new TrianglePoint{}};
	
	Result->SetPoint(Point);
	
	return Result;
}

Triangle * CreateTriangle(Point * Point1, Point * Point2, Point * Point3)
{
	auto TrianglePoint1{CreateTrianglePoint(Point1)};
	auto TrianglePoint2{CreateTrianglePoint(Point2)};
	auto TrianglePoint3{CreateTrianglePoint(Point3)};
	auto Result{new Triangle{}};
	
	Result->SetTrianglePoint<0>(TrianglePoint1);
	Result->SetTrianglePoint<1>(TrianglePoint2);
	Result->SetTrianglePoint<2>(TrianglePoint3);
	Result->RealignNormal();
	g_Triangles.push_back(Result);
	
	return Result;
}

void CreateRegularIcosahedron(void)
{
	auto Phi{(1.0f + sqrt(5.0f)) / 2.0f};
	auto Point1{CreatePoint(Vector3f::CreateFromComponents(0.0f, 1.0f, Phi).Normalize())};
	auto Point2{CreatePoint(Vector3f::CreateFromComponents(0.0f, -1.0f, Phi).Normalize())};
	auto Point3{CreatePoint(Vector3f::CreateFromComponents(0.0f, -1.0f, -Phi).Normalize())};
	auto Point4{CreatePoint(Vector3f::CreateFromComponents(0.0f, 1.0f, -Phi).Normalize())};
	auto Point5{CreatePoint(Vector3f::CreateFromComponents(Phi, 0.0f, 1.0f).Normalize())};
	auto Point6{CreatePoint(Vector3f::CreateFromComponents(Phi, 0.0f, -1.0f).Normalize())};
	auto Point7{CreatePoint(Vector3f::CreateFromComponents(-Phi, 0.0f, -1.0f).Normalize())};
	auto Point8{CreatePoint(Vector3f::CreateFromComponents(-Phi, 0.0f, 1.0f).Normalize())};
	auto Point9{CreatePoint(Vector3f::CreateFromComponents(1.0f, Phi, 0.0f).Normalize())};
	auto Point10{CreatePoint(Vector3f::CreateFromComponents(-1.0f, Phi, 0.0f).Normalize())};
	auto Point11{CreatePoint(Vector3f::CreateFromComponents(-1.0f, -Phi, 0.0f).Normalize())};
	auto Point12{CreatePoint(Vector3f::CreateFromComponents(1.0f, -Phi, 0.0f).Normalize())};
	
	CreateTriangle(Point1, Point2, Point5);
	CreateTriangle(Point1, Point8, Point2);
	CreateTriangle(Point1, Point10, Point8);
	CreateTriangle(Point1, Point9, Point10);
	CreateTriangle(Point1, Point5, Point9);
	CreateTriangle(Point2, Point12, Point5);
	CreateTriangle(Point2, Point11, Point12);
	CreateTriangle(Point2, Point8, Point11);
	CreateTriangle(Point3, Point4, Point6);
	CreateTriangle(Point3, Point6, Point12);
	CreateTriangle(Point3, Point12, Point11);
	CreateTriangle(Point3, Point11, Point7);
	CreateTriangle(Point3, Point7, Point4);
	CreateTriangle(Point4, Point9, Point6);
	CreateTriangle(Point4, Point10, Point9);
	CreateTriangle(Point4, Point7, Point10);
	CreateTriangle(Point5, Point12, Point6);
	CreateTriangle(Point5, Point6, Point9);
	CreateTriangle(Point8, Point7, Point11);
	CreateTriangle(Point8, Point10, Point7);
}

/**
 * @brief Performs the picking of model elements.
 * 
 *A precondition for this function is that a current camera is selected in g_CurrentCamera.
 **/
void vPerformPicking(void)
{
	assert(g_CurrentCamera != 0);
	
	vStartPicking(g_MousePosition);
	glPushName(LITTLEM_TRIANGLE);
	// dummy entry so that we can call glLoadName in the loop below: it will be replace immediately
	glPushName(0xFFFFFFFF);
	for(std::vector< Triangle * >::size_type stI = 0; stI < g_Triangles.size(); ++stI)
	{
		glLoadName(stI);
		glBegin(GL_TRIANGLES);
		glVertex3fv(g_Triangles[stI]->m_ppTrianglePoints[0]->GetPoint()->GetPosition().GetPointer());
		glVertex3fv(g_Triangles[stI]->m_ppTrianglePoints[1]->GetPoint()->GetPosition().GetPointer());
		glVertex3fv(g_Triangles[stI]->m_ppTrianglePoints[2]->GetPoint()->GetPosition().GetPointer());
		glEnd();
	}
	glPopName();
	glLoadName(LITTLEM_POINT);
	// dummy entry so that we can call glLoadName in the loop below: it will be replace immediately
	glPushName(0xFFFFFFFF);
	for(std::vector< Point * >::size_type stI = 0; stI < g_Points.size(); ++stI)
	{
		if(IsPointDrawable(g_Points[stI], g_CurrentCamera) == true)
		{
			glLoadName(stI);
			glBegin(GL_POINTS);
			glVertex3fv(g_Points[stI]->GetPosition().GetPointer());
			glEnd();
		}
	}
	glPopName();
	glLoadName(LITTLEM_LIGHT);
	// dummy entry so that we can call glLoadName in the loop below: it will be replace immediately
	glPushName(0xFFFFFFFF);
	for(std::vector< Light * >::size_type stLight = 0; stLight < Light::GetLights().size(); ++stLight)
	{
		glLoadName(stLight);
		glBegin(GL_POINTS);
		glVertex3fv(Light::GetLights()[stLight]->GetPosition().GetPointer());
		glEnd();
	}
	glPopName();
	glLoadName(LITTLEM_CAMERA);
	// dummy entry so that we can call glLoadName in the loop below: it will be replace immediately
	glPushName(0xFFFFFFFF);
	for(std::vector< Camera * >::size_type stCamera = 0; stCamera < g_Cameras.size(); ++stCamera)
	{
		glLoadName(stCamera);
		glBegin(GL_POINTS);
		glVertex3fv(g_Cameras[stCamera]->GetPosition().GetPointer());
		glEnd();
	}
	glPopName();
	glLoadName(LITTLEM_MARKER);
	// dummy entry so that we can call glLoadName in the loop below: it will be replace immediately
	glPushName(0xFFFFFFFF);
	for(auto MarkerIndex = 0ul; MarkerIndex < g_Markers.size(); ++MarkerIndex)
	{
		glLoadName(MarkerIndex);
		glBegin(GL_POINTS);
		glVertex3fv(g_Markers[MarkerIndex]->GetPosition().GetPointer());
		glEnd();
	}
	glPopName();
	glPopName();
	vStopPicking();
}

/**
 * @brief Displays the model.
 * 
 * A precondition for this function is that a current camera is selected in g_CurrentCamera.
 **/
void vDisplayModel(void)
{
	assert(g_CurrentCamera != nullptr);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(g_CurrentCamera->GetSpacialMatrix().Inverted().GetPointer());
	for(auto EnabledLight : Light::GetEnabledLights())
	{
		EnabledLight->DrawLight();
	}
	glBegin(GL_TRIANGLES);
	glColor3f(0.3f, 0.3f, 0.5f);
	for(auto Triangle : g_Triangles)
	{
		glNormal3fv(Triangle->m_ppTrianglePoints[0]->_Normal.GetPointer());
		glVertex3fv(Triangle->m_ppTrianglePoints[0]->GetPoint()->GetPosition().GetPointer());
		glNormal3fv(Triangle->m_ppTrianglePoints[1]->_Normal.GetPointer());
		glVertex3fv(Triangle->m_ppTrianglePoints[1]->GetPoint()->GetPosition().GetPointer());
		glNormal3fv(Triangle->m_ppTrianglePoints[2]->_Normal.GetPointer());
		glVertex3fv(Triangle->m_ppTrianglePoints[2]->GetPoint()->GetPosition().GetPointer());
	}
	glEnd();
	
	GLboolean bLighting = glIsEnabled(GL_LIGHTING);
	
	if(bLighting == GL_TRUE)
	{
		glDisable(GL_LIGHTING);
	}
	// draw all points with deactivated lighting
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	for(auto Point : g_Points)
	{
		if(IsPointDrawable(Point, g_CurrentCamera) == true)
		{
			glVertex3fv(Point->GetPosition().GetPointer());
		}
	}
	glEnd();
	// draw all lights with deactivated lighting
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(auto Light : Light::GetLights())
	{
		Light->DrawPoint();
	}
	glEnd();
	// draw all markers with deactivated lighting
	glColor3f(0.6f, 0.6f, 0.6f);
	glBegin(GL_POINTS);
	for(auto Marker : g_Markers)
	{
		glVertex3fv(Marker->GetPosition().GetPointer());
	}
	glEnd();
	// draw all cameras with deactivated lighting
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(auto Camera : g_Cameras)
	{
		glVertex3fv(Camera->GetPosition().GetPointer());
	}
	glEnd();
	// now draw selected stuff
	if(g_SelectedTriangles.empty() == false)
	{
		for(auto SelectedTriangle : g_SelectedTriangles)
		{
			glColor3f(0.5f, 0.8f, 0.2f);
			glBegin(GL_TRIANGLES);
			glNormal3fv(SelectedTriangle->m_ppTrianglePoints[0]->_Normal.GetPointer());
			glVertex3fv(SelectedTriangle->m_ppTrianglePoints[0]->GetPoint()->GetPosition().GetPointer());
			glNormal3fv(SelectedTriangle->m_ppTrianglePoints[1]->_Normal.GetPointer());
			glVertex3fv(SelectedTriangle->m_ppTrianglePoints[1]->GetPoint()->GetPosition().GetPointer());
			glNormal3fv(SelectedTriangle->m_ppTrianglePoints[2]->_Normal.GetPointer());
			glVertex3fv(SelectedTriangle->m_ppTrianglePoints[2]->GetPoint()->GetPosition().GetPointer());
			glEnd();
			glColor3f(0.6f, 0.6f, 0.0f);
			glPushMatrix();
			
			Vector3f TriangleCenter(SelectedTriangle->GetTriangleCenter());
			Vector3f TriangleNormal(SelectedTriangle->GetTriangleNormal());
			float Scale(0.0f);
			
			glTranslatef(TriangleCenter[0], TriangleCenter[1], TriangleCenter[2]);
			Scale = 2.0f * (TriangleCenter - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f);
			glScalef(Scale, Scale, Scale);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(TriangleNormal.Scale(0.05f).GetPointer());
			glEnd();
			glPopMatrix();
			glColor3f(0.4f, 0.1f, 0.0f);
			glPushMatrix();
			glTranslatef(SelectedTriangle->GetPoint(1)->GetPosition()[0], SelectedTriangle->GetPoint(1)->GetPosition()[1], SelectedTriangle->GetPoint(1)->GetPosition()[2]);
			Scale = 2.0f * (SelectedTriangle->GetPoint(1)->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f);
			glScalef(Scale, Scale, Scale);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(SelectedTriangle->GetTrianglePoint(1)->_Normal.Scaled(0.05f).GetPointer());
			glEnd();
			glPopMatrix();
			glPushMatrix();
			glTranslatef(SelectedTriangle->GetPoint(2)->GetPosition()[0], SelectedTriangle->GetPoint(2)->GetPosition()[1], SelectedTriangle->GetPoint(2)->GetPosition()[2]);
			Scale = 2.0f * (SelectedTriangle->GetPoint(2)->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f);
			glScalef(Scale, Scale, Scale);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(SelectedTriangle->GetTrianglePoint(2)->_Normal.Scaled(0.05f).GetPointer());
			glEnd();
			glPopMatrix();
			glPushMatrix();
			glTranslatef(SelectedTriangle->GetPoint(3)->GetPosition()[0], SelectedTriangle->GetPoint(3)->GetPosition()[1], SelectedTriangle->GetPoint(3)->GetPosition()[2]);
			Scale = 2.0f * (SelectedTriangle->GetPoint(3)->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f);
			glScalef(Scale, Scale, Scale);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(SelectedTriangle->GetTrianglePoint(3)->_Normal.Scaled(0.05f).GetPointer());
			glEnd();
			glPopMatrix();
		}
	}
	if(g_SelectedPoints.empty() == false)
	{
		glDisable(GL_DEPTH_TEST);
		
		auto Matrix(Matrix4f::CreateRotation(g_CurrentCamera->GetOrientation()));
		
		for(auto Point : g_SelectedPoints)
		{
			glPushMatrix();
			glTranslatef(Point->GetPosition()[0], Point->GetPosition()[1], Point->GetPosition()[2]);
			glMultMatrixf(Matrix.GetPointer());
			
			auto Scale(2.0f * (Point->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f));
			
			glScalef(Scale, Scale, Scale);
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.01f, 0.005f);
			glVertex2f(-0.01f, 0.01f);
			glVertex2f(-0.005f, 0.01f);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(0.005f, 0.01f);
			glVertex2f(0.01f, 0.01f);
			glVertex2f(0.01f, 0.005f);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(0.01f, -0.005f);
			glVertex2f(0.01f, -0.01f);
			glVertex2f(0.005f, -0.01f);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.005f, -0.01f);
			glVertex2f(-0.01f, -0.01f);
			glVertex2f(-0.01f, -0.005f);
			glEnd();
			glPopMatrix();
		}
		glEnable(GL_DEPTH_TEST);
	}
	if(g_SelectedLight != nullptr)
	{
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		glTranslatef(g_SelectedLight->GetPosition()[0], g_SelectedLight->GetPosition()[1], g_SelectedLight->GetPosition()[2]);
		glMultMatrixf(Matrix4f::CreateRotation(g_CurrentCamera->GetOrientation()).GetPointer());
		
		auto Scale(2.0f * (g_SelectedLight->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f));
		
		glScalef(Scale, Scale, Scale);
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-0.01f, 0.005f);
		glVertex2f(-0.01f, 0.01f);
		glVertex2f(-0.005f, 0.01f);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.005f, 0.01f);
		glVertex2f(0.01f, 0.01f);
		glVertex2f(0.01f, 0.005f);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.01f, -0.005f);
		glVertex2f(0.01f, -0.01f);
		glVertex2f(0.005f, -0.01f);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(-0.005f, -0.01f);
		glVertex2f(-0.01f, -0.01f);
		glVertex2f(-0.01f, -0.005f);
		glEnd();
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
	if(g_SelectedCamera != nullptr)
	{
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		glTranslatef(g_SelectedCamera->GetPosition()[0], g_SelectedCamera->GetPosition()[1], g_SelectedCamera->GetPosition()[2]);
		glPushMatrix();
		glMultMatrixf(Matrix4f::CreateRotation(g_CurrentCamera->GetOrientation()).GetPointer());
		
		auto Scale(2.0f * (g_SelectedCamera->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f));
		
		glScalef(Scale, Scale, Scale);
		glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-0.01f, 0.005f);
		glVertex2f(-0.01f, 0.01f);
		glVertex2f(-0.005f, 0.01f);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.005f, 0.01f);
		glVertex2f(0.01f, 0.01f);
		glVertex2f(0.01f, 0.005f);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.01f, -0.005f);
		glVertex2f(0.01f, -0.01f);
		glVertex2f(0.005f, -0.01f);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(-0.005f, -0.01f);
		glVertex2f(-0.01f, -0.01f);
		glVertex2f(-0.01f, -0.005f);
		glEnd();
		glPopMatrix();
		glMultMatrixf(Matrix4f::CreateRotation(g_SelectedCamera->GetOrientation()).GetPointer());
		glScalef(Scale, Scale, Scale);
		glColor3f(0.2f, 0.8f, 0.5f);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -0.05f); // the default camera is looking along the negative z axis
		glEnd();
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
	if(g_SelectedMarker != nullptr)
	{
		assert(g_SelectedMarker->HasPosition() == true);
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		glTranslatef(g_SelectedMarker->GetPosition()[0], g_SelectedMarker->GetPosition()[1], g_SelectedMarker->GetPosition()[2]);
		glPushMatrix();
		glMultMatrixf(Matrix4f::CreateRotation(g_CurrentCamera->GetOrientation()).GetPointer());
		
		auto Scale(2.0f * (g_SelectedMarker->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f));
		
		glScalef(Scale, Scale, Scale);
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-0.01f, 0.005f);
		glVertex2f(-0.01f, 0.01f);
		glVertex2f(-0.005f, 0.01f);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.005f, 0.01f);
		glVertex2f(0.01f, 0.01f);
		glVertex2f(0.01f, 0.005f);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.01f, -0.005f);
		glVertex2f(0.01f, -0.01f);
		glVertex2f(0.005f, -0.01f);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(-0.005f, -0.01f);
		glVertex2f(-0.01f, -0.01f);
		glVertex2f(-0.01f, -0.005f);
		glEnd();
		glPopMatrix();
		if(g_SelectedMarker->HasOrientation() == true)
		{
			glPushMatrix();
			glMultMatrixf(Matrix4f::CreateRotation(g_SelectedMarker->GetOrientation()).GetPointer());
			glColor3f(1.0f, 0.0f, 1.0f);
			glBegin(GL_LINE_STRIP);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
			glEnd();
			glPopMatrix();
		}
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
	if(g_HoveredPoint != nullptr)
	{
		glColor3f(0.5f, 0.5f, 1.0f);
		glBegin(GL_POINTS);
		glVertex3fv(g_HoveredPoint->GetPosition().GetPointer());
		glEnd();
	}
	else if(g_HoveredTriangle != nullptr)
	{
		glColor3f(1.0f, 0.7f, 0.3f);
		glBegin(GL_TRIANGLES);
		glNormal3fv(g_HoveredTriangle->m_ppTrianglePoints[0]->_Normal.GetPointer());
		glVertex3fv(g_HoveredTriangle->m_ppTrianglePoints[0]->GetPoint()->GetPosition().GetPointer());
		glNormal3fv(g_HoveredTriangle->m_ppTrianglePoints[1]->_Normal.GetPointer());
		glVertex3fv(g_HoveredTriangle->m_ppTrianglePoints[1]->GetPoint()->GetPosition().GetPointer());
		glNormal3fv(g_HoveredTriangle->m_ppTrianglePoints[2]->_Normal.GetPointer());
		glVertex3fv(g_HoveredTriangle->m_ppTrianglePoints[2]->GetPoint()->GetPosition().GetPointer());
		glEnd();
		glColor3f(0.3f, 0.1f, 0.3f);
		glPushMatrix();
		
		Vector3f TriangleCenter(g_HoveredTriangle->GetTriangleCenter());
		Vector3f TriangleNormal(g_HoveredTriangle->GetTriangleNormal());
		float Scale(0.0f);
		
		glTranslatef(TriangleCenter[0], TriangleCenter[1], TriangleCenter[2]);
		Scale = 2.0f * (TriangleCenter - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f);
		glScalef(Scale, Scale, Scale);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(TriangleNormal.Scale(0.05).GetPointer());
		glEnd();
		glPopMatrix();
		glColor3f(0.0f, 0.1f, 0.4f);
		glPushMatrix();
		glTranslatef(g_HoveredTriangle->GetPoint(1)->GetPosition()[0], g_HoveredTriangle->GetPoint(1)->GetPosition()[1], g_HoveredTriangle->GetPoint(1)->GetPosition()[2]);
		Scale = 2.0f * (g_HoveredTriangle->GetPoint(1)->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f);
		glScalef(Scale, Scale, Scale);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(g_HoveredTriangle->GetTrianglePoint(1)->_Normal.Scaled(0.05f).GetPointer());
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(g_HoveredTriangle->GetPoint(2)->GetPosition()[0], g_HoveredTriangle->GetPoint(2)->GetPosition()[1], g_HoveredTriangle->GetPoint(2)->GetPosition()[2]);
		Scale = 2.0f * (g_HoveredTriangle->GetPoint(2)->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f);
		glScalef(Scale, Scale, Scale);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(g_HoveredTriangle->GetTrianglePoint(2)->_Normal.Scaled(0.05f).GetPointer());
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(g_HoveredTriangle->GetPoint(3)->GetPosition()[0], g_HoveredTriangle->GetPoint(3)->GetPosition()[1], g_HoveredTriangle->GetPoint(3)->GetPosition()[2]);
		Scale = 2.0f * (g_HoveredTriangle->GetPoint(3)->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f);
		glScalef(Scale, Scale, Scale);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(g_HoveredTriangle->GetTrianglePoint(3)->_Normal.Scaled(0.05f).GetPointer());
		glEnd();
		glPopMatrix();
	}
	else if(g_HoveredLight != nullptr)
	{
		glColor3f(1.0f, 0.5f, 0.5f);
		glBegin(GL_POINTS);
		glVertex3fv(g_HoveredLight->GetPosition().GetPointer());
		glEnd();
	}
	else if(g_HoveredCamera != nullptr)
	{
		glColor3f(0.0f, 1.0f, 0.5f);
		glBegin(GL_POINTS);
		glVertex3fv(g_HoveredCamera->GetPosition().GetPointer());
		glEnd();
		glPushMatrix();
		glTranslatef(g_HoveredCamera->GetPosition()[0], g_HoveredCamera->GetPosition()[1], g_HoveredCamera->GetPosition()[2]);
		glMultMatrixf(Matrix4f::CreateRotation(g_HoveredCamera->GetOrientation()).GetPointer());
		
		auto Scale(2.0f * (g_HoveredCamera->GetPosition() - g_CurrentCamera->GetPosition()).Length() * tan(g_CurrentCamera->GetFieldOfViewY() / 2.0f));
		
		glScalef(Scale, Scale, Scale);
		glColor3f(0.0f, 0.5f, 0.5f);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -0.05f); // the default camera is looking along the negative z axis
		glEnd();
		glPopMatrix();
	}
	else if(g_HoveredMarker != nullptr)
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_POINTS);
		glVertex3fv(g_HoveredMarker->GetPosition().GetPointer());
		glEnd();
	}
	if(bLighting == GL_TRUE)
	{
		glEnable(GL_LIGHTING);
	}
}

void vDisplayModelView(void)
{
	if(g_CurrentCamera != nullptr)
	{
		vPerformPicking();
		vDisplayModel();
	}
}

void vDisplayUserInterface(void)
{
	auto Lighting{glIsEnabled(GL_LIGHTING)};
	
	if(Lighting == GL_TRUE)
	{
		glDisable(GL_LIGHTING);
	}
	glDisable(GL_DEPTH_TEST);
	vDisplayTexts();
	glEnable(GL_DEPTH_TEST);
	if(Lighting == GL_TRUE)
	{
		glEnable(GL_LIGHTING);
	}
}

void vDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vDisplayModelView();
	vDisplayUserInterface();
}

void DeleteTriangle(std::vector< Triangle * >::iterator iTriangle)
{
	assert(iTriangle != g_Triangles.end());
	
	Triangle * pTriangle(*iTriangle);
	
	if(pTriangle == g_HoveredTriangle)
	{
		g_HoveredTriangle = 0;
	}
	
	std::vector< Triangle * >::iterator iSelectedTriangle(std::find(g_SelectedTriangles.begin(), g_SelectedTriangles.end(), pTriangle));
	
	if(iSelectedTriangle != g_SelectedTriangles.end())
	{
		g_SelectedTriangles.erase(iSelectedTriangle);
	}
	g_Triangles.erase(iTriangle);
	pTriangle->SetTrianglePoint<0>(nullptr);
	pTriangle->SetTrianglePoint<1>(nullptr);
	pTriangle->SetTrianglePoint<2>(nullptr);
	delete pTriangle;
}

void DeleteTriangle(std::vector< Triangle * >::size_type stTriangle)
{
	assert(stTriangle < g_Triangles.size());
	DeleteTriangle(g_Triangles.begin() + stTriangle);
}

void DeleteTriangle(Triangle * pTriangle)
{
	assert(pTriangle != 0);
	DeleteTriangle(std::find(g_Triangles.begin(), g_Triangles.end(), pTriangle));
}

void DeleteCamera(std::vector< Camera * >::iterator iCamera)
{
	assert(iCamera != g_Cameras.end());
	
	auto pCamera(*iCamera);
	
	if(pCamera == g_HoveredCamera)
	{
		g_HoveredCamera = nullptr;
	}
	if(pCamera == g_CurrentCamera)
	{
		g_CurrentCamera = nullptr;
	}
	if(pCamera == g_SelectedCamera)
	{
		g_SelectedCamera = nullptr;
	}
	g_Cameras.erase(iCamera);
}

void DeleteCamera(Camera * Camera)
{
	assert(Camera != nullptr);
	DeleteCamera(std::find(g_Cameras.begin(), g_Cameras.end(), Camera));
}

void DeletePoint(std::vector< Point * >::iterator PointIterator)
{
	assert(PointIterator != g_Points.end());
	
	auto Point(*PointIterator);
	
	while(Point->GetTrianglePoints().size() > 0)
	{
		assert(Point->GetTrianglePoints().front()->GetPoint() == Point);
		if(Point->GetTrianglePoints().front()->_Triangles.size() == 0)
		{
			std::cout << "encountered a triangle point without a triangle!" << std::endl;
			delete Point->GetTrianglePoints().front();
		}
		else
		{
			while(Point->GetTrianglePoints().front()->_Triangles.size() > 0)
			{
				DeleteTriangle(Point->GetTrianglePoints().front()->_Triangles.front());
			}
		}
	}
	if(g_HoveredPoint == Point)
	{
		g_HoveredPoint = nullptr;
	}
	
	auto SelectedPointIterator(std::find(g_SelectedPoints.begin(), g_SelectedPoints.end(), Point));
	
	if(SelectedPointIterator != g_SelectedPoints.end())
	{
		g_SelectedPoints.erase(SelectedPointIterator);
	}
	g_Points.erase(PointIterator);
	delete Point;
}

void vDeletePoint(Point * pPoint)
{
	assert(pPoint != 0);
	DeletePoint(std::find(g_Points.begin(), g_Points.end(), pPoint));
}

void DeleteMarker(std::vector< Marker * >::iterator MarkerIterator)
{
	assert(MarkerIterator != g_Markers.end());
	
	auto Marker(*MarkerIterator);
	
	if(Marker == g_HoveredMarker)
	{
		g_HoveredMarker = nullptr;
	}
	if(Marker == g_SelectedMarker)
	{
		g_SelectedMarker = nullptr;
	}
	g_Markers.erase(MarkerIterator);
}

void DeleteMarker(Marker * Marker)
{
	assert(Marker != nullptr);
	DeleteMarker(std::find(g_Markers.begin(), g_Markers.end(), Marker));
}

void vClearScene(void)
{
	Light::DeleteLights();
	while(g_Triangles.size() > 0)
	{
		DeleteTriangle(g_Triangles.begin());
	}
	while(g_Points.size() > 0)
	{
		DeletePoint(g_Points.begin());
	}
	while(g_Cameras.size() > 0)
	{
		DeleteCamera(g_Cameras.begin());
	}
	while(g_Markers.size() > 0)
	{
		DeleteMarker(g_Markers.begin());
	}
}

/**
 * @brief Toggle the OpenGL option GL_POINT_SMOOTH.
 * 
 * The OpenGL option GL_POINT_SMOOTH specifies whether points should be round or rectangled.
 **/
void vTogglePointSmooth(void)
{
	if(glIsEnabled(GL_POINT_SMOOTH) == GL_TRUE)
	{
		glDisable(GL_POINT_SMOOTH);
	}
	else
	{
		glEnable(GL_POINT_SMOOTH);
	}
}

/**
 * @brief Toggle the OpenGL option GL_CULL_FACE.
 * 
 * GL_CULL_FACE is the OpenGL option that specifies whether polygon back sides are omitted during drawing.
 **/
void vToggleCullFace(void)
{
	if(glIsEnabled(GL_CULL_FACE) == GL_TRUE)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}
}

/**
 * @brief Toggle the OpenGL option GL_DEPTH_TEST.
 * 
 * GL_DEPTH_TEST is the OpenGL option that specifies whether a depth test is performed during drawing. If the depth test is enabled objects in the front will cover objects in the back. If it is disabled the ordering of drawing decides which objects are visible.
 **/
void vToggleDepthTest(void)
{
	if(glIsEnabled(GL_DEPTH_TEST) == GL_TRUE)
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}
}

/**
 * @brief Toggle the OpenGL option GL_FRONT_FACE between GL_CCW and GL_CW.
 * 
 * The OpenGL option GL_FRONT_FACE specifies whether front faces are defined by clockwise drawing (GL_CW) or counter-clockwise drawing (GL_CCW). This global option changes the front face side for all faces.
 **/
void vToggleFrontFace(void)
{
	GLint iFrontFace;
	
	glGetIntegerv(GL_FRONT_FACE, &iFrontFace);
	if(iFrontFace == GL_CCW)
	{
		glFrontFace(GL_CW);
	}
	else
	{
		glFrontFace(GL_CCW);
	}
}

/**
 * @brief Toggle the OpenGL option GL_LIGHTING.
 * 
 * The OpenGL option GL_LIGHTING specifies whether the light sources in the scene should be used for lighting effects.
 **/
void vToggleLighting(void)
{
	if(glIsEnabled(GL_LIGHTING) == GL_TRUE)
	{
		glDisable(GL_LIGHTING);
	}
	else
	{
		glEnable(GL_LIGHTING);
	}
}

/**
 * @brief Toggle the snapping option.
 * 
 * The snapping option defines whether movement operations on points will snap their positions to a grid.
 **/
void vToggleSnapping(void)
{
	g_Snapping = !g_Snapping;
}

float GetSnapped(float Value)
{
	return roundf(Value / g_SnapFactor) * g_SnapFactor;
}

Vector3f GetSnapped(const Vector3f & Position)
{
	return Vector3f::CreateFromComponents(GetSnapped(Position[0]), GetSnapped(Position[1]), GetSnapped(Position[2]));
}

Vector3f GetSnappedStepInView(const Vector3f & Position, FixedDirection Direction)
{
	auto Delta(0.01f);
	
	if(g_Snapping == true)
	{
		Delta = g_SnapFactor;
	}
	assert(g_CurrentCamera != nullptr);
	
	auto Result(Position + GetDirectionVector(Direction).Scaled(Delta).Rotate(g_CurrentCamera->GetOrientation()));
	
	if(g_Snapping == true)
	{
		if(Result[0] != Position[0])
		{
			Result[0] = GetSnapped(Result[0]);
		}
		if(Result[1] != Position[1])
		{
			Result[1] = GetSnapped(Result[1]);
		}
		if(Result[2] != Position[2])
		{
			Result[2] = GetSnapped(Result[2]);
		}
	}
	
	return Result;
}

bool AcceptKeyInPointView(int KeyCode, bool IsDown)
{
	bool bKeyAccepted(false);
	
	switch(KeyCode)
	{
	case 65: // SPACE
		{
			bKeyAccepted = true;
			if(IsDown == true)
			{
				// create triangle from three selected points
				if(g_SelectedPoints.size() == 3)
				{
					auto NewTriangle{CreateTriangle(g_SelectedPoints[0], g_SelectedPoints[1], g_SelectedPoints[2])};
					
					g_SelectedTriangles.push_back(NewTriangle);
					g_SelectedPoints.clear();
				}
			}
			
			break;
		}
	case 23: // TABULATOR
		{
			if(IsDown == true)
			{
				// rotate point selection
				if(g_SelectedPoints.size() > 0)
				{
					bKeyAccepted = true;
				}
				
				std::vector< Point * > NewSelectedPoints;
				
				for(std::vector< Point * >::size_type stSelectedPoint = 0; stSelectedPoint < g_SelectedPoints.size(); ++stSelectedPoint)
				{
					std::vector< Point * >::iterator iPoint(std::find(g_Points.begin(), g_Points.end(), g_SelectedPoints[stSelectedPoint]));
					
					++iPoint;
					if(iPoint == g_Points.end())
					{
						iPoint = g_Points.begin();
					}
					g_SelectedPoints[stSelectedPoint] = *iPoint;
				}
			}
			
			break;
		}
	case 38: // A
		{
			if((IsDown == true) && (g_Keyboard.IsAnyControlActive() == true))
			{
				g_SelectedPoints.clear();
				std::copy(g_Points.begin(), g_Points.end(), std::back_inserter(g_SelectedPoints));
				bKeyAccepted = true;
				
				break;
			}
		}
	case 40: // D
		{
			if(IsDown == true)
			{
				// duplicate selected points
				if(g_SelectedPoints.size() > 0)
				{
					bKeyAccepted = true;
					std::vector< Point * > NewSelectedPoints;
					
					for(auto SelectedPoint : g_SelectedPoints)
					{
						auto NewPoint{new Point{}};
						
						NewPoint->SetPosition(SelectedPoint->GetPosition());
						g_Points.push_back(NewPoint);
						NewSelectedPoints.push_back(NewPoint);
					}
					g_SelectedPoints = NewSelectedPoints;
				}
			}
			
			break;
		}
	case 57: // N
		{
			if(IsDown == true)
			{
				auto NewPoint(new Point());
				
				if(g_Keyboard.IsAltActive() == true)
				{
					// assign random coordinates within (-1.0 .. 1.0, -1.0 .. 1.0, -1.0 .. 1.0)
					NewPoint->SetPosition(Vector3f::CreateFromComponents(-1.0f + 2 * (static_cast< double> (random()) / RAND_MAX), -1.0f + 2 * (static_cast< double> (random()) / RAND_MAX), -1.0f + 2 * (static_cast< double> (random()) / RAND_MAX)));
				}
				g_Points.push_back(NewPoint);
				bKeyAccepted = true;
			}
			
			break;
		}
	case 39: // S
		{
			if((IsDown == true) && (g_Keyboard.IsAnyControlActive() == true) && (g_SelectedPoints.empty() == false))
			{
				for(auto Point : g_SelectedPoints)
				{
					Point->SetPosition(GetSnapped(Point->GetPosition()));
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 28: // T
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true))
			{
				// select all triangles which contain the selected points
				if(g_SelectedPoints.size() > 0)
				{
					g_SelectedTriangles.clear();
					
					std::vector< Triangle * > TrianglesToSelect;
					
					for(auto Point : g_SelectedPoints)
					{
						for(auto TrianglePoint : Point->GetTrianglePoints())
						{
							std::copy(TrianglePoint->GetTriangles().begin(), TrianglePoint->GetTriangles().end(), back_inserter(TrianglesToSelect));
						}
					}
					std::sort(TrianglesToSelect.begin(), TrianglesToSelect.end());
					std::unique_copy(TrianglesToSelect.begin(), TrianglesToSelect.end(), back_inserter(g_SelectedTriangles));
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	case 119: // DELETE
		{
			bKeyAccepted = true;
			if(IsDown == true)
			{
				// delete selected points
				while(g_SelectedPoints.size() > 0)
				{
					vDeletePoint(g_SelectedPoints.front());
				}
			}
			
			break;
		}
	case 114: // RIGHT
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedPoints.empty() == false))
			{
				for(auto Point : g_SelectedPoints)
				{
					Point->SetPosition(GetSnappedStepInView(Point->GetPosition(), FixedDirection::Right));
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 113: // LEFT
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedPoints.empty() == false))
			{
				for(auto Point : g_SelectedPoints)
				{
					Point->SetPosition(GetSnappedStepInView(Point->GetPosition(), FixedDirection::Left));
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 111: // UP
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedPoints.empty() == false))
			{
				for(auto Point : g_SelectedPoints)
				{
					Point->SetPosition(GetSnappedStepInView(Point->GetPosition(), FixedDirection::Up));
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 116: // DOWN
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedPoints.empty() == false))
			{
				for(auto Point : g_SelectedPoints)
				{
					Point->SetPosition(GetSnappedStepInView(Point->GetPosition(), FixedDirection::Down));
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 117: // PAGE DOWN
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedPoints.empty() == false))
			{
				for(auto Point : g_SelectedPoints)
				{
					Point->SetPosition(GetSnappedStepInView(Point->GetPosition(), FixedDirection::Backward));
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 112: // PAGE UP
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedPoints.empty() == false))
			{
				for(auto Point : g_SelectedPoints)
				{
					Point->SetPosition(GetSnappedStepInView(Point->GetPosition(), FixedDirection::Forward));
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	}
	
	return bKeyAccepted;
}

bool AcceptKeyInTriangleView(int KeyCode, bool IsDown)
{
	bool bKeyAccepted(false);
	
	switch(KeyCode)
	{
	case 31: // I
		{
			// change triangle front and back
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedTriangles.empty() == false))
			{
				for(auto Triangle : g_SelectedTriangles)
				{
					Triangle->Invert();
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 39: // S
		{
			// separate all triangle points of selected points and triangles
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedTriangles.empty() == false) && (g_SelectedPoints.empty() == false))
			{
				for(auto Point : g_SelectedPoints)
				{
					for(auto Triangle : g_SelectedTriangles)
					{
						auto OldTrianglePoint(Triangle->GetTrianglePoint(Point));
						
						if(OldTrianglePoint != nullptr)
						{
							auto NewTrianglePoint(new TrianglePoint());
							
							NewTrianglePoint->SetPoint(Point);
							NewTrianglePoint->_Normal = Triangle->GetTriangleNormal();
							if(OldTrianglePoint == Triangle->GetTrianglePoint(1))
							{
								Triangle->SetTrianglePoint<0>(NewTrianglePoint);
							}
							else if(OldTrianglePoint == Triangle->GetTrianglePoint(2))
							{
								Triangle->SetTrianglePoint<1>(NewTrianglePoint);
							}
							else if(OldTrianglePoint == Triangle->GetTrianglePoint(3))
							{
								Triangle->SetTrianglePoint<2>(NewTrianglePoint);
							}
						}
					}
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 33: // P
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedTriangles.empty() == false))
			{
				// select all points on the selected triangles
				g_SelectedPoints.clear();
				
				std::vector< Point * > PointsToSelect;
				
				for(auto Triangle : g_SelectedTriangles)
				{
					PointsToSelect.push_back(Triangle->GetPoint(1));
					PointsToSelect.push_back(Triangle->GetPoint(2));
					PointsToSelect.push_back(Triangle->GetPoint(3));
				}
				std::sort(PointsToSelect.begin(), PointsToSelect.end());
				std::unique_copy(PointsToSelect.begin(), PointsToSelect.end(), std::back_inserter(g_SelectedPoints));
				bKeyAccepted = true;
			}
			
			break;
		}
	case 30: // U
		{
			for(auto Point : g_SelectedPoints)
			{
				TrianglePoint * NewTrianglePoint(nullptr);
				
				for(auto Triangle : g_SelectedTriangles)
				{
					auto OldTrianglePoint(Triangle->GetTrianglePoint(Point));
					
					if(OldTrianglePoint != nullptr)
					{
						if(NewTrianglePoint == nullptr)
						{
							NewTrianglePoint = new TrianglePoint();
							NewTrianglePoint->SetPoint(Point);
							NewTrianglePoint->_Normal.Set(0.0f, 0.0f, 0.0f);
						}
						NewTrianglePoint->_Normal.Translate(OldTrianglePoint->_Normal);
						if(OldTrianglePoint == Triangle->GetTrianglePoint(1))
						{
							Triangle->SetTrianglePoint<0>(NewTrianglePoint);
						}
						else if(OldTrianglePoint == Triangle->GetTrianglePoint(2))
						{
							Triangle->SetTrianglePoint<1>(NewTrianglePoint);
						}
						else if(OldTrianglePoint == Triangle->GetTrianglePoint(3))
						{
							Triangle->SetTrianglePoint<2>(NewTrianglePoint);
						}
					}
				}
				if(NewTrianglePoint != nullptr)
				{
					NewTrianglePoint->_Normal.Normalize();
				}
			}
			bKeyAccepted = true;
			
			break;
		}
	case 36: // ENTER
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedTriangles.empty() == false))
			{
				for(auto Triangle : g_SelectedTriangles)
				{
					Triangle->RealignNormal();
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 38: // A
		{
			if((IsDown == true) && (g_Keyboard.IsAnyControlActive() == true))
			{
				// select all triangles in the scene
				g_SelectedTriangles.clear();
				std::copy(std::begin(g_Triangles), std::end(g_Triangles), std::back_inserter(g_SelectedTriangles));
				bKeyAccepted = true;
			}
			
			break;
		}
	case 58: // M
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedTriangles.empty() == false))
			{
				// for each selected triangle, create mid points along the edges
				// then create four new triangles and remove the selected one
				std::vector< Triangle * > NewTriangles;
				
				while(g_SelectedTriangles.size() > 0)
				{
					auto SelectedTriangle{g_SelectedTriangles.front()};
					auto Point1{SelectedTriangle->GetPoint(1)};
					auto Point2{SelectedTriangle->GetPoint(2)};
					auto Point3{SelectedTriangle->GetPoint(3)};
					auto MidPoint1{CreatePoint((Point1->GetPosition() + Point2->GetPosition()) / 2.0f)};
					auto MidPoint2{CreatePoint((Point2->GetPosition() + Point3->GetPosition()) / 2.0f)};
					auto MidPoint3{CreatePoint((Point3->GetPosition() + Point1->GetPosition()) / 2.0f)};
					auto Length1{Point1->GetPosition().Length()};
					auto Length2{Point2->GetPosition().Length()};
					auto Length3{Point3->GetPosition().Length()};
					
					MidPoint1->SetPosition(MidPoint1->GetPosition().Normalized().Scale((Length1 + Length2) / 2.0f));
					MidPoint2->SetPosition(MidPoint2->GetPosition().Normalized().Scale((Length2 + Length3) / 2.0f));
					MidPoint3->SetPosition(MidPoint3->GetPosition().Normalized().Scale((Length3 + Length1) / 2.0f));
					DeleteTriangle(SelectedTriangle);
					NewTriangles.push_back(CreateTriangle(Point1, MidPoint1, MidPoint3));
					NewTriangles.push_back(CreateTriangle(MidPoint1, Point2, MidPoint2));
					NewTriangles.push_back(CreateTriangle(MidPoint3, MidPoint2, Point3));
					NewTriangles.push_back(CreateTriangle(MidPoint1, MidPoint2, MidPoint3));
				}
				std::copy(NewTriangles.begin(), NewTriangles.end(), std::back_inserter(g_SelectedTriangles));
				bKeyAccepted = true;
			}
			
			break;
		}
	case 119: // DELETE
		{
			if(g_SelectedTriangles.size() > 0)
			{
				while(g_SelectedTriangles.size() > 0)
				{
					DeleteTriangle(g_SelectedTriangles.front());
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	}
	
	return bKeyAccepted;
}

bool AcceptKeyInCameraView(int KeyCode, bool IsDown)
{
	bool bKeyAccepted(false);
	
	switch(KeyCode)
	{
	case 54: // C
		{
			if(IsDown == true)
			{
				auto NewCamera(new Camera());
				
				g_Cameras.push_back(NewCamera);
				g_SelectedCamera = NewCamera;
				bKeyAccepted = true;
			}
			
			break;
		}
	case 36: // ENTER
		{
			if(g_HoveredCamera != nullptr)
			{
				g_CurrentCamera = g_HoveredCamera;
				bKeyAccepted = true;
			}
			else if(g_SelectedCamera != nullptr)
			{
				g_CurrentCamera = g_SelectedCamera;
				bKeyAccepted = true;
			}
			
			break;
		}
	case 114: // RIGHT
		{
			if(IsDown == true)
			{
				if(g_SelectedCamera != nullptr)
				{
					g_SelectedCamera->SetPosition(GetSnappedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Right));
				}
				else if(g_CurrentCamera != nullptr)
				{
					if(g_Keyboard.IsAnyControlActive() == true)
					{
						g_CurrentCamera->TurnRight(0.02f);
					}
					else
					{
						g_CurrentCamera->MoveRight(0.2f);
					}
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 113: // LEFT
		{
			if(IsDown == true)
			{
				if(g_SelectedCamera != nullptr)
				{
					g_SelectedCamera->SetPosition(GetSnappedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Left));
				}
				else if(g_CurrentCamera != nullptr)
				{
					if(g_Keyboard.IsAnyControlActive() == true)
					{
						g_CurrentCamera->TurnLeft(0.02f);
					}
					else
					{
						g_CurrentCamera->MoveLeft(0.2f);
					}
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 111: // UP
		{
			if(IsDown == true)
			{
				if(g_SelectedCamera != nullptr)
				{
					g_SelectedCamera->SetPosition(GetSnappedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Up));
				}
				else if(g_CurrentCamera != nullptr)
				{
					if(g_Keyboard.IsAnyControlActive() == true)
					{
						g_CurrentCamera->TurnUp(0.02f);
					}
					else
					{
						g_CurrentCamera->MoveUp(0.2f);
					}
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 116: // DOWN
		{
			if(IsDown == true)
			{
				if(g_SelectedCamera != nullptr)
				{
					g_SelectedCamera->SetPosition(GetSnappedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Down));
				}
				else if(g_CurrentCamera != nullptr)
				{
					if(g_Keyboard.IsAnyControlActive() == true)
					{
						g_CurrentCamera->TurnDown(0.02f);
					}
					else
					{
						g_CurrentCamera->MoveDown(0.2f);
					}
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 117: // PAGE DOWN
		{
			if(IsDown == true)
			{
				if(g_SelectedCamera != nullptr)
				{
					g_SelectedCamera->SetPosition(GetSnappedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Backward));
				}
				else if(g_CurrentCamera != nullptr)
				{
					g_CurrentCamera->MoveBackward(0.2f);
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 112: // PAGE UP
		{
			if(IsDown == true)
			{
				if(g_SelectedCamera != nullptr)
				{
					g_SelectedCamera->SetPosition(GetSnappedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Forward));
				}
				else if(g_CurrentCamera != nullptr)
				{
					g_CurrentCamera->MoveForward(0.2f);
				}
				bKeyAccepted = true;
			}
			
			break;
		}
	case 110: // HOME
		{
			if(g_SelectedCamera != nullptr)
			{
				g_SelectedCamera->SetFieldOfViewY(g_SelectedCamera->GetFieldOfViewY() + 0.01f);
			}
			else if(g_CurrentCamera != nullptr)
			{
				g_CurrentCamera->SetFieldOfViewY(g_CurrentCamera->GetFieldOfViewY() + 0.01f);
				vSetupProjection();
			}
			
			break;
		}
	case 115: // END
		{
			if(g_SelectedCamera != nullptr)
			{
				g_SelectedCamera->SetFieldOfViewY(g_SelectedCamera->GetFieldOfViewY() - 0.01f);
			}
			else if(g_CurrentCamera != nullptr)
			{
				g_CurrentCamera->SetFieldOfViewY(g_CurrentCamera->GetFieldOfViewY() - 0.01f);
				vSetupProjection();
			}
			
			break;
		}
	case 119: // DELETE
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true))
			{
				if(g_SelectedCamera != nullptr)
				{
					DeleteCamera(g_SelectedCamera);
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	}
	
	return bKeyAccepted;
}

XMLStream & mesh(XMLStream & XMLStream)
{
	XMLStream << element << "mesh";
	
	std::map< Point *, unsigned long > PointMap;
	std::map< TrianglePoint *, unsigned long > TrianglePointMap;
	unsigned long PointIdentifier(0);
	
	for(auto Point : g_Points)
	{
		XMLStream << element << "point" << attribute << "identifier" << value << PointIdentifier << attribute << "position-x" << value << Point->GetPosition()[0] << attribute << "position-y" << value << Point->GetPosition()[1] << attribute << "position-z" << value << Point->GetPosition()[2] << end;
		PointMap[Point] = PointIdentifier;
		++PointIdentifier;
	}
	std::cout << "Written " << PointIdentifier << " points to the stream." << std::endl;
	
	unsigned long TrianglePointIdentifier(0);
	
	for(auto Point : g_Points)
	{
		for(auto TrianglePoint : Point->GetTrianglePoints())
		{
			XMLStream << element << "triangle-point" << attribute << "identifier" << value << TrianglePointIdentifier << attribute << "normal-x" << value << TrianglePoint->_Normal[0] << attribute << "normal-y" << value << TrianglePoint->_Normal[1] << attribute << "normal-z" << value << TrianglePoint->_Normal[2];
			XMLStream << element << "point" << attribute << "point-identifier" << value << PointMap[TrianglePoint->GetPoint()] << end;
			XMLStream << end;
			TrianglePointMap[TrianglePoint] = TrianglePointIdentifier;
			++TrianglePointIdentifier;
		}
	}
	std::cout << "Written " << TrianglePointIdentifier << " triangle points to the stream." << std::endl;
	
	unsigned long TriangleIdentifier(0);
	
	for(auto Triangle : g_Triangles)
	{
		XMLStream << element << "triangle" << attribute << "identifier" << value << TriangleIdentifier;
		XMLStream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << TrianglePointMap[Triangle->GetTrianglePoint(1)] << end;
		XMLStream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << TrianglePointMap[Triangle->GetTrianglePoint(2)] << end;
		XMLStream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << TrianglePointMap[Triangle->GetTrianglePoint(3)] << end;
		XMLStream << end;
		++TriangleIdentifier;
	}
	std::cout << "Written " << TriangleIdentifier << " triangles to the stream." << std::endl;
	
	auto NumberOfMarkers{0};
	
	for(auto Marker : g_Markers)
	{
		XMLStream << *Marker << end;
		NumberOfMarkers++;
	}
	std::cout << "Written " << NumberOfMarkers << " markers to the stream." << std::endl;
	
	return XMLStream;
}

void Import(Marker * Marker)
{
	g_Markers.push_back(Marker);
}

void ImportMesh(const std::string & FilePath)
{
	std::cout << "Importing from \"" << FilePath << "\"." << std::endl;
	
	std::ifstream InputFileStream(FilePath);
	MeshReader MeshReader(InputFileStream);
	
	MeshReader.Parse();
	
	std::vector< Point * > Points(MeshReader.GetPoints());
	std::vector< Triangle * > Triangles(MeshReader.GetTriangles());
	
	copy(Points.begin(), Points.end(), back_inserter(g_Points));
	copy(Triangles.begin(), Triangles.end(), back_inserter(g_Triangles));
	for(auto Marker : MeshReader.GetMarkers())
	{
		Import(Marker);
	}
}

inline long int GetRandomInteger(long int HighestValue)
{
	return random() % (HighestValue + 1);
}

bool AcceptKeyInMarkerView(int KeyCode, bool IsDown)
{
	auto Result{false};
	
	switch(KeyCode)
	{
	case 57: // N
		{
			if(IsDown == true)
			{
				auto NewMarker(new Marker());
				
				NewMarker->SetPosition(Vector3f::CreateFromComponents(0.0f, 0.0f, 0.0f));
				
				std::string Identifier{""};
				
				for(auto CharacterIndex = 0; CharacterIndex < 8; ++CharacterIndex)
				{
					Identifier += (char)('a' + GetRandomInteger(26));
				}
				NewMarker->SetIdentifier(Identifier);
				g_Markers.push_back(NewMarker);
			}
			Result = true;
			
			break;
		}
	case 119: // DELETE
		{
			if(IsDown == true)
			{
				// delete selected marker
				if(g_SelectedMarker != nullptr)
				{
					DeleteMarker(g_SelectedMarker);
				}
			}
			Result = true;
			
			break;
		}
	case 114: // RIGHT
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedMarker != nullptr))
			{
				g_SelectedMarker->SetPosition(GetSnappedStepInView(g_SelectedMarker->GetPosition(), FixedDirection::Right));
			}
			Result = true;
			
			break;
		}
	case 113: // LEFT
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedMarker != nullptr))
			{
				g_SelectedMarker->SetPosition(GetSnappedStepInView(g_SelectedMarker->GetPosition(), FixedDirection::Left));
			}
			Result = true;
			
			break;
		}
	case 111: // UP
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedMarker != nullptr))
			{
				g_SelectedMarker->SetPosition(GetSnappedStepInView(g_SelectedMarker->GetPosition(), FixedDirection::Up));
			}
			Result = true;
			
			break;
		}
	case 116: // DOWN
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedMarker != nullptr))
			{
				g_SelectedMarker->SetPosition(GetSnappedStepInView(g_SelectedMarker->GetPosition(), FixedDirection::Down));
			}
			Result = true;
			
			break;
		}
	case 117: // PAGE DOWN
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedMarker != nullptr))
			{
				g_SelectedMarker->SetPosition(GetSnappedStepInView(g_SelectedMarker->GetPosition(), FixedDirection::Backward));
			}
			Result = true;
			
			break;
		}
	case 112: // PAGE UP
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true) && (g_SelectedMarker != nullptr))
			{
				g_SelectedMarker->SetPosition(GetSnappedStepInView(g_SelectedMarker->GetPosition(), FixedDirection::Forward));
			}
			Result = true;
			
			break;
		}
	}
	
	return Result;
}

bool AcceptKeyInModelView(int KeyCode, bool IsDown)
{
	bool bKeyAccepted{true};
	
	switch(KeyCode)
	{
	case 10: // 1
		{
			if((IsDown == true) && (g_Keyboard.IsAnyShiftActive() == true))
			{
				g_ModelerView = ModelerView::Point;
			}
			
			break;
		}
	case 11: // 2
		{
			if((IsDown == true) && (g_Keyboard.IsAnyShiftActive() == true))
			{
				g_ModelerView = ModelerView::Triangle;
			}
			
			break;
		}
	case 12: // 3
		{
			if((IsDown == true) && (g_Keyboard.IsAnyShiftActive() == true))
			{
				g_ModelerView = ModelerView::Model;
			}
			
			break;
		}
	case 13: // 4
		{
			if((IsDown == true) && (g_Keyboard.IsAnyShiftActive() == true))
			{
				g_ModelerView = ModelerView::Camera;
			}
			
			break;
		}
	case 14: // 5
		{
			if((IsDown == true) && (g_Keyboard.IsAnyShiftActive() == true))
			{
				g_ModelerView = ModelerView::Marker;
			}
			
			break;
		}
	case 54: // C
		{
			if((IsDown == true) && (g_Keyboard.IsAltActive() == true))
			{
				vToggleCullFace();
			}
			
			break;
		}
	case 40: // D
		{
			if(IsDown == true)
			{
				if(g_Keyboard.IsAltActive() == true)
				{
					vToggleDepthTest();
				}
			}
			
			break;
		}
	case 26: // E
		{
			if((IsDown == false) && (g_Keyboard.IsAnyControlActive() == true))
			{
				std::cout << "Exporting to mesh.xml." << std::endl;
				
				std::ofstream OutputFileStream("mesh.xml");
				XMLStream XMLStream(OutputFileStream);
				
				XMLStream << mesh;
			}
			
			break;
		}
	case 41: // F
		{
			if((IsDown == true) && (g_Keyboard.IsAltActive() == true))
			{
				vToggleFrontFace();
			}
		
			break;
		}
	case 31: // I
		{
			if((IsDown == false) && (g_Keyboard.IsAnyControlActive() == true))
			{
				ImportMesh("mesh.xml");
			}
			
			break;
		}
	case 46: // L
		{
			if(g_Keyboard.IsAltActive() == true)
			{
				if(IsDown == true)
				{
					vToggleLighting();
				}
			}
			else if(g_Keyboard.IsAnyControlActive() == true)
			{
				if(IsDown == false)
				{
					std::cout << "Loading scene.xml." << std::endl;
					vClearScene();
					
					std::ifstream InputFileStream("scene.xml");
					SceneReader SceneReader(InputFileStream);
					
					SceneReader.Parse();
					
					std::vector< Point * > Points(SceneReader.GetPoints());
					std::vector< Triangle * > Triangles(SceneReader.GetTriangles());
					
					copy(Points.begin(), Points.end(), back_inserter(g_Points));
					copy(Triangles.begin(), Triangles.end(), back_inserter(g_Triangles));
					for(auto & LightDescription : SceneReader.GetLightDescriptions())
					{
						auto NewLight(new Light());
						
						NewLight->SetPosition(LightDescription.m_Position);
						NewLight->SetDiffuseColor(LightDescription.m_DiffuseColor[0], LightDescription.m_DiffuseColor[1], LightDescription.m_DiffuseColor[2], LightDescription.m_DiffuseColor[3]);
						if(LightDescription.m_bEnabled == true)
						{
							NewLight->Enable();
						}
					}
					for(auto & CameraDescription : SceneReader.GetCameraDescriptions())
					{
						auto NewCamera(new Camera());
						
						NewCamera->SetPosition(CameraDescription.Position);
						NewCamera->SetOrientation(CameraDescription.Orientation);
						NewCamera->SetFieldOfViewY(CameraDescription.FieldOfViewY);
						g_Cameras.push_back(NewCamera);
					}
					for(auto & Marker : SceneReader.GetMarkers())
					{
						Import(Marker);
					}
					if(g_Cameras.size() > 0)
					{
						g_CurrentCamera = g_Cameras.front();
						vSetupProjection();
					}
				}
			}
			else
			{
				if(IsDown == true)
				{
					auto NewLight(new Light());
					
					NewLight->SetPosition(Vector3f::CreateZero());
					NewLight->SetDiffuseColor(1.0f, 1.0f, 1.0f);
					NewLight->Enable();
				}
			}
			
			break;
		}
	case 33: // P
		{
			if((IsDown == true) && (g_Keyboard.IsAltActive() == true))
			{
				g_CullPoints = !g_CullPoints;
			}
			
			break;
		}
	case 39: // S
		{
			if(g_Keyboard.IsAltActive() == true)
			{
				if(IsDown == true)
				{
					vToggleSnapping();
				}
			}
			else if(g_Keyboard.IsAnyControlActive() == true)
			{
				if(IsDown == false)
				{
					std::cout << "Saving scene.xml." << std::endl;
					
					std::ofstream OutputFileStream("scene.xml");
					XMLStream XMLStream(OutputFileStream);
					
					XMLStream << element << "scene";
					for(auto Light : Light::GetLights())
					{
						XMLStream << light(Light) << end;
					}
					for(auto Camera : g_Cameras)
					{
						XMLStream << camera(Camera) << end;
					}
					XMLStream << mesh << end;
				}
			}
			else
			{
				if(IsDown == true)
				{
					CreateRegularIcosahedron();
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	case 36: // ENTER
		{
			if(IsDown == true)
			{
				if(g_SelectedLight != nullptr)
				{
					if(g_SelectedLight->IsEnabled() == true)
					{
						g_SelectedLight->Disable();
					}
					else
					{
						g_SelectedLight->Enable();
					}
				}
			}
			
			break;
		}
	case 9: // ESCAPE
		{
			if(IsDown == false)
			{
				g_Quit = true;
			}
			
			break;
		}
	case 119: // DELETE
		{
			if(IsDown == false)
			{
				if(g_SelectedLight != nullptr)
				{
					delete g_SelectedLight;
				}
			}
			
			break;
		}
	case 86: // NUMPAD PLUS
		{
			if(IsDown == true)
			{
				GLfloat fPointSize;
				
				glGetFloatv(GL_POINT_SIZE, &fPointSize);
				fPointSize += 1.0f;
				glPointSize(fPointSize);
			}
			
			break;
		}
	case 82: // NUMPAD MINUS
		{
			if(IsDown == true)
			{
				GLfloat fPointSize;
				
				glGetFloatv(GL_POINT_SIZE, &fPointSize);
				fPointSize -= 1.0f;
				glPointSize(fPointSize);
			}
			
			break;
		}
	case 63: // NUMPAD ASTERISK
		{
			if(IsDown == true)
			{
				vTogglePointSmooth();
			}
			
			break;
		}
	case 59: // COMMA
		{
			if(IsDown == true)
			{
				g_SnapFactor *= 10;
			}
			
			break;
		}
	case 60: // FULL STOP
		{
			if(IsDown == true)
			{
				g_SnapFactor /= 10;
			}
			
			break;
		}
	case 34: // LEFT BRACKET
		{
			if(g_SelectedTriangles.size() == 1)
			{
				Point * pPoint(0);
				
				if(g_SelectedPoints.size() > 0)
				{
					pPoint = g_SelectedPoints[0];
				}
				g_SelectedPoints.clear();
				if(pPoint == g_SelectedTriangles.front()->GetPoint(2))
				{
					g_SelectedPoints.push_back(g_SelectedTriangles.front()->GetPoint(1));
				}
				else if(pPoint == g_SelectedTriangles.front()->GetPoint(3))
				{
					g_SelectedPoints.push_back(g_SelectedTriangles.front()->GetPoint(2));
				}
				else
				{
					g_SelectedPoints.push_back(g_SelectedTriangles.front()->GetPoint(3));
				}
			}
			
			break;
		}
	case 35: // RIGHT BRACKET
		{
			if(g_SelectedTriangles.size() == 1)
			{
				Point * pPoint(0);
				
				if(g_SelectedPoints.size() > 0)
				{
					pPoint = g_SelectedPoints[0];
				}
				g_SelectedPoints.clear();
				if(pPoint == g_SelectedTriangles.front()->GetPoint(1))
				{
					g_SelectedPoints.push_back(g_SelectedTriangles.front()->GetPoint(2));
				}
				else if(pPoint == g_SelectedTriangles.front()->GetPoint(2))
				{
					g_SelectedPoints.push_back(g_SelectedTriangles.front()->GetPoint(3));
				}
				else
				{
					g_SelectedPoints.push_back(g_SelectedTriangles.front()->GetPoint(1));
				}
			}
			
			break;
		}
	case 114: // RIGHT
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true))
			{
				if(g_SelectedLight != nullptr)
				{
					g_SelectedLight->SetPosition(GetSnappedStepInView(g_SelectedLight->GetPosition(), FixedDirection::Right));
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	case 113: // LEFT
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true))
			{
				if(g_SelectedLight != nullptr)
				{
					g_SelectedLight->SetPosition(GetSnappedStepInView(g_SelectedLight->GetPosition(), FixedDirection::Left));
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	case 111: // UP
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true))
			{
				if(g_SelectedLight != nullptr)
				{
					g_SelectedLight->SetPosition(GetSnappedStepInView(g_SelectedLight->GetPosition(), FixedDirection::Up));
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	case 116: // DOWN
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true))
			{
				if(g_SelectedLight != nullptr)
				{
					g_SelectedLight->SetPosition(GetSnappedStepInView(g_SelectedLight->GetPosition(), FixedDirection::Down));
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	case 117: // PAGE DOWN
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true))
			{
				if(g_SelectedLight != nullptr)
				{
					g_SelectedLight->SetPosition(GetSnappedStepInView(g_SelectedLight->GetPosition(), FixedDirection::Backward));
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	case 112: // PAGE UP
		{
			if((IsDown == true) && (g_Keyboard.IsNoModifierKeyActive() == true))
			{
				if(g_SelectedLight != nullptr)
				{
					g_SelectedLight->SetPosition(GetSnappedStepInView(g_SelectedLight->GetPosition(), FixedDirection::Forward));
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	case 67: // F1
		{
			if(IsDown == false)
			{
				// switch to normalized front view
				if(g_CurrentCamera != nullptr)
				{
					if((g_UpAxis == FixedAxis::PositiveZ) && (g_ForwardAxis == FixedAxis::PositiveX))
					{
						g_CurrentCamera->SetOrientation(Quaternion::CreateAsRotationX(M_PI_2).RotateY(M_PI_2));
						g_CurrentCamera->SetPosition(Vector3f::CreateTranslationX(4.0f));
						bKeyAccepted = true;
					}
					else
					{
						std::cout << "Unknown combination: View=Front, Up=" << GetAxisString(g_UpAxis) << ", Front=" << GetAxisString(g_ForwardAxis) << std::endl;
						assert(false);
					}
				}
			}
			
			break;
		}
	case 68: // F2
		{
			if(IsDown == false)
			{
				// switch to normalized left view
				if(g_CurrentCamera != nullptr)
				{
					if((g_UpAxis == FixedAxis::PositiveZ) && (g_ForwardAxis == FixedAxis::PositiveX))
					{
						g_CurrentCamera->SetOrientation(Quaternion::CreateAsRotationX(M_PI_2));
						g_CurrentCamera->SetPosition(Vector3f::CreateTranslationY(-4.0f));
						bKeyAccepted = true;
					}
					else
					{
						std::cout << "Unknown combination: View=Left, Up=" << GetAxisString(g_UpAxis) << ", Front=" << GetAxisString(g_ForwardAxis) << std::endl;
						assert(false);
					}
				}
			}
			
			break;
		}
	case 69: // F3
		{
			if(IsDown == false)
			{
				// switch to normalized right view
				if(g_CurrentCamera != nullptr)
				{
					if((g_UpAxis == FixedAxis::PositiveZ) && (g_ForwardAxis == FixedAxis::PositiveX))
					{
						g_CurrentCamera->SetOrientation(Quaternion::CreateAsRotationY(M_PI).RotateX(-M_PI_2));
						g_CurrentCamera->SetPosition(Vector3f::CreateTranslationY(4.0f));
						bKeyAccepted = true;
					}
					else
					{
						std::cout << "Unknown combination: View=Right, Up=" << GetAxisString(g_UpAxis) << ", Front=" << GetAxisString(g_ForwardAxis) << std::endl;
						assert(false);
					}
				}
			}
			
			break;
		}
	case 70: // F4
		{
			if(IsDown == false)
			{
				// switch to normalized back view
				if(g_CurrentCamera != nullptr)
				{
					if((g_UpAxis == FixedAxis::PositiveZ) && (g_ForwardAxis == FixedAxis::PositiveX))
					{
						g_CurrentCamera->SetOrientation(Quaternion::CreateAsRotationX(M_PI_2).RotateY(-M_PI_2));
						g_CurrentCamera->SetPosition(Vector3f::CreateTranslationX(-4.0f));
						bKeyAccepted = true;
					}
					else
					{
						std::cout << "Unknown combination: View=Back, Up=" << GetAxisString(g_UpAxis) << ", Front=" << GetAxisString(g_ForwardAxis) << std::endl;
						assert(false);
					}
				}
			}
			
			break;
		}
	case 71: // F5
		{
			if(IsDown == false)
			{
				// switch to normalized top view
				if(g_CurrentCamera != nullptr)
				{
					if((g_UpAxis == FixedAxis::PositiveZ) && (g_ForwardAxis == FixedAxis::PositiveX))
					{
						g_CurrentCamera->SetOrientation(Quaternion::CreateAsRotationZ(M_PI_2));
						g_CurrentCamera->SetPosition(Vector3f::CreateTranslationZ(4.0f));
						bKeyAccepted = true;
					}
					else
					{
						std::cout << "Unknown combination: View=Top, Up=" << GetAxisString(g_UpAxis) << ", Front=" << GetAxisString(g_ForwardAxis) << std::endl;
						assert(false);
					}
				}
			}
			
			break;
		}
	case 72: // F6
		{
			if(IsDown == false)
			{
				// switch to normalized bottom view
				if(g_CurrentCamera != nullptr)
				{
					if((g_UpAxis == FixedAxis::PositiveZ) && (g_ForwardAxis == FixedAxis::PositiveX))
					{
						g_CurrentCamera->SetOrientation(Quaternion::CreateAsRotationZ(M_PI_2).RotateY(M_PI));
						g_CurrentCamera->SetPosition(Vector3f::CreateTranslationZ(-4.0f));
						bKeyAccepted = true;
					}
					else
					{
						std::cout << "Unknown combination: View=Bottom, Up=" << GetAxisString(g_UpAxis) << ", Front=" << GetAxisString(g_ForwardAxis) << std::endl;
						assert(false);
					}
				}
			}
			
			break;
		}
	case 73: // F7
		{
			if(IsDown == false)
			{
				// switch to normalized view
				if(g_CurrentCamera != nullptr)
				{
					g_CurrentCamera->SetPosition(Vector3f::CreateTranslationZ(4.0f));
					g_CurrentCamera->SetOrientation(Quaternion::CreateIdentity());
					bKeyAccepted = true;
				}
			}
			
			break;
		}
	case 96: // F12
		{
			if(IsDown == false)
			{
				Light::DeleteLights();
				
				auto NewLight1(new Light());
				
				NewLight1->SetPosition(Vector3f::CreateFromComponents(6.0f, 12.0f, 12.0f));
				NewLight1->SetDiffuseColor(0.5f, 0.5f, 0.7f);
				NewLight1->Enable();
				
				auto NewLight2(new Light());
				
				NewLight2->SetPosition(Vector3f::CreateFromComponents(-12.0f, 6.0f, -12.0f));
				NewLight2->SetDiffuseColor(0.5f, 0.7f, 0.5f);
				NewLight2->Enable();
				
				auto NewLight3(new Light());
				
				NewLight3->SetPosition(Vector3f::CreateFromComponents(-12.0f, -12.0f, 6.0f));
				NewLight3->SetDiffuseColor(0.7f, 0.5f, 0.5f);
				NewLight3->Enable();
				
				auto NewLight4(new Light());
				
				NewLight4->SetPosition(Vector3f::CreateFromComponents(6.0f, -12.0f, -12.0f));
				NewLight4->SetDiffuseColor(0.4f, 0.65f, 0.65f);
				NewLight4->Enable();
			}
		}
	default:
		{
			bKeyAccepted = false;
		}
	}
	
	return bKeyAccepted;
}

void KeyEvent(int KeyCode, bool IsDown)
{
	ON_DEBUG(std::cout << "Key event " << KeyCode << " / " << IsDown << "." << std::endl);
	g_Keyboard.SetKey(KeyCode, IsDown);
	
	bool KeyAccepted(false);
	
	if(g_ModelerView == ModelerView::Point)
	{
		KeyAccepted = AcceptKeyInPointView(KeyCode, IsDown);
	}
	else if(g_ModelerView == ModelerView::Triangle)
	{
		KeyAccepted = AcceptKeyInTriangleView(KeyCode, IsDown);
	}
	else if(g_ModelerView == ModelerView::Camera)
	{
		KeyAccepted = AcceptKeyInCameraView(KeyCode, IsDown);
	}
	else if(g_ModelerView == ModelerView::Marker)
	{
		KeyAccepted = AcceptKeyInMarkerView(KeyCode, IsDown);
	}
	if(KeyAccepted == false)
	{
		AcceptKeyInModelView(KeyCode, IsDown);
	}
}

void MouseButtonEvent(MouseButton Button, bool IsDown, const Vector2f & MousePosition)
{
	switch(Button)
	{
	case MouseButton::Left:
		{
			if(IsDown == false)
			{
				if(g_HoveredPoint != nullptr)
				{
					if(g_Keyboard.IsAnyShiftActive() == true)
					{
						auto HoveredPointIterator{std::find(g_SelectedPoints.begin(), g_SelectedPoints.end(), g_HoveredPoint)};
						
						if(HoveredPointIterator == g_SelectedPoints.end())
						{
							g_SelectedPoints.push_back(g_HoveredPoint);
						}
						else
						{
							g_SelectedPoints.erase(HoveredPointIterator);
						}
					}
					else
					{
						if((g_SelectedPoints.size() == 1) && (g_SelectedPoints.front() == g_HoveredPoint))
						{
							g_SelectedPoints.clear();
						}
						else
						{
							g_SelectedPoints.clear();
							g_SelectedPoints.push_back(g_HoveredPoint);
						}
					}
				}
				else if(g_HoveredTriangle != nullptr)
				{
					if(g_Keyboard.IsAnyShiftActive() == true)
					{
						auto HoveredTriangleIterator{std::find(g_SelectedTriangles.begin(), g_SelectedTriangles.end(), g_HoveredTriangle)};
						
						if(HoveredTriangleIterator == g_SelectedTriangles.end())
						{
							g_SelectedTriangles.push_back(g_HoveredTriangle);
						}
						else
						{
							g_SelectedTriangles.erase(HoveredTriangleIterator);
						}
					}
					else
					{
						if((g_SelectedTriangles.size() == 1) && (g_SelectedTriangles.front() == g_HoveredTriangle))
						{
							g_SelectedTriangles.clear();
						}
						else
						{
							g_SelectedTriangles.clear();
							g_SelectedTriangles.push_back(g_HoveredTriangle);
						}
					}
				}
				else if(g_HoveredLight != nullptr)
				{
					if(g_SelectedLight == g_HoveredLight)
					{
						g_SelectedLight = nullptr;
					}
					else
					{
						g_SelectedLight = g_HoveredLight;
					}
				}
				else if(g_HoveredCamera != nullptr)
				{
					if(g_SelectedCamera == g_HoveredCamera)
					{
						g_SelectedCamera = nullptr;
					}
					else
					{
						g_SelectedCamera = g_HoveredCamera;
					}
				}
				else if(g_HoveredMarker != nullptr)
				{
					if(g_SelectedMarker == g_HoveredMarker)
					{
						g_SelectedMarker = nullptr;
					}
					else
					{
						g_SelectedMarker = g_HoveredMarker;
					}
				}
			}
			
			break;
		}
	case MouseButton::Middle:
		{
			if(IsDown == true)
			{
				g_MouseButton = MouseButton::Middle;
				g_LastMousePosition = MousePosition;
			}
			else
			{
				g_MouseButton = MouseButton::Undefined;
			}
			
			break;
		}
	case MouseButton::Right:
		{
			if(IsDown == true)
			{
				g_bMoved = false;
			}
			else
			{
				if(g_bMoved == false)
				{
					g_SelectedPoints.clear();
					g_SelectedTriangles.clear();
					g_SelectedLight = nullptr;
					g_SelectedCamera = nullptr;
					g_SelectedMarker = nullptr;
				}
			}
			
			break;
		}
	case MouseButton::WheelUp:
		{
			if(IsDown == true)
			{
				if(g_CurrentCamera != nullptr)
				{
					g_CurrentCamera->MoveForward(0.2f);
				}
			}
			
			break;
		}
	case MouseButton::WheelDown:
		{
			if(IsDown == true)
			{
				if(g_CurrentCamera != nullptr)
				{
					g_CurrentCamera->MoveBackward(0.2f);
				}
			}
			
			break;
		}
	default:
		{
			assert(false);
		}
	}
}

void MouseMotion(const Vector2f MousePosition)
{
	g_bMoved = true;
	g_MousePosition = MousePosition;
	
	auto Delta(MousePosition - g_LastMousePosition);
	
	g_LastMousePosition = MousePosition;
	if((g_MouseButton == MouseButton::Middle) && (g_CurrentCamera != nullptr))
	{
		g_CurrentCamera->SetOrientation(g_CurrentCamera->GetOrientation().RotatedX(-0.005 * Delta[1]).RotateY(-0.005 * Delta[0]));
	}
}

void Resize(void)
{
	vSetupViewport();
	vSetupProjection();
}

bool StartsWith(const std::string & One, const std::string & Two)
{
	auto OneIterator(One.begin());
	auto OneEnd(One.end());
	auto TwoIterator(Two.begin());
	auto TwoEnd(Two.end());
	
	for(; (OneIterator != OneEnd) && (TwoIterator != TwoEnd); ++OneIterator, ++TwoIterator)
	{
		if(*OneIterator != *TwoIterator)
		{
			return false;
		}
	}
	
	return TwoIterator == TwoEnd;
}

typedef GLXContext (* CreateContextWithAttributesFunction)(Display * Display, GLXFBConfig FBConfiguration, GLXContext ShareContext, bool DirectRendering, const int * AttributeList);

void CreateWindow(void)
{
	ON_DEBUG(std::cout << "Opening display." << std::endl);
	g_Display = XOpenDisplay(0);
	if(g_Display == 0)
	{
		std::cerr << "Coud not open the default display." << std::endl;
		exit(1);
	}
	ON_DEBUG(std::cout << "Checking for GLX availability." << std::endl);
	
	int ErrorBase(0);
	int EventBase(0);
	
	if(glXQueryExtension(g_Display, &ErrorBase, &EventBase) == true)
	{
		ON_DEBUG(std::cout << "  The GLX extension is available." << std::endl);
	}
	else
	{
		std::cerr << "The GLX extension is not available." << std::endl;
		exit(1);
	}
	ON_DEBUG(std::cout << "Checking for GLX version." << std::endl);
	
	int MajorVersionNumber(0);
	int MinorVersionNumber(0);
	
	if(glXQueryVersion(g_Display, &MajorVersionNumber, &MinorVersionNumber) == false)
	{
		std::cerr << "Checking the GLX version failed." << std::endl;
		exit(1);
	}
	else
	{
		ON_DEBUG(std::cout << "  The GLX version is " << MajorVersionNumber << "." << MinorVersionNumber << "." << std::endl);
		if((MajorVersionNumber < 1) || ((MajorVersionNumber == 1) && (MinorVersionNumber < 4)))
		{
			std::cerr << "The GLX version " << MajorVersionNumber << "." << MinorVersionNumber << " is below the minimal required version number 1.4." << std::endl;
			exit(1);
		}
	}
	ON_DEBUG(std::cout << "Getting default screen." << std::endl);
	
	int ScreenNumber(DefaultScreen(g_Display));
	
	ON_DEBUG(std::cout << "Getting the GLX server properties." << std::endl);
	
	const char * GLXServerVendor(glXQueryServerString(g_Display, ScreenNumber, GLX_VENDOR));
	
	if(GLXServerVendor != 0)
	{
		ON_DEBUG(std::cout << "  GLX server vendor: \"" << GLXServerVendor << "\"" << std::endl);
	}
	else
	{
		std::cerr << "The GLX server vendor string is invalid." << std::endl;
		exit(1);
	}
	
	const char * GLXServerVersion(glXQueryServerString(g_Display, ScreenNumber, GLX_VERSION));
	
	if(GLXServerVersion != 0)
	{
		ON_DEBUG(std::cout << "  GLX server version: \"" << GLXServerVersion << "\"" << std::endl);
	}
	else
	{
		std::cerr << "The GLX server version string is invalid." << std::endl;
		exit(1);
	}
	
	const char * GLXServerExtensionsString(glXQueryServerString(g_Display, ScreenNumber, GLX_EXTENSIONS));
	
	if(GLXServerExtensionsString != 0)
	{
		std::vector< std::string > GLXServerExtensions(SplitString(GLXServerExtensionsString, ' '));
		
		ON_DEBUG(std::cout << "  GLX server extensions:" << std::endl);
		for(std::vector< std::string >::iterator Iterator = GLXServerExtensions.begin(); Iterator != GLXServerExtensions.end(); ++Iterator)
		{
			ON_DEBUG(std::cout << "    \"" << *Iterator << '"' << std::endl);
		}
	}
	else
	{
		std::cerr << "The GLX server extensions string is invalid." << std::endl;
		exit(1);
	}
	ON_DEBUG(std::cout << "Getting the GLX client properties." << std::endl);
	
	const char * GLXClientVendor(glXGetClientString(g_Display, GLX_VENDOR));
	
	if(GLXClientVendor != 0)
	{
		ON_DEBUG(std::cout << "  GLX client vendor: \"" << GLXClientVendor << "\"" << std::endl);
	}
	else
	{
		std::cerr << "The GLX client vendor string is invalid." << std::endl;
		exit(1);
	}
	
	const char * GLXClientVersion(glXGetClientString(g_Display, GLX_VERSION));
	
	if(GLXClientVersion != 0)
	{
		ON_DEBUG(std::cout << "  GLX client version: \"" << GLXClientVersion << "\"" << std::endl);
	}
	else
	{
		std::cerr << "The GLX client version string is invalid." << std::endl;
		exit(1);
	}
	
	const char * GLXClientExtensionsString(glXGetClientString(g_Display, GLX_EXTENSIONS));
	
	if(GLXClientExtensionsString != 0)
	{
		std::vector< std::string > GLXClientExtensions(SplitString(GLXClientExtensionsString, ' '));
		
		ON_DEBUG(std::cout << "  GLX client extensions:" << std::endl);
		for(std::vector< std::string >::iterator Iterator = GLXClientExtensions.begin(); Iterator != GLXClientExtensions.end(); ++Iterator)
		{
			ON_DEBUG(std::cout << "    \"" << *Iterator << '"' << std::endl);
		}
	}
	else
	{
		std::cerr << "The GLX client extensions string is invalid." << std::endl;
		exit(1);
	}
	ON_DEBUG(std::cout << "Getting the GLX connection extensions." << std::endl);
	
	const char * GLXExtensionsString(glXQueryExtensionsString(g_Display, ScreenNumber));
	std::vector< std::string > ExtensionStrings(SplitString(GLXExtensionsString, ' '));
	bool GLX_ARB_create_context_Available(false);
	
	ON_DEBUG(std::cout << "  GLX connection extensions:" << std::endl);
	for(std::vector< std::string >::iterator Iterator = ExtensionStrings.begin(); Iterator != ExtensionStrings.end(); ++Iterator)
	{
		if(*Iterator == "GLX_ARB_create_context")
		{
			GLX_ARB_create_context_Available = true;
		}
		ON_DEBUG(std::cout << "    \"" << *Iterator << '"' << std::endl);
	}
	if(GLX_ARB_create_context_Available == false)
	{
		std::cerr << "Your GLX connection does not support the required extension GLX_ARB_create_context." << std::endl;
		exit(1);
	}
	
	int NumberOfConfigurations(0);
	int RequestedAttributes[] =
	{
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_X_RENDERABLE, True,
		GLX_BUFFER_SIZE, 32,
		GLX_DOUBLEBUFFER, True,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		0
	};
	ON_DEBUG(std::cout << "Choosing a FB configuration." << std::endl);
	GLXFBConfig * Configurations(glXChooseFBConfig(g_Display, ScreenNumber, RequestedAttributes, &NumberOfConfigurations));
	
	if(Configurations == 0)
	{
		std::cerr << "Sorry, could not find a suitable FB configuration." << std::endl;
		exit(1);
	}
	ON_DEBUG(std::cout << "Getting display dimensions." << std::endl);
	g_Width = DisplayWidth(g_Display, ScreenNumber);
	g_Height = DisplayHeight(g_Display, ScreenNumber);
	ON_DEBUG(std::cout << "Getting visualization information." << std::endl);
	
	XVisualInfo * VisualizationInformation(glXGetVisualFromFBConfig(g_Display, Configurations[0]));
	
	ON_DEBUG(std::cout << "Creating an X colormap." << std::endl);
	g_ColorMap = XCreateColormap(g_Display, RootWindow(g_Display, VisualizationInformation->screen), VisualizationInformation->visual, AllocNone);
	
	XSetWindowAttributes WindowAttributes;
	
	WindowAttributes.colormap = g_ColorMap;
	WindowAttributes.border_pixel = 0;
	WindowAttributes.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | PointerMotionMask;
	ON_DEBUG(std::cout << "Creating X window." << std::endl);
	g_Window = XCreateWindow(g_Display, RootWindow(g_Display, VisualizationInformation->screen), 0, 0, static_cast< unsigned int >(g_Width), static_cast< unsigned int >(g_Height), 0, VisualizationInformation->depth, InputOutput, VisualizationInformation->visual, CWBorderPixel | CWColormap | CWEventMask/* | CWOverrideRedirect*/, &WindowAttributes);
	if(g_Window == 0)
	{
		std::cerr << "Failed to create a window with the specified visualization information." << std::endl;
		exit(1);
	}
	XFree(VisualizationInformation);
	
	Atom wmDelete = XInternAtom(g_Display, "WM_DELETE_WINDOW", True);
	
	XSetWMProtocols(g_Display, g_Window, &wmDelete, 1);
	XStoreName(g_Display, g_Window, "LittleM - A small 3D modeller for Galactic Fall.");
	XMapWindow(g_Display, g_Window);
	ON_DEBUG(std::cout << "Getting function \"glXCreateContextAttribsARB\"." << std::endl);
	
	CreateContextWithAttributesFunction CreateContextWithAttributes(reinterpret_cast< CreateContextWithAttributesFunction >(glXGetProcAddressARB(reinterpret_cast< const GLubyte * >("glXCreateContextAttribsARB"))));
	
	if(CreateContextWithAttributes == 0)
	{
		std::cerr << "Failed to get the function \"glXCreateContextAttribsARB\"." << std::endl;
		exit(1);
	}
	
	int RequestedContextAttributes[] =
	{
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 1,
// 		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		None
	};
	
	ON_DEBUG(std::cout << "Creating GLX context." << std::endl);
	ON_DEBUG(std::cout << "  Asking for GLX context with version " << RequestedContextAttributes[1] << '.' << RequestedContextAttributes[3] << "." << std::endl);
	g_GLXContext = CreateContextWithAttributes(g_Display, Configurations[0], 0, true, RequestedContextAttributes);
	if(glXIsDirect(g_Display, g_GLXContext) == false)
	{
		std::cerr << "Failed to acquire direct rendering context." << std::endl;
		exit(1);
	}
	else
	{
		ON_DEBUG(std::cout << "  Direct rendering context acquired." << std::endl);
	}
	ON_DEBUG(std::cout << "Activating context." << std::endl);
	glXMakeCurrent(g_Display, g_Window, g_GLXContext);
	XFree(Configurations);
}

void InitializeOpenGL(void)
{
	vSetupViewport();
	vSetupProjection();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	vClearScene();
	
	GLint iNumberOfLights(0);
	
	glGetIntegerv(GL_MAX_LIGHTS, &iNumberOfLights);
	Light::InitializeFreeLights(iNumberOfLights);
	glEnable(GL_LIGHTING);
	
	auto NewLight(new Light());
	
	NewLight->SetPosition(Vector3f::CreateTranslationZ(4.0f));
	NewLight->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	NewLight->Enable();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	auto NewCamera(new Camera());
	
	g_Cameras.push_back(NewCamera);
	g_CurrentCamera = NewCamera;
	if((g_UpAxis == FixedAxis::PositiveZ) && (g_ForwardAxis == FixedAxis::PositiveX))
	{
		NewCamera->SetOrientation(Quaternion::CreateAsRotationX(M_PI_2).RotateY(M_PI_2));
		NewCamera->SetPosition(Vector3f::CreateTranslationX(4.0f));
	}
	g_Snapping = true;
	g_SnapFactor = 0.1;
}

void ProcessEvents(void)
{
	XEvent Event;
	
	while(XPending(g_Display) > 0)
	{
		XNextEvent(g_Display, &Event);
		switch(Event.type)
		{
		case ConfigureNotify:
			{
				if((Event.xconfigure.width != g_Width) || (Event.xconfigure.height != g_Height))
				{
					g_Width = Event.xconfigure.width;
					g_Height = Event.xconfigure.height;
					Resize();
				}
				
				break;
			}
		case MotionNotify:
			{
				MouseMotion(Vector2f(static_cast< float >(Event.xbutton.x), static_cast< float >(Event.xbutton.y)));
				
				break;
			}
		case ButtonPress:
		case ButtonRelease:
			{
				MouseButton Button(MouseButton::Undefined);
				
				switch(Event.xbutton.button)
				{
				case 1:
					{
						Button = MouseButton::Left;
						
						break;
					}
				case 2:
					{
						Button = MouseButton::Middle;
						
						break;
					}
				case 3:
					{
						Button = MouseButton::Right;
						
						break;
					}
				case 4:
					{
						Button = MouseButton::WheelUp;
						
						break;
					}
				case 5:
					{
						Button = MouseButton::WheelDown;
						
						break;
					}
				default:
					{
						assert(false);
					}
				}
				MouseButtonEvent(Button, Event.xbutton.type == ButtonPress, Vector2f(static_cast< float >(Event.xbutton.x), static_cast< float >(Event.xbutton.y)));
				
				break;
			}
		case KeyPress:
		case KeyRelease:
			{
				KeyEvent(Event.xkey.keycode, Event.xkey.type == KeyPress);
				
				break;
			}
		}
	}
}

void DestroyWindow(void)
{
	XAutoRepeatOn(g_Display);
    if(g_GLXContext != 0)
    {
        if(glXMakeCurrent(g_Display, None, 0) == 0)
        {
            std::cerr << "Could not release GLX context." << std::endl;
        }
        glXDestroyContext(g_Display, g_GLXContext);
        g_GLXContext = 0;
    }
	XDestroyWindow(g_Display, g_Window);
	g_Window = 0;
	XFreeColormap(g_Display, g_ColorMap);
	g_ColorMap = 0;
    XCloseDisplay(g_Display);
	g_Display = 0;
}

int main(int argc, char ** argv)
{
	// <static-initialization>
	g_UpAxis = FixedAxis::PositiveZ;
	g_ForwardAxis = FixedAxis::PositiveX;
	g_ModelerView = ModelerView::Model;
	// </static-initialization>
	// setup the random number generator for everyday use
	srand(time(0));
	ON_DEBUG(std::cout << "Setting up the window." << std::endl);
	CreateWindow();
	ON_DEBUG(std::cout << "Initializing OpenGL." << std::endl);
	InitializeOpenGL();
	Resize();
	for(auto Argument : std::vector< std::string >(argv + 1, argv + argc))
	{
		if(StartsWith(Argument, "--import-mesh=") == true)
		{
			ImportMesh(Argument.substr(14));
		}
	}
	// main loop
	ON_DEBUG(std::cout << "Entering game loop." << std::endl);
	while(g_Quit == false)
	{
		ProcessEvents();
		vDisplay();
		glXSwapBuffers(g_Display, g_Window);
	}
	ON_DEBUG(std::cout << "Left game loop." << std::endl);
	ON_DEBUG(std::cout << "Destroying window." << std::endl);
	DestroyWindow();
	
	return 0;
}
