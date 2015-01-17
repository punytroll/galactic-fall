#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <algorithm>
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

#include <xml_stream/xml_stream.h>

#include "widgets/widgets.h"
#include "xmlparser.h"

// types of objects
#define LITTLEM_CAMERA 0
#define LITTLEM_LIGHT 1
#define LITTLEM_POINT 2
#define LITTLEM_TRIANGLE 3

#define CHECK_GL_ERROR() vCheckGLError(__FILE__, __LINE__)

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

std::string GetViewString(FixedView View)
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

FixedAxis g_UpAxis;
FixedAxis g_ForwardAxis;
float g_Width(800.0f);
float g_Height(800.0f);
bool g_AltActive(false);
bool g_ShiftActive(false);
bool g_ControlActive(false);

#include "point.h"
#include "triangle.h"
#include "triangle_point.h"

class MeshReader : public XMLParser
{
public:
	MeshReader(std::istream & InputStream) :
		XMLParser(InputStream),
		_InMesh(false),
		_CurrentTrianglePoint(0),
		_CurrentTriangle(0),
		_TrianglePoint(0)
	{
	}
	
	virtual ~MeshReader(void)
	{
	}
	
	virtual void elementStart(const std::string & ElementName, const std::map< std::string, std::string > & Attributes)
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
			if(_CurrentTrianglePoint == 0)
			{
				Point * CurrentPoint(new Point(ConvertToFloat(Attributes.find("position-x")->second), ConvertToFloat(Attributes.find("position-y")->second), ConvertToFloat(Attributes.find("position-z")->second)));
				std::map< std::string, std::string >::const_iterator NameAttribute(Attributes.find("name"));
				
				if(NameAttribute != Attributes.end())
				{
					CurrentPoint->vSetName(NameAttribute->second);
				}
				_Points[ConvertToUnsigedLong(Attributes.find("identifier")->second)] = CurrentPoint;
			}
			else
			{
				_CurrentTrianglePoint->m_pPoint = _Points[ConvertToUnsigedLong(Attributes.find("point-identifier")->second)];
				_CurrentTrianglePoint->m_pPoint->m_TrianglePoints.push_back(_CurrentTrianglePoint);
			}
		}
		else if(ElementName == "triangle-point")
		{
			if(_CurrentTriangle == 0)
			{
				assert(_CurrentTrianglePoint == 0);
				_CurrentTrianglePoint = new TrianglePoint();
				_TrianglePoints[ConvertToUnsigedLong(Attributes.find("identifier")->second)] = _CurrentTrianglePoint;
				_CurrentTrianglePoint->m_Normal.Set(ConvertToFloat(Attributes.find("normal-x")->second), ConvertToFloat(Attributes.find("normal-y")->second), ConvertToFloat(Attributes.find("normal-z")->second));
			}
			else
			{
				_CurrentTriangle->vSetTrianglePoint(_TrianglePoint + 1, _TrianglePoints[ConvertToUnsigedLong(Attributes.find("triangle-point-identifier")->second)]);
				_CurrentTriangle->pGetTrianglePoint(_TrianglePoint + 1)->m_Triangles.push_back(_CurrentTriangle);
				++_TrianglePoint;
			}
		}
		else if(ElementName == "triangle")
		{
			assert(_CurrentTriangle == 0);
			_CurrentTriangle = new Triangle();
		
			std::map< std::string, std::string >::const_iterator NameAttribute(Attributes.find("name"));
			
			if(NameAttribute != Attributes.end())
			{
				_CurrentTriangle->vSetName(NameAttribute->second);
			}
			_Triangles[ConvertToUnsigedLong(Attributes.find("identifier")->second)] = _CurrentTriangle;
		}
	}
	
	virtual void elementEnd(const std::string & ElementName)
	{
		if(ElementName == "mesh")
		{
			assert(_InMesh == true);
			_InMesh = false;
		}
		else if(ElementName == "triangle")
		{
			_CurrentTriangle = 0;
			_TrianglePoint = 0;
		}
		else if(ElementName == "triangle-point")
		{
			_CurrentTrianglePoint = 0;
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
	
	std::vector< TrianglePoint * > GetTrianglePoints(void)
	{
		std::vector< TrianglePoint * > Result;
		std::map< unsigned long, TrianglePoint * >::iterator TrianglePoint(_TrianglePoints.begin());
		
		while(TrianglePoint != _TrianglePoints.end())
		{
			Result.push_back(TrianglePoint->second);
			++TrianglePoint;
		}
		
		return Result;
	}
protected:
	unsigned long ConvertToUnsigedLong(const std::string & Value)
	{
		std::istringstream InputStringStream(Value);
		unsigned long Result;
		
		InputStringStream >> Result;
		
		return Result;
	}
	
	float ConvertToFloat(const std::string & Value)
	{
		std::istringstream InputStringStream(Value);
		float Result;
		
		InputStringStream >> Result;
		
		return Result;
	}
	
	bool ConvertToBool(const std::string & Value)
	{
		return Value == "true";
	}
private:
	bool _InMesh;
	TrianglePoint * _CurrentTrianglePoint;
	Triangle * _CurrentTriangle;
	int _TrianglePoint;
	std::map< unsigned long, Point * > _Points;
	std::map< unsigned long, Triangle * > _Triangles;
	std::map< unsigned long, TrianglePoint * > _TrianglePoints;
};

class Light;

void vEnableLight(Light * pLight);
void vDisableLight(Light * pLight);

class Light : public Position
{
public:
	Light(void) :
		m_LightNumber(0)
	{
	}
	
	void vSetDiffuseColor(float Red, float Green, float Blue, float Alpha = 0.0f)
	{
		m_DiffuseColor[0] = Red;
		m_DiffuseColor[1] = Green;
		m_DiffuseColor[2] = Blue;
		m_DiffuseColor[3] = Alpha;
		if(m_LightNumber != 0)
		{
			vSetDiffuseColor();
		}
	}
	
	void vSetDiffuseColor(void)
	{
		glLightfv(m_LightNumber, GL_DIFFUSE, m_DiffuseColor.GetPointer());
	}
	
	bool bIsEnabled(void) const
	{
		return m_LightNumber != 0;
	}
	
	void vEnable(void)
	{
		if(bIsEnabled() == false)
		{
			vEnableLight(this);
		}
	}
	
	void vDisable(void)
	{
		if(bIsEnabled() == true)
		{
			vDisableLight(this);
		}
	}
	
	Vector4f m_DiffuseColor;
	GLenum m_LightNumber;
};

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
		m_DiffuseColor(pLight->m_DiffuseColor),
		m_bEnabled(pLight->bIsEnabled())
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
	
	virtual void elementStart(const std::string & sElementName, const std::map< std::string, std::string > & Attributes)
	{
		if(sElementName == "light")
		{
			LightDescription LightDescription;
			
			LightDescription.m_Position = Vector3f::CreateFromComponents(ConvertToFloat(Attributes.find("position-x")->second), ConvertToFloat(Attributes.find("position-y")->second), ConvertToFloat(Attributes.find("position-z")->second));
			LightDescription.m_DiffuseColor = Vector4f(ConvertToFloat(Attributes.find("color-red")->second), ConvertToFloat(Attributes.find("color-green")->second), ConvertToFloat(Attributes.find("color-blue")->second), ConvertToFloat(Attributes.find("color-alpha")->second));
			LightDescription.m_bEnabled = ConvertToBool(Attributes.find("enabled")->second);
			
			m_LightDescriptions.push_back(LightDescription);
		}
		else if(sElementName == "camera")
		{
			CameraDescription CameraDescription;
			
			CameraDescription.Position = Vector3f::CreateFromComponents(ConvertToFloat(Attributes.find("position-x")->second), ConvertToFloat(Attributes.find("position-y")->second), ConvertToFloat(Attributes.find("position-z")->second));
			CameraDescription.Orientation = Quaternion(ConvertToFloat(Attributes.find("orientation-w")->second), ConvertToFloat(Attributes.find("orientation-x")->second), ConvertToFloat(Attributes.find("orientation-y")->second), ConvertToFloat(Attributes.find("orientation-z")->second));
			CameraDescription.FieldOfViewY = ConvertToFloat(Attributes.find("field-of-view")->second);
			m_CameraDescriptions.push_back(CameraDescription);
		}
		else
		{
			MeshReader::elementStart(sElementName, Attributes);
		}
	}
	
	virtual void elementEnd(const std::string & sElementName)
	{
		MeshReader::elementEnd(sElementName);
	}
	
	const std::vector< CameraDescription > & GetCameraDescriptions(void) const
	{
		return m_CameraDescriptions;
	}
	
	const std::vector< LightDescription > & GetLightDescriptions(void) const
	{
		return m_LightDescriptions;
	}
private:
	std::vector< CameraDescription > m_CameraDescriptions;
	std::vector< LightDescription > m_LightDescriptions;
};

Display * g_Display;
GLXContext g_GLXContext;
Window g_Window;
Colormap g_ColorMap;
std::vector< Point * > g_Points;
std::vector< TrianglePoint * > g_TrianglePoints;
std::vector< Triangle * > g_Triangles;
std::vector< Point * > g_SelectedPoints;
std::vector< Triangle * > g_SelectedTriangles;
Point * g_pHoveredPoint = 0;
Triangle * g_pHoveredTriangle = 0;
Light * g_pHoveredLight = 0;
Light * g_pSelectedLight = 0;
std::vector< Light * > g_Lights;
std::vector< Light * > g_EnabledLights;
std::vector< Camera * > g_Cameras;
Camera * g_pHoveredCamera = 0;
Camera * g_SelectedCamera = 0;
Camera * g_CurrentCamera = 0;
int g_iLastMotionX = -1;
int g_iLastMotionY = -1;
bool g_bMoved = false;
bool g_Quit(false);
GLuint g_puiSelectionBuffer[1024];
bool g_Snapping = false;
float g_SnapFactor = 0.1;
MouseButton g_MouseButton(MouseButton::Undefined);
std::deque< int > g_FreeLights;
UserInterface g_UserInterface;
Vector2f g_MousePosition;

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
	glTranslatef((g_Width - 2.0f * MousePosition[0]) / 5.0f, (g_Height - 2.0f * (g_Height - MousePosition[1])) / 5.0f, 0.0f);
	glScalef(g_Width / 5.0f, g_Height / 5.0f, 1.0);
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
	
	int iHits = glRenderMode(GL_RENDER);
	GLuint uiNearestDepth = 0xFFFFFFFF;
	GLuint uiNearestObject = 0;
	GLuint uiNearestObjectType = 0;
	GLuint * puiHitPointer = g_puiSelectionBuffer;
	bool bHover = false;
	
	g_pHoveredPoint = 0;
	g_pHoveredTriangle = 0;
	g_pHoveredLight = 0;
	g_pHoveredCamera = 0;
	for(int iHit = 1; iHit <= iHits; iHit++)
	{
		//~ std::cout << "Hit " << iHit << " is the " << ((puiHitPointer[3] == GL_POINTS) ? ("Point") : ("Triangle")) << ' ' << puiHitPointer[4] << " with minimum depth " << puiHitPointer[1] << std::endl;
		if((puiHitPointer[1] < uiNearestDepth) || ((uiNearestObjectType == LITTLEM_TRIANGLE) && ((puiHitPointer[3] == LITTLEM_POINT) || (puiHitPointer[3] == LITTLEM_LIGHT))))
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
				g_pHoveredCamera = g_Cameras[uiNearestObject];
				
				break;
			}
		case LITTLEM_LIGHT:
			{
				g_pHoveredLight = g_Lights[uiNearestObject];
				
				break;
			}
		case LITTLEM_POINT:
			{
				g_pHoveredPoint = g_Points[uiNearestObject];
				
				break;
			}
		case LITTLEM_TRIANGLE:
			{
				g_pHoveredTriangle = g_Triangles[uiNearestObject];
				
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

void vDrawBoxAt(float fX, float fY, float fWidth, float fHeight)
{
	glBegin(GL_QUADS);
	glVertex2f(fX, fY);
	glVertex2f(fX, fY + fHeight);
	glVertex2f(fX + fWidth, fY + fHeight);
	glVertex2f(fX + fWidth, fY);
	glEnd();
}

void vDrawBoxAt(Vector2f & Position, Vector2f & Size)
{
	vDrawBoxAt(Position[0], Position[1], Size[0], Size[1]);
}

void vDrawTextAt(GLfloat fPositionX, GLfloat fPositionY, const std::string & sString)
{
	glPushMatrix();
	glTranslatef(fPositionX, fPositionY, 0.0f);
	vDrawText(sString);
	glPopMatrix();
}

void vEnableLight(Light * pLight)
{
	if(g_FreeLights.size() > 0)
	{
		int iLightNumber(g_FreeLights.front());
		
		g_FreeLights.pop_front();
		g_EnabledLights.push_back(pLight);
		pLight->m_LightNumber = GL_LIGHT0 + iLightNumber;
		glEnable(pLight->m_LightNumber);
		pLight->vSetDiffuseColor();
	}
}

void vDisableLight(Light * pLight)
{
	std::vector< Light * >::iterator iLight(std::find(g_EnabledLights.begin(), g_EnabledLights.end(), pLight));
	
	g_EnabledLights.erase(iLight);
	glDisable(pLight->m_LightNumber);
	g_FreeLights.push_back(pLight->m_LightNumber - GL_LIGHT0);
	pLight->m_LightNumber = 0;
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
	
	std::stringstream ssStatusText;
	
	ssStatusText << "#Points: " << g_Points.size() << "   #TrianglePoints: " << g_TrianglePoints.size() << "   #Triangles: " << g_Triangles.size() << "   #Selected Points: " << g_SelectedPoints.size() << "   Width: " << g_Width << "   Height: " << g_Height << "   Front Face: ";
	
	GLint iFrontFace;
	
	glGetIntegerv(GL_FRONT_FACE, &iFrontFace);
	if(iFrontFace == GL_CCW)
	{
		ssStatusText << "Counter Clock Wise";
	}
	else
	{
		ssStatusText << "Clock Wise";
	}
	vDrawTextAt(0, 0, ssStatusText.str());
	
	std::stringstream ssSnapInformationText;
	
	ssSnapInformationText << "Snapping: ";
	if(g_Snapping == true)
	{
		ssSnapInformationText << "on   SnapFactor: " << g_SnapFactor;
	}
	else
	{
		ssSnapInformationText << "off";
	}
	vDrawTextAt(0, g_Height - 24, ssSnapInformationText.str());
	if(g_SelectedPoints.size() > 0)
	{
		std::stringstream ssPointInformationText;
		
		ssPointInformationText << "Point:  X: " << g_SelectedPoints.front()->GetX() << "   Y: " << g_SelectedPoints.front()->GetY() << "   Z: " << g_SelectedPoints.front()->GetZ() << "   Name: \"" << g_SelectedPoints.front()->sGetName() << "\"   #TrianglePoints: " << g_SelectedPoints.front()->m_TrianglePoints.size();
		vDrawTextAt(0, g_Height - 36, ssPointInformationText.str());
	}
	if(g_SelectedTriangles.size() > 0)
	{
		std::stringstream ssTriangleInformationText;
		
		ssTriangleInformationText << "Triangle:  Name: \"" << g_SelectedTriangles.front()->sGetName() << '"';
		vDrawTextAt(0, g_Height - 48, ssTriangleInformationText.str());
	}
	if(g_SelectedCamera != nullptr)
	{
		std::stringstream ssCameraInformationText;
		
		ssCameraInformationText << "Camera:  X: " << g_SelectedCamera->GetPosition()[0] << "   Y: " << g_SelectedCamera->GetPosition()[1] << "   Z: " << g_SelectedCamera->GetPosition()[2];
		vDrawTextAt(0, g_Height - 60, ssCameraInformationText.str());
	}
	g_UserInterface.vDraw();
	glFlush();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
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
		g_Triangles[stI]->vDrawSelection();
		glEnd();
	}
	glPopName();
	glLoadName(LITTLEM_POINT);
	// dummy entry so that we can call glLoadName in the loop below: it will be replace immediately
	glPushName(0xFFFFFFFF);
	for(std::vector< Point * >::size_type stI = 0; stI < g_Points.size(); ++stI)
	{
		glLoadName(stI);
		glBegin(GL_POINTS);
		g_Points[stI]->DrawSelection();
		glEnd();
	}
	glPopName();
	glLoadName(LITTLEM_LIGHT);
	// dummy entry so that we can call glLoadName in the loop below: it will be replace immediately
	glPushName(0xFFFFFFFF);
	for(std::vector< Light * >::size_type stLight = 0; stLight < g_Lights.size(); ++stLight)
	{
		glLoadName(stLight);
		glBegin(GL_POINTS);
		g_Lights[stLight]->DrawSelection();
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
	assert(g_CurrentCamera != 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(g_CurrentCamera->GetSpacialMatrix().Inverted().GetPointer());
	for(std::vector< Light * >::size_type stLight = 0; stLight < g_EnabledLights.size(); ++stLight)
	{
		Vector4f LightPosition(g_EnabledLights[stLight]->GetX(), g_EnabledLights[stLight]->GetY(), g_EnabledLights[stLight]->GetZ(), 1.0f);
		
		glLightfv(g_EnabledLights[stLight]->m_LightNumber, GL_POSITION, LightPosition.GetPointer());
	}
	glBegin(GL_TRIANGLES);
	glColor3f(0.3f, 0.3f, 0.5f);
	for(std::vector< Triangle * >::size_type stI = 0; stI < g_Triangles.size(); ++stI)
	{
		g_Triangles[stI]->vDraw();
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
	for(std::vector< Point * >::size_type stI = 0; stI < g_Points.size(); ++stI)
	{
		g_Points[stI]->Draw();
	}
	glEnd();
	// draw all lights with deactivated lighting
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(std::vector< Light * >::size_type stLight = 0; stLight < g_Lights.size(); ++stLight)
	{
		g_Lights[stLight]->Draw();
	}
	glEnd();
	// draw all cameras with deactivated lighting
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(std::vector< Camera * >::size_type stCamera = 0; stCamera < g_Cameras.size(); ++stCamera)
	{
		glVertex3fv(g_Cameras[stCamera]->GetPosition().GetPointer());
	}
	glEnd();
	// now draw selected stuff
	if(g_SelectedTriangles.size() > 0)
	{
		for(std::vector< Triangle * >::size_type stTriangle = 0; stTriangle < g_SelectedTriangles.size(); ++stTriangle)
		{
			glColor3f(0.5f, 0.8f, 0.2f);
			glBegin(GL_TRIANGLES);
			g_SelectedTriangles[stTriangle]->vDraw();
			glEnd();
			glColor3f(0.6f, 0.6f, 0.0f);
			glPushMatrix();
			
			Vector3f TriangleCenter(g_SelectedTriangles[stTriangle]->GetTriangleCenter());
			Vector3f TriangleNormal(g_SelectedTriangles[stTriangle]->GetTriangleNormal());
			
			glTranslatef(TriangleCenter[0], TriangleCenter[1], TriangleCenter[2]);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(TriangleNormal.GetPointer());
			glEnd();
			glPopMatrix();
			glColor3f(0.4f, 0.1f, 0.0f);
			glPushMatrix();
			glTranslatef(g_SelectedTriangles[stTriangle]->pGetPoint(1)->GetX(), g_SelectedTriangles[stTriangle]->pGetPoint(1)->GetY(), g_SelectedTriangles[stTriangle]->pGetPoint(1)->GetZ());
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(g_SelectedTriangles[stTriangle]->pGetTrianglePoint(1)->m_Normal.GetPointer());
			glEnd();
			glPopMatrix();
			glPushMatrix();
			glTranslatef(g_SelectedTriangles[stTriangle]->pGetPoint(2)->GetX(), g_SelectedTriangles[stTriangle]->pGetPoint(2)->GetY(), g_SelectedTriangles[stTriangle]->pGetPoint(2)->GetZ());
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(g_SelectedTriangles[stTriangle]->pGetTrianglePoint(2)->m_Normal.GetPointer());
			glEnd();
			glPopMatrix();
			glPushMatrix();
			glTranslatef(g_SelectedTriangles[stTriangle]->pGetPoint(3)->GetX(), g_SelectedTriangles[stTriangle]->pGetPoint(3)->GetY(), g_SelectedTriangles[stTriangle]->pGetPoint(3)->GetZ());
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(g_SelectedTriangles[stTriangle]->pGetTrianglePoint(3)->m_Normal.GetPointer());
			glEnd();
			glPopMatrix();
		}
	}
	if(g_SelectedPoints.size() > 0)
	{
		glDisable(GL_DEPTH_TEST);
		
		auto Matrix(Matrix4f::CreateRotation(g_CurrentCamera->GetOrientation()));
		
		for(auto Point : g_SelectedPoints)
		{
			glPushMatrix();
			glTranslatef(Point->GetX(), Point->GetY(), Point->GetZ());
			glMultMatrixf(Matrix.GetPointer());
			glBegin(GL_POINTS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-0.05f, -0.05f, 0.0f);
			glVertex3f(0.05f, -0.05f, 0.0f);
			glVertex3f(0.05f, 0.05f, 0.0f);
			glVertex3f(-0.05f, 0.05f, 0.0f);
			glEnd();
			glPopMatrix();
		}
		glEnable(GL_DEPTH_TEST);
	}
	if(g_pSelectedLight != 0)
	{
		glPushMatrix();
		glTranslatef(g_pSelectedLight->GetX(), g_pSelectedLight->GetY(), g_pSelectedLight->GetZ());
		glMultMatrixf(Matrix4f::CreateRotation(g_CurrentCamera->GetOrientation()).GetPointer());
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, -0.05f, 0.0f);
		glVertex3f(0.05f, -0.05f, 0.0f);
		glVertex3f(0.05f, 0.05f, 0.0f);
		glVertex3f(-0.05f, 0.05f, 0.0f);
		glEnd();
		glPopMatrix();
	}
	if(g_SelectedCamera != 0)
	{
		glPushMatrix();
		glTranslatef(g_SelectedCamera->GetPosition()[0], g_SelectedCamera->GetPosition()[1], g_SelectedCamera->GetPosition()[2]);
		glPushMatrix();
		glMultMatrixf(Matrix4f::CreateRotation(g_CurrentCamera->GetOrientation()).GetPointer());
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, -0.05f, 0.0f);
		glVertex3f(0.05f, -0.05f, 0.0f);
		glVertex3f(0.05f, 0.05f, 0.0f);
		glVertex3f(-0.05f, 0.05f, 0.0f);
		glEnd();
		glPopMatrix();
		glMultMatrixf(Matrix4f::CreateRotation(g_SelectedCamera->GetOrientation()).GetPointer());
		glColor3f(0.2f, 0.8f, 0.5f);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -1.0f); // the default camera is looking along the negative z axis
		glEnd();
		glPopMatrix();
	}
	if(g_pHoveredPoint != 0)
	{
		glColor3f(0.5f, 0.5f, 1.0f);
		glBegin(GL_POINTS);
		g_pHoveredPoint->Draw();
		glEnd();
	}
	else if(g_pHoveredTriangle != 0)
	{
		glColor3f(1.0f, 0.7f, 0.3f);
		glBegin(GL_TRIANGLES);
		g_pHoveredTriangle->vDraw();
		glEnd();
		glColor3f(0.3f, 0.1f, 0.3f);
		glPushMatrix();
		
		Vector3f TriangleCenter(g_pHoveredTriangle->GetTriangleCenter());
		Vector3f TriangleNormal(g_pHoveredTriangle->GetTriangleNormal());
		
		glTranslatef(TriangleCenter[0], TriangleCenter[1], TriangleCenter[2]);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(TriangleNormal.GetPointer());
		glEnd();
		glPopMatrix();
		glColor3f(0.0f, 0.1f, 0.4f);
		glPushMatrix();
		glTranslatef(g_pHoveredTriangle->pGetPoint(1)->GetX(), g_pHoveredTriangle->pGetPoint(1)->GetY(), g_pHoveredTriangle->pGetPoint(1)->GetZ());
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(g_pHoveredTriangle->pGetTrianglePoint(1)->m_Normal.GetPointer());
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(g_pHoveredTriangle->pGetPoint(2)->GetX(), g_pHoveredTriangle->pGetPoint(2)->GetY(), g_pHoveredTriangle->pGetPoint(2)->GetZ());
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(g_pHoveredTriangle->pGetTrianglePoint(2)->m_Normal.GetPointer());
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(g_pHoveredTriangle->pGetPoint(3)->GetX(), g_pHoveredTriangle->pGetPoint(3)->GetY(), g_pHoveredTriangle->pGetPoint(3)->GetZ());
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(g_pHoveredTriangle->pGetTrianglePoint(3)->m_Normal.GetPointer());
		glEnd();
		glPopMatrix();
	}
	else if(g_pHoveredLight != 0)
	{
		glColor3f(1.0f, 0.5f, 0.5f);
		glBegin(GL_POINTS);
		g_pHoveredLight->Draw();
		glEnd();
	}
	else if(g_pHoveredCamera != 0)
	{
		glColor3f(0.0f, 1.0f, 0.5f);
		glBegin(GL_POINTS);
		glVertex3fv(g_pHoveredCamera->GetPosition().GetPointer());
		glEnd();
		glPushMatrix();
		glTranslatef(g_pHoveredCamera->GetPosition()[0], g_pHoveredCamera->GetPosition()[1], g_pHoveredCamera->GetPosition()[2]);
		glMultMatrixf(Matrix4f::CreateRotation(g_pHoveredCamera->GetOrientation()).GetPointer());
		glColor3f(0.0f, 0.5f, 0.5f);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -1.0f); // the default camera is looking along the negative z axis
		glEnd();
		glPopMatrix();
	}
	if(bLighting == GL_TRUE)
	{
		glEnable(GL_LIGHTING);
	}
}

void vDisplayModelView(void)
{
	if(g_CurrentCamera != 0)
	{
		vPerformPicking();
		vDisplayModel();
	}
}

void vDisplayUserInterface(void)
{
	GLboolean bLighting = glIsEnabled(GL_LIGHTING);
	
	if(bLighting == GL_TRUE)
	{
		glDisable(GL_LIGHTING);
	}
	glDisable(GL_DEPTH_TEST);
	vDisplayTexts();
	glEnable(GL_DEPTH_TEST);
	if(bLighting == GL_TRUE)
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

void vDeleteTrianglePoint(TrianglePoint * pTrianglePoint)
{
	pTrianglePoint->m_pPoint->m_TrianglePoints.erase(std::find(pTrianglePoint->m_pPoint->m_TrianglePoints.begin(), pTrianglePoint->m_pPoint->m_TrianglePoints.end(), pTrianglePoint));
	g_TrianglePoints.erase(std::find(g_TrianglePoints.begin(), g_TrianglePoints.end(), pTrianglePoint));
	delete pTrianglePoint;
}

void vDetachTrianglePoint(Triangle * pTriangle, int iTrianglePointSlot)
{
	TrianglePoint * pTrianglePoint(pTriangle->pGetTrianglePoint(iTrianglePointSlot));
	
	assert(pTrianglePoint != 0);
	pTriangle->vSetTrianglePoint(iTrianglePointSlot, 0);
	pTrianglePoint->m_Triangles.erase(std::find(pTrianglePoint->m_Triangles.begin(), pTrianglePoint->m_Triangles.end(), pTriangle));
	/// automatically remove unreferenced TrianglePoints from the model
	if(pTrianglePoint->m_Triangles.size() == 0)
	{
		vDeleteTrianglePoint(pTrianglePoint);
	}
}

/**
 * @brief Attaches a triangle point to a triangle on a certain slot.
 * 
 * This function will detach any triangle point that might have been attached to the slot prior to attaching the new one. This is done using vDetachTrianglePoint().
 * This function will also add the triangle to the triangle point's triangle list.
 **/
void vAttachTrianglePoint(Triangle * pTriangle, int iTrianglePointSlot, TrianglePoint * pNewTrianglePoint)
{
	if(pTriangle->pGetTrianglePoint(iTrianglePointSlot) != 0)
	{
		vDetachTrianglePoint(pTriangle, iTrianglePointSlot);
	}
	pNewTrianglePoint->m_Triangles.push_back(pTriangle);
	pTriangle->vSetTrianglePoint(iTrianglePointSlot, pNewTrianglePoint);
}

void vExchangeTrianglePoint(Triangle * pTriangle, TrianglePoint * pOldTrianglePoint, TrianglePoint * pNewTrianglePoint)
{
	if(pOldTrianglePoint == pTriangle->pGetTrianglePoint(1))
	{
		vDetachTrianglePoint(pTriangle, 1);
		vAttachTrianglePoint(pTriangle, 1, pNewTrianglePoint);
	}
	else if(pOldTrianglePoint == pTriangle->pGetTrianglePoint(2))
	{
		vDetachTrianglePoint(pTriangle, 2);
		vAttachTrianglePoint(pTriangle, 2, pNewTrianglePoint);
	}
	else if(pOldTrianglePoint == pTriangle->pGetTrianglePoint(3))
	{
		vDetachTrianglePoint(pTriangle, 3);
		vAttachTrianglePoint(pTriangle, 3, pNewTrianglePoint);
	}
}

void vDeleteTriangle(std::vector< Triangle * >::iterator iTriangle)
{
	assert(iTriangle != g_Triangles.end());
	
	Triangle * pTriangle(*iTriangle);
	
	if(pTriangle == g_pHoveredTriangle)
	{
		g_pHoveredTriangle = 0;
	}
	
	std::vector< Triangle * >::iterator iSelectedTriangle(std::find(g_SelectedTriangles.begin(), g_SelectedTriangles.end(), pTriangle));
	
	if(iSelectedTriangle != g_SelectedTriangles.end())
	{
		g_SelectedTriangles.erase(iSelectedTriangle);
	}
	g_Triangles.erase(iTriangle);
	vDetachTrianglePoint(pTriangle, 1);
	vDetachTrianglePoint(pTriangle, 2);
	vDetachTrianglePoint(pTriangle, 3);
	delete pTriangle;
}

void vDeleteTriangle(std::vector< Triangle * >::size_type stTriangle)
{
	assert(stTriangle < g_Triangles.size());
	vDeleteTriangle(g_Triangles.begin() + stTriangle);
}

void vDeleteTriangle(Triangle * pTriangle)
{
	assert(pTriangle != 0);
	vDeleteTriangle(std::find(g_Triangles.begin(), g_Triangles.end(), pTriangle));
}

void vDeleteCamera(std::vector< Camera * >::iterator iCamera)
{
	assert(iCamera != g_Cameras.end());
	
	Camera * pCamera(*iCamera);
	
	if(pCamera == g_pHoveredCamera)
	{
		g_pHoveredCamera = 0;
	}
	if(pCamera == g_CurrentCamera)
	{
		g_CurrentCamera = 0;
	}
	g_Cameras.erase(iCamera);
}

void vDeleteLight(std::vector< Light * >::iterator iLight)
{
	assert(iLight != g_Lights.end());
	
	Light * pLight(*iLight);
	
	if(pLight == g_pHoveredLight)
	{
		g_pHoveredLight = 0;
	}
	if(pLight == g_pSelectedLight)
	{
		g_pSelectedLight = 0;
	}
	pLight->vDisable();
	g_Lights.erase(iLight);
}

void vDeleteLight(Light * pLight)
{
	assert(pLight != 0);
	vDeleteLight(std::find(g_Lights.begin(), g_Lights.end(), pLight));
}

void vDeletePoint(std::vector< Point * >::iterator iPoint)
{
	assert(iPoint != g_Points.end());
	
	Point * pPoint(*iPoint);
	
	while(pPoint->m_TrianglePoints.size() > 0)
	{
		while(pPoint->m_TrianglePoints.front()->m_Triangles.size() > 0)
		{
			vDeleteTriangle(pPoint->m_TrianglePoints.front()->m_Triangles.front());
		}
	}
	if(g_pHoveredPoint == pPoint)
	{
		g_pHoveredPoint = 0;
	}
	
	std::vector< Point * >::iterator iSelectedPoint(std::find(g_SelectedPoints.begin(), g_SelectedPoints.end(), pPoint));
	
	if(iSelectedPoint != g_SelectedPoints.end())
	{
		g_SelectedPoints.erase(iSelectedPoint);
	}
	g_Points.erase(iPoint);
	delete pPoint;
}

void vDeletePoint(Point * pPoint)
{
	assert(pPoint != 0);
	vDeletePoint(std::find(g_Points.begin(), g_Points.end(), pPoint));
}

void vClearScene(void)
{
	while(g_Lights.size() > 0)
	{
		vDeleteLight(g_Lights.begin());
	}
	while(g_Triangles.size() > 0)
	{
		vDeleteTriangle(g_Triangles.begin());
	}
	while(g_Points.size() > 0)
	{
		vDeletePoint(g_Points.begin());
	}
	while(g_Cameras.size() > 0)
	{
		vDeleteCamera(g_Cameras.begin());
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
	CHECK_GL_ERROR();
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

void vAdjustFloatValue(float & fValue, float fDelta)
{
	if(g_Snapping == true)
	{
		if(fDelta < 0)
		{
			if(fDelta > -g_SnapFactor)
			{
				fDelta = -g_SnapFactor;
			}
		}
		else
		{
			if(fDelta < g_SnapFactor)
			{
				fDelta = g_SnapFactor;
			}
		}
	}
	fValue += fDelta;
	if(g_Snapping == true)
	{
		fValue = roundf(fValue / g_SnapFactor) * g_SnapFactor;
	}
}

float GetAdjustedFloatValue(float Value, float Delta)
{
	if(g_Snapping == true)
	{
		if(Delta < 0)
		{
			if(Delta > -g_SnapFactor)
			{
				Delta = -g_SnapFactor;
			}
		}
		else
		{
			if(Delta < g_SnapFactor)
			{
				Delta = g_SnapFactor;
			}
		}
	}
	
	float Result(Value + Delta);
	
	if(g_Snapping == true)
	{
		Result = roundf(Result / g_SnapFactor) * g_SnapFactor;
	}
	
	return Result;
}

void MovePosition(Position * Position, int Axis, float Delta)
{
	if(Axis == 0)
	{
		Position->SetX(GetAdjustedFloatValue(Position->GetX(), Delta));
	}
	else if(Axis == 1)
	{
		Position->SetY(GetAdjustedFloatValue(Position->GetY(), Delta));
	}
	else if(Axis == 2)
	{
		Position->SetZ(GetAdjustedFloatValue(Position->GetZ(), Delta));
	}
}

Vector3f GetAdjustedStepInView(const Vector3f & Position, FixedDirection Direction)
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
		Result[0] = roundf(Result[0] / g_SnapFactor) * g_SnapFactor;
		Result[1] = roundf(Result[1] / g_SnapFactor) * g_SnapFactor;
		Result[2] = roundf(Result[2] / g_SnapFactor) * g_SnapFactor;
	}
	
	return Result;
}

class PointsView : public KeyAcceptor
{
public:
	virtual bool AcceptKey(int KeyCode, bool IsDown) override
	{
		bool bKeyAccepted(false);
		
		switch(KeyCode)
		{
		case 65: // SPACE
			{
				if(IsDown == true)
				{
					// create triangle from three selected points
					if(g_SelectedPoints.size() == 3)
					{
						TrianglePoint * pTrianglePoint1(new TrianglePoint(g_SelectedPoints[0]));
						
						g_TrianglePoints.push_back(pTrianglePoint1);
						g_SelectedPoints[0]->m_TrianglePoints.push_back(pTrianglePoint1);
						
						TrianglePoint * pTrianglePoint2(new TrianglePoint(g_SelectedPoints[1]));
						
						g_TrianglePoints.push_back(pTrianglePoint2);
						g_SelectedPoints[1]->m_TrianglePoints.push_back(pTrianglePoint2);
						
						TrianglePoint * pTrianglePoint3(new TrianglePoint(g_SelectedPoints[2]));
						
						g_TrianglePoints.push_back(pTrianglePoint3);
						g_SelectedPoints[2]->m_TrianglePoints.push_back(pTrianglePoint3);
						
						Triangle * pNewTriangle(new Triangle());
						
						vAttachTrianglePoint(pNewTriangle, 1, pTrianglePoint1);
						vAttachTrianglePoint(pNewTriangle, 2, pTrianglePoint2);
						vAttachTrianglePoint(pNewTriangle, 3, pTrianglePoint3);
						pNewTriangle->vRealignNormal();
						g_Triangles.push_back(pNewTriangle);
						g_SelectedTriangles.push_back(pNewTriangle);
						g_SelectedPoints.clear();
						bKeyAccepted = true;
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
		case 40: // D
			{
				if(IsDown == true)
				{
					// duplicate selected points
					if(g_SelectedPoints.size() > 0)
					{
						bKeyAccepted = true;
					}
					
					std::vector< Point * > NewSelectedPoints;
					
					for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
					{
						g_Points.push_back(new Point(*g_SelectedPoints[stI]));
						NewSelectedPoints.push_back(g_Points.back());
					}
					g_SelectedPoints = NewSelectedPoints;
				}
				
				break;
			}
		case 57: // N
			{
				if(IsDown == true)
				{
					if(g_ShiftActive == true)
					{
						// create new point at coordinates (0.0, 0.0, 0.0)
						g_Points.push_back(new Point(0.0f, 0.0f, 0.0f));
					}
					else
					{
						// create new point at random coordinates within (-1.0 .. 1.0, -1.0 .. 1.0, -1.0 .. 1.0)
						g_Points.push_back(new Point(-1.0f + 2 * (static_cast< double> (random()) / RAND_MAX), -1.0f + 2 * (static_cast< double> (random()) / RAND_MAX), -1.0f + 2 * (static_cast< double> (random()) / RAND_MAX)));
					}
					bKeyAccepted = true;
				}
				
				break;
			}
		case 28: // T
			{
				if(IsDown == true)
				{
					// select all triangles which contain the selected points
					if(g_SelectedPoints.size() > 0)
					{
						g_SelectedTriangles.clear();
						
						std::vector< Triangle * > TrianglesToSelect;
						
						for(std::vector< Point * >::size_type stPoint = 0; stPoint < g_SelectedPoints.size(); ++stPoint)
						{
							for(std::vector< TrianglePoint * >::size_type stTrianglePoint = 0; stTrianglePoint < g_SelectedPoints[stPoint]->m_TrianglePoints.size(); ++stTrianglePoint)
							{
								copy(g_SelectedPoints[stPoint]->m_TrianglePoints[stTrianglePoint]->m_Triangles.begin(), g_SelectedPoints[stPoint]->m_TrianglePoints[stTrianglePoint]->m_Triangles.end(), back_inserter(TrianglesToSelect));
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
				if(IsDown == true)
				{
					// delete selected points
					while(g_SelectedPoints.size() > 0)
					{
						vDeletePoint(g_SelectedPoints.front());
					}
					bKeyAccepted = true;
				}
				
				break;
			}
		case 114: // RIGHT
			{
				if(IsDown == true)
				{
					if(g_SelectedPoints.size() > 0)
					{
						for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
						{
							MovePosition(g_SelectedPoints[stI], 0, g_SnapFactor);
						}
						bKeyAccepted = true;
					}
				}
				
				break;
			}
		case 113: // LEFT
			{
				if(IsDown == true)
				{
					if(g_SelectedPoints.size() > 0)
					{
						for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
						{
							MovePosition(g_SelectedPoints[stI], 0, -g_SnapFactor);
						}
						bKeyAccepted = true;
					}
				}
				
				break;
			}
		case 111: // UP
			{
				if(IsDown == true)
				{
					if(g_SelectedPoints.size() > 0)
					{
						for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
						{
							MovePosition(g_SelectedPoints[stI], 1, g_SnapFactor);
						}
						bKeyAccepted = true;
					}
				}
				
				break;
			}
		case 116: // DOWN
			{
				if(IsDown == true)
				{
					if(g_SelectedPoints.size() > 0)
					{
						for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
						{
							MovePosition(g_SelectedPoints[stI], 1, -g_SnapFactor);
						}
						bKeyAccepted = true;
					}
				}
				
				break;
			}
		case 117: // PAGE DOWN
			{
				if(IsDown == true)
				{
					if(g_SelectedPoints.size() > 0)
					{
						for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
						{
							MovePosition(g_SelectedPoints[stI], 2, g_SnapFactor);
						}
						bKeyAccepted = true;
					}
				}
				
				break;
			}
		case 112: // PAGE UP
			{
				if(IsDown == true)
				{
					if(g_SelectedPoints.size() > 0)
					{
						for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
						{
							MovePosition(g_SelectedPoints[stI], 2, -g_SnapFactor);
						}
						bKeyAccepted = true;
					}
				}
				
				break;
			}
		}
		
		return bKeyAccepted;
	}
};

class TriangleView : public KeyAcceptor
{
public:
	virtual bool AcceptKey(int KeyCode, bool IsDown) override
	{
		bool bKeyAccepted(false);
		
		switch(KeyCode)
		{
		case 31: // I
			{
				// change triangle front and back
				if(g_SelectedTriangles.size() > 0)
				{
					for(std::vector< Triangle * >::size_type stTriangle = 0; stTriangle < g_SelectedTriangles.size(); ++stTriangle)
					{
						g_SelectedTriangles[stTriangle]->vInvert();
					}
					bKeyAccepted = true;
				}
				
				break;
			}
		case 39: // S
			{
				for(std::vector< Point * >::size_type stPoint = 0; stPoint < g_SelectedPoints.size(); ++stPoint)
				{
					for(std::vector< Triangle * >::size_type stTriangle = 0; stTriangle < g_SelectedTriangles.size(); ++stTriangle)
					{
						TrianglePoint * pOldTrianglePoint(g_SelectedTriangles[stTriangle]->pGetTrianglePoint(g_SelectedPoints[stPoint]));
						
						if(pOldTrianglePoint != 0)
						{
							TrianglePoint * pNewTrianglePoint(new TrianglePoint());
							
							pNewTrianglePoint->m_pPoint = g_SelectedPoints[stPoint];
							pNewTrianglePoint->m_Normal = g_SelectedTriangles[stTriangle]->GetTriangleNormal();
							g_TrianglePoints.push_back(pNewTrianglePoint);
							vExchangeTrianglePoint(g_SelectedTriangles[stTriangle], pOldTrianglePoint, pNewTrianglePoint);
						}
					}
				}
				bKeyAccepted = true;
				
				break;
			}
		case 28: // T
			{
				if(g_SelectedTriangles.size() > 0)
				{
					g_SelectedPoints.clear();
					
					std::vector< Point * > PointsToSelect;
					
					for(std::vector< Triangle * >::size_type stTriangle = 0; stTriangle < g_SelectedTriangles.size(); ++stTriangle)
					{
						PointsToSelect.push_back(g_SelectedTriangles[stTriangle]->pGetPoint(1));
						PointsToSelect.push_back(g_SelectedTriangles[stTriangle]->pGetPoint(2));
						PointsToSelect.push_back(g_SelectedTriangles[stTriangle]->pGetPoint(3));
					}
					std::sort(PointsToSelect.begin(), PointsToSelect.end());
					std::unique_copy(PointsToSelect.begin(), PointsToSelect.end(), back_inserter(g_SelectedPoints));
					bKeyAccepted = true;
				}
				
				break;
			}
		case 30: // U
			{
				for(std::vector< Point * >::size_type stPoint = 0; stPoint < g_SelectedPoints.size(); ++stPoint)
				{
					TrianglePoint * pNewTrianglePoint(0);
					
					for(std::vector< Triangle * >::size_type stTriangle = 0; stTriangle < g_SelectedTriangles.size(); ++stTriangle)
					{
						TrianglePoint * pTrianglePoint(g_SelectedTriangles[stTriangle]->pGetTrianglePoint(g_SelectedPoints[stPoint]));
						
						if(pTrianglePoint != 0)
						{
							if(pNewTrianglePoint == 0)
							{
								pNewTrianglePoint = new TrianglePoint(g_SelectedPoints[stPoint]);
								pNewTrianglePoint->m_Normal.Set(0.0f, 0.0f, 0.0f);
								g_TrianglePoints.push_back(pNewTrianglePoint);
								g_SelectedPoints[stPoint]->m_TrianglePoints.push_back(pNewTrianglePoint);
							}
							pNewTrianglePoint->m_Normal.Translate(pTrianglePoint->m_Normal);
							vExchangeTrianglePoint(g_SelectedTriangles[stTriangle], pTrianglePoint, pNewTrianglePoint);
						}
					}
					if(pNewTrianglePoint != 0)
					{
						pNewTrianglePoint->m_Normal.Normalize();
					}
				}
				bKeyAccepted = true;
				
				break;
			}
		case 36: // ENTER
			{
				if(g_SelectedTriangles.size() > 0)
				{
					for(std::vector< Triangle * >::size_type stTriangle = 0; stTriangle < g_SelectedTriangles.size(); ++stTriangle)
					{
						g_SelectedTriangles[stTriangle]->vRealignNormal();
					}
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
						vDeleteTriangle(g_SelectedTriangles.front());
					}
					bKeyAccepted = true;
				}
				
				break;
			}
		}
		
		return bKeyAccepted;
	}
};

class CameraView : public KeyAcceptor
{
public:
	virtual bool AcceptKey(int KeyCode, bool IsDown) override
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
		case 65: // SPACE
			{
				if(g_pHoveredCamera != 0)
				{
					g_CurrentCamera = g_pHoveredCamera;
					bKeyAccepted = true;
				}
				else if(g_SelectedCamera != 0)
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
						g_SelectedCamera->SetPosition(GetAdjustedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Right));
					}
					else if(g_CurrentCamera != nullptr)
					{
						if(g_ControlActive == true)
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
						g_SelectedCamera->SetPosition(GetAdjustedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Left));
					}
					else if(g_CurrentCamera != nullptr)
					{
						if(g_ControlActive == true)
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
						g_SelectedCamera->SetPosition(GetAdjustedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Up));
					}
					else if(g_CurrentCamera != nullptr)
					{
						if(g_ControlActive == true)
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
						g_SelectedCamera->SetPosition(GetAdjustedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Down));
					}
					else if(g_CurrentCamera != nullptr)
					{
						if(g_ControlActive == true)
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
						g_SelectedCamera->SetPosition(GetAdjustedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Backward));
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
						g_SelectedCamera->SetPosition(GetAdjustedStepInView(g_SelectedCamera->GetPosition(), FixedDirection::Forward));
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
				if(g_SelectedCamera != 0)
				{
					g_SelectedCamera->SetFieldOfViewY(g_SelectedCamera->GetFieldOfViewY() + 1.0f);
				}
				else if(g_CurrentCamera != 0)
				{
					g_CurrentCamera->SetFieldOfViewY(g_SelectedCamera->GetFieldOfViewY() + 1.0f);
					vSetupProjection();
				}
				
				break;
			}
		case 115: // END
			{
				if(g_SelectedCamera != 0)
				{
					g_SelectedCamera->SetFieldOfViewY(g_SelectedCamera->GetFieldOfViewY() - 1.0f);
				}
				else if(g_CurrentCamera != 0)
				{
					g_CurrentCamera->SetFieldOfViewY(g_SelectedCamera->GetFieldOfViewY() - 1.0f);
					vSetupProjection();
				}
				
				break;
			}
		}
		
		return bKeyAccepted;
	}
};

XMLStream & mesh(XMLStream & XMLStream)
{
	XMLStream << element << "mesh";
	
	std::map< Point *, unsigned long > PointMap;
	std::map< TrianglePoint *, unsigned long > TrianglePointMap;
	unsigned long PointIdentifier(0);
	
	for(auto Point : g_Points)
	{
		XMLStream << element << "point" << attribute << "identifier" << value << PointIdentifier << attribute << "position-x" << value << Point->GetPosition()[0] << attribute << "position-y" << value << Point->GetPosition()[1] << attribute << "position-z" << value << Point->GetPosition()[2] << attribute << "name" << value << Point->sGetName() << end;
		PointMap[Point] = PointIdentifier;
		++PointIdentifier;
	}
	
	unsigned long TrianglePointIdentifier(0);
	
	for(auto TrianglePoint : g_TrianglePoints)
	{
		XMLStream << element << "triangle-point" << attribute << "identifier" << value << TrianglePointIdentifier << attribute << "normal-x" << value << TrianglePoint->m_Normal[0] << attribute << "normal-y" << value << TrianglePoint->m_Normal[1] << attribute << "normal-z" << value << TrianglePoint->m_Normal[2];
		XMLStream << element << "point" << attribute << "point-identifier" << value << PointMap[TrianglePoint->m_pPoint] << end;
		XMLStream << end;
		TrianglePointMap[TrianglePoint] = TrianglePointIdentifier;
		++TrianglePointIdentifier;
	}
	
	unsigned long TriangleIdentifier(0);
	
	for(auto Triangle : g_Triangles)
	{
		XMLStream << element << "triangle" << attribute << "identifier" << value << TriangleIdentifier << attribute << "name" << value << Triangle->sGetName();
		XMLStream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << TrianglePointMap[Triangle->pGetTrianglePoint(1)] << end;
		XMLStream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << TrianglePointMap[Triangle->pGetTrianglePoint(2)] << end;
		XMLStream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << TrianglePointMap[Triangle->pGetTrianglePoint(3)] << end;
		XMLStream << end;
		++TriangleIdentifier;
	}
	XMLStream << end;
	
	return XMLStream;
}

Label g_CurrentView;

void ImportMesh(const std::string & FilePath)
{
	std::cout << "Importing from \"" << FilePath << "\"." << std::endl;
	
	std::ifstream InputFileStream(FilePath);
	MeshReader MeshReader(InputFileStream);
	
	MeshReader.parse();
	
	std::vector< Point * > Points(MeshReader.GetPoints());
	std::vector< TrianglePoint * > TrianglePoints(MeshReader.GetTrianglePoints());
	std::vector< Triangle * > Triangles(MeshReader.GetTriangles());
	
	copy(Points.begin(), Points.end(), back_inserter(g_Points));
	copy(TrianglePoints.begin(), TrianglePoints.end(), back_inserter(g_TrianglePoints));
	copy(Triangles.begin(), Triangles.end(), back_inserter(g_Triangles));
}

class ModelView : public Widget
{
public:
	PointsView m_PointView;
	TriangleView m_TriangleView;
	CameraView m_CameraView;
	
	ModelView(void)
	{
		g_UserInterface.vAddKeyAcceptor(this);
		g_UserInterface.vGrabKeyFocus(this);
		vAddKeyAcceptor(&m_PointView);
		vAddKeyAcceptor(&m_TriangleView);
		vAddKeyAcceptor(&m_CameraView);
		g_CurrentView.vSetString("Model View");
	}
	
	virtual bool AcceptKey(int KeyCode, bool IsDown) override
	{
		bool bKeyAccepted(true);
		
		switch(KeyCode)
		{
		case 54: // C
			{
				if(g_AltActive == true)
				{
					vToggleCullFace();
				}
				else if(g_ShiftActive == true)
				{
					vSetKeyAcceptor(&m_CameraView);
					g_CurrentView.vSetString("Camera View");
				}
				
				break;
			}
		case 40: // D
			{
				if(g_AltActive == true)
				{
					vToggleDepthTest();
				}
				
				break;
			}
		case 26: // E
			{
				if(g_AltActive == true)
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
				if(g_AltActive == true)
				{
					vToggleFrontFace();
				}
				
				break;
			}
		case 31: // I
			{
				if(g_AltActive == true)
				{
					ImportMesh("mesh.xml");
				}
				
				break;
			}
		case 46: // L
			{
				if(g_AltActive == true)
				{
					vToggleLighting();
				}
				else if(g_ShiftActive == true)
				{
					std::cout << "Loading scene.xml." << std::endl;
					vClearScene();
					
					std::ifstream InputFileStream("scene.xml");
					SceneReader SceneReader(InputFileStream);
					
					SceneReader.parse();
					
					std::vector< Point * > Points(SceneReader.GetPoints());
					std::vector< TrianglePoint * > TrianglePoints(SceneReader.GetTrianglePoints());
					std::vector< Triangle * > Triangles(SceneReader.GetTriangles());
					
					copy(Points.begin(), Points.end(), back_inserter(g_Points));
					copy(TrianglePoints.begin(), TrianglePoints.end(), back_inserter(g_TrianglePoints));
					copy(Triangles.begin(), Triangles.end(), back_inserter(g_Triangles));
					for(auto & LightDescription : SceneReader.GetLightDescriptions())
					{
						auto NewLight(new Light());
						
						NewLight->SetPosition(LightDescription.m_Position[0], LightDescription.m_Position[1], LightDescription.m_Position[2]);
						NewLight->vSetDiffuseColor(LightDescription.m_DiffuseColor[0], LightDescription.m_DiffuseColor[1], LightDescription.m_DiffuseColor[2], LightDescription.m_DiffuseColor[3]);
						if(LightDescription.m_bEnabled == true)
						{
							NewLight->vEnable();
						}
						g_Lights.push_back(NewLight);
					}
					for(auto & CameraDescription : SceneReader.GetCameraDescriptions())
					{
						auto NewCamera(new Camera());
						
						NewCamera->SetPosition(CameraDescription.Position);
						NewCamera->SetOrientation(CameraDescription.Orientation);
						NewCamera->SetFieldOfViewY(CameraDescription.FieldOfViewY);
						g_Cameras.push_back(NewCamera);
					}
					if(g_Cameras.size() > 0)
					{
						g_CurrentCamera = g_Cameras.front();
						vSetupProjection();
					}
				}
				else
				{
					Light * pLight(new Light());
					
					g_Lights.push_back(pLight);
					pLight->SetPosition(0.0f, 0.0f, 0.0f);
					pLight->vSetDiffuseColor(1.0f, 1.0f, 1.0f);
				}
				
				break;
			}
		case 58: // M
			{
				if(g_ShiftActive == true)
				{
					vSetKeyAcceptor(0);
					g_CurrentView.vSetString("Model View");
				}
				
				break;
			}
		case 33: // P
			{
				if(g_ShiftActive == true)
				{
					vSetKeyAcceptor(&m_PointView);
					g_CurrentView.vSetString("Point View");
				}
				
				break;
			}
		case 39: // S
			{
				if(g_AltActive == true)
				{
					if(IsDown == true)
					{
						vToggleSnapping();
					}
				}
				else if(g_ShiftActive == true)
				{
					if(IsDown == false)
					{
						std::cout << "Saving scene.xml." << std::endl;
						
						std::ofstream OutputFileStream("scene.xml");
						XMLStream XMLStream(OutputFileStream);
						
						XMLStream << element << "scene" << mesh;
						for(auto Light : g_Lights)
						{
							XMLStream << light(Light) << end;
						}
						for(auto Camera : g_Cameras)
						{
							XMLStream << camera(Camera) << end;
						}
					}
				}
				
				break;
			}
		case 28: // T
			{
				if(IsDown == false)
				{
					if(g_ShiftActive == true)
					{
						vSetKeyAcceptor(&m_TriangleView);
						g_CurrentView.vSetString("Triangle View");
					}
				}
				
				break;
			}
		case 36: // ENTER
			{
				if(IsDown == true)
				{
					if(g_pSelectedLight != 0)
					{
						if(g_pSelectedLight->bIsEnabled() == true)
						{
							g_pSelectedLight->vDisable();
						}
						else
						{
							g_pSelectedLight->vEnable();
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
					if(g_pSelectedLight != 0)
					{
						vDeleteLight(g_pSelectedLight);
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
					if(pPoint == g_SelectedTriangles.front()->pGetPoint(2))
					{
						g_SelectedPoints.push_back(g_SelectedTriangles.front()->pGetPoint(1));
					}
					else if(pPoint == g_SelectedTriangles.front()->pGetPoint(3))
					{
						g_SelectedPoints.push_back(g_SelectedTriangles.front()->pGetPoint(2));
					}
					else
					{
						g_SelectedPoints.push_back(g_SelectedTriangles.front()->pGetPoint(3));
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
					if(pPoint == g_SelectedTriangles.front()->pGetPoint(1))
					{
						g_SelectedPoints.push_back(g_SelectedTriangles.front()->pGetPoint(2));
					}
					else if(pPoint == g_SelectedTriangles.front()->pGetPoint(2))
					{
						g_SelectedPoints.push_back(g_SelectedTriangles.front()->pGetPoint(3));
					}
					else
					{
						g_SelectedPoints.push_back(g_SelectedTriangles.front()->pGetPoint(1));
					}
				}
				
				break;
			}
		case 114: // RIGHT
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 0, 0.01);
					bKeyAccepted = true;
				}
				
				break;
			}
		case 113: // LEFT
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 0, -0.01);
					bKeyAccepted = true;
				}
				
				break;
			}
		case 111: // UP
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 1, 0.01);
					bKeyAccepted = true;
				}
				
				break;
			}
		case 116: // DOWN
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 1, -0.01);
					bKeyAccepted = true;
				}
				
				break;
			}
		case 117: // PAGE DOWN
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 2, 0.01);
					bKeyAccepted = true;
				}
				
				break;
			}
		case 112: // PAGE UP
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 2, -0.01);
					bKeyAccepted = true;
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
		default:
			{
				bKeyAccepted = false;
			}
		}
		
		return bKeyAccepted;
	}
};

ModelView g_ModelView;

void KeyEvent(unsigned int KeyCode, bool IsDown)
{
	ON_DEBUG(std::cout << "Key event " << KeyCode << " / " << IsDown << "." << std::endl);
	g_UserInterface.AcceptKey(KeyCode, IsDown);
}

void MouseButtonEvent(MouseButton Button, bool IsDown, const Vector2f & MousePosition)
{
	switch(Button)
	{
	case MouseButton::Left:
		{
			if(IsDown == false)
			{
				if(g_pHoveredPoint != 0)
				{
					if(g_ShiftActive == true)
					{
						std::vector< Point * >::iterator iHoveredPoint(std::find(g_SelectedPoints.begin(), g_SelectedPoints.end(), g_pHoveredPoint));
						
						if(iHoveredPoint == g_SelectedPoints.end())
						{
							g_SelectedPoints.push_back(g_pHoveredPoint);
						}
						else
						{
							g_SelectedPoints.erase(iHoveredPoint);
						}
					}
					else
					{
						if((g_SelectedPoints.size() == 1) && (g_SelectedPoints.front() == g_pHoveredPoint))
						{
							g_SelectedPoints.clear();
						}
						else
						{
							g_SelectedPoints.clear();
							g_SelectedPoints.push_back(g_pHoveredPoint);
						}
					}
				}
				else if(g_pHoveredTriangle != 0)
				{
					if(g_ShiftActive == true)
					{
						std::vector< Triangle * >::iterator iHoveredTriangle(std::find(g_SelectedTriangles.begin(), g_SelectedTriangles.end(), g_pHoveredTriangle));
						
						if(iHoveredTriangle == g_SelectedTriangles.end())
						{
							g_SelectedTriangles.push_back(g_pHoveredTriangle);
						}
						else
						{
							g_SelectedTriangles.erase(iHoveredTriangle);
						}
					}
					else
					{
						if((g_SelectedTriangles.size() == 1) && (g_SelectedTriangles.front() == g_pHoveredTriangle))
						{
							g_SelectedTriangles.clear();
						}
						else
						{
							g_SelectedTriangles.clear();
							g_SelectedTriangles.push_back(g_pHoveredTriangle);
						}
					}
				}
				else if(g_pHoveredLight != 0)
				{
					if(g_pSelectedLight == g_pHoveredLight)
					{
						g_pSelectedLight = 0;
					}
					else
					{
						g_pSelectedLight = g_pHoveredLight;
					}
				}
				else if(g_pHoveredCamera != 0)
				{
					if(g_SelectedCamera == g_pHoveredCamera)
					{
						g_SelectedCamera = 0;
					}
					else
					{
						g_SelectedCamera = g_pHoveredCamera;
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
				g_iLastMotionX = MousePosition[0];
				g_iLastMotionY = MousePosition[1];
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
					g_pSelectedLight = 0;
					g_SelectedCamera = 0;
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
	
	int iDeltaX = MousePosition[0] - g_iLastMotionX;
	int iDeltaY = MousePosition[1] - g_iLastMotionY;
	
	g_iLastMotionX = MousePosition[0];
	g_iLastMotionY = MousePosition[1];
	if((g_MouseButton == MouseButton::Middle) && (g_CurrentCamera != 0))
	{
		g_CurrentCamera->SetOrientation(g_CurrentCamera->GetOrientation().RotatedX(-0.005 * iDeltaY).RotateY(-0.005 * iDeltaX));
	}
}

void Resize(void)
{
	vSetupViewport();
	vSetupProjection();
}

void vSetupUserInterface(void)
{
	Widget * pRootWidget(new Widget());
	
	pRootWidget->vSetLayout(new Boxes::FreeLayout());
	g_UserInterface.vSetRootWidget(pRootWidget);
	
	Widget * pCopyright(new Label("littlem by PunyTroll"));
	
	pRootWidget->vAddBox(pCopyright, Boxes::FreeLayout::AddOptions(0.0f, 20.0f));
	pRootWidget->vAddBox(&g_CurrentView, Boxes::FreeLayout::AddOptions(0.0f, 36.0f));
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
	XStoreName(g_Display, g_Window, "galactic-fall 0.2");
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
	for(int iLight = 0; iLight < iNumberOfLights; ++iLight)
	{
		g_FreeLights.push_back(iLight);
	}
	glEnable(GL_LIGHTING);
	
	Light * pLight(new Light());
	
	g_Lights.push_back(pLight);
	pLight->SetPosition(0.0f, 0.0f, 4.0f);
	pLight->vSetDiffuseColor(1.0f, 1.0f, 1.0f);
	pLight->vEnable();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	Camera * pCamera(new Camera());
	
	g_Cameras.push_back(pCamera);
	g_CurrentCamera = pCamera;
	if((g_UpAxis == FixedAxis::PositiveZ) && (g_ForwardAxis == FixedAxis::PositiveX))
	{
		g_CurrentCamera->SetOrientation(Quaternion::CreateAsRotationX(M_PI_2).RotateY(M_PI_2));
		g_CurrentCamera->SetPosition(Vector3f::CreateTranslationX(4.0f));
	}
	g_Snapping = true;
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
				if((Event.xkey.keycode == 50) || (Event.xkey.keycode == 62)) // LEFT SHIFT or RIGHT SHIFT
				{
					g_ShiftActive = Event.xkey.type == KeyPress;
				}
				else if((Event.xkey.keycode == 37) || (Event.xkey.keycode == 105)) // LEFT CONTROL or RIGHT CONTROL
				{
					g_ControlActive = Event.xkey.type == KeyPress;
				}
				else if(Event.xkey.keycode == 64)
				{
					g_AltActive = Event.xkey.type == KeyPress;
				}
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
	// </static-initialization>
	// setup the random number generator for everyday use
	srand(time(0));
	ON_DEBUG(std::cout << "Setting up the window." << std::endl);
	CreateWindow();
	ON_DEBUG(std::cout << "Initializing OpenGL." << std::endl);
	InitializeOpenGL();
	vSetupUserInterface();
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
