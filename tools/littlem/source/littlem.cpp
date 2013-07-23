#include <assert.h>
#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

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

#define glPointParameterfv(A, B) glPointParameterfvARB((A), (B))
#define glPointParameterf(A, B) glPointParameterfARB((A), (B))

// define names for keys which are not yet defined by GLUT
#define GLUT_KEY_BACKSPACE 8
#define GLUT_KEY_TABULATOR 9
#define GLUT_KEY_ENTER 13
#define GLUT_KEY_ESCAPE 27
#define GLUT_KEY_DELETE 127

// define our own key codes
#define LITTLEM_KEY_NONE -1
#define LITTLEM_KEY_0 0
#define LITTLEM_KEY_1 1
#define LITTLEM_KEY_2 2
#define LITTLEM_KEY_3 3
#define LITTLEM_KEY_4 4
#define LITTLEM_KEY_5 5
#define LITTLEM_KEY_6 6
#define LITTLEM_KEY_7 7
#define LITTLEM_KEY_8 8
#define LITTLEM_KEY_9 9
#define LITTLEM_KEY_A 10
#define LITTLEM_KEY_B 11
#define LITTLEM_KEY_C 12
#define LITTLEM_KEY_D 13
#define LITTLEM_KEY_E 14
#define LITTLEM_KEY_F 15
#define LITTLEM_KEY_G 16
#define LITTLEM_KEY_H 17
#define LITTLEM_KEY_I 18
#define LITTLEM_KEY_J 19
#define LITTLEM_KEY_K 20
#define LITTLEM_KEY_L 21
#define LITTLEM_KEY_M 22
#define LITTLEM_KEY_N 23
#define LITTLEM_KEY_O 24
#define LITTLEM_KEY_P 25
#define LITTLEM_KEY_Q 26
#define LITTLEM_KEY_R 27
#define LITTLEM_KEY_S 28
#define LITTLEM_KEY_T 29
#define LITTLEM_KEY_U 30
#define LITTLEM_KEY_V 31
#define LITTLEM_KEY_W 32
#define LITTLEM_KEY_X 33
#define LITTLEM_KEY_Y 34
#define LITTLEM_KEY_Z 35
#define LITTLEM_KEY_SPACE 36
#define LITTLEM_KEY_ENTER 37
#define LITTLEM_KEY_ESCAPE 38
#define LITTLEM_KEY_DELETE 39
#define LITTLEM_KEY_PLUS 40
#define LITTLEM_KEY_MINUS 41
#define LITTLEM_KEY_ASTERISK 42
#define LITTLEM_KEY_LESS_THAN 43
#define LITTLEM_KEY_GREATER_THAN 44
#define LITTLEM_KEY_SLASH 45
#define LITTLEM_KEY_LEFT_BRACKET 46
#define LITTLEM_KEY_RIGHT_BRACKET 47
#define LITTLEM_KEY_UP 48
#define LITTLEM_KEY_DOWN 49
#define LITTLEM_KEY_LEFT 50
#define LITTLEM_KEY_RIGHT 51
#define LITTLEM_KEY_PAGE_UP 52
#define LITTLEM_KEY_PAGE_DOWN 53
#define LITTLEM_KEY_HOME 54
#define LITTLEM_KEY_END 55
#define LITTLEM_KEY_F1 56
#define LITTLEM_KEY_F2 57
#define LITTLEM_KEY_F3 58
#define LITTLEM_KEY_F4 59
#define LITTLEM_KEY_F5 60
#define LITTLEM_KEY_F6 61
#define LITTLEM_KEY_F12 67
#define LITTLEM_KEY_BACKTICK 68
#define LITTLEM_KEY_BACKSPACE 69
#define LITTLEM_KEY_FULL_STOP 70
#define LITTLEM_KEY_TABULATOR 71

// define flags for mouse buttons
#define LITTLEM_LEFT_MOUSE_BUTTON 1
#define LITTLEM_MIDDLE_MOUSE_BUTTON 2
#define LITTLEM_RIGHT_MOUSE_BUTTON 4

// types of objects
#define LITTLEM_CAMERA 0
#define LITTLEM_LIGHT 1
#define LITTLEM_POINT 2
#define LITTLEM_TRIANGLE 3

// define names for mouse buttons 3 and 4 which are mouse wheel up and down respectively
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

#define CHECK_GL_ERROR() vCheckGLError(__FILE__, __LINE__)

// XML writing
struct XMLPoint
{
	unsigned long m_Identifier;
	Vector3f m_Position;
	std::string m_Name;
};

inline XMLPoint point(unsigned long Identifier, const Vector3f & Position, const std::string & Name)
{
	XMLPoint Point;
	
	Point.m_Identifier = Identifier;
	Point.m_Position = Position;
	Point.m_Name = Name;
	
	return Point;
}

XMLStream & operator<<(XMLStream & XMLStream, const XMLPoint & Point)
{
	XMLStream << element << "point" << attribute << "identifier" << value << Point.m_Identifier << attribute << "position-x" << value << Point.m_Position[0] << attribute << "position-y" << value << Point.m_Position[1] << attribute << "position-z" << value << Point.m_Position[2] << attribute << "name" << value << Point.m_Name;
	
	return XMLStream;
}

struct XMLPointReference
{
	unsigned long m_PointIdentifier;
};

inline XMLPointReference point(unsigned long PointIdentifier)
{
	XMLPointReference Point;
	
	Point.m_PointIdentifier = PointIdentifier;
	
	return Point;
}

XMLStream & operator<<(XMLStream & XMLStream, const XMLPointReference & PointReference)
{
	XMLStream << element << "point" << attribute << "point-identifier" << value <<PointReference.m_PointIdentifier;
	
	return XMLStream;
}

struct XMLTriangle
{
	unsigned long m_Identifier;
	std::string m_Name;
};

inline XMLTriangle triangle(unsigned long Identifier, const std::string & Name)
{
	XMLTriangle Triangle;
	
	Triangle.m_Identifier = Identifier;
	Triangle.m_Name = Name;
	
	return Triangle;
}

XMLStream & operator<<(XMLStream & XMLStream, const XMLTriangle & Triangle)
{
	XMLStream << element << "triangle" << attribute << "identifier" << value << Triangle.m_Identifier << attribute << "name" << value << Triangle.m_Name;
	
	return XMLStream;
}

struct XMLTrianglePoint
{
	unsigned long m_TrianglePointIdentifier;
	Vector3f m_Normal;
};

inline XMLTrianglePoint trianglepoint(unsigned long TrianglePointIdentifier, const Vector3f & Normal)
{
	XMLTrianglePoint TrianglePoint;
	
	TrianglePoint.m_TrianglePointIdentifier = TrianglePointIdentifier;
	TrianglePoint.m_Normal = Normal;
	
	return TrianglePoint;
}

XMLStream & operator<<(XMLStream & XMLStream, const XMLTrianglePoint & TrianglePoint)
{
	XMLStream << element << "triangle-point" << attribute << "identifier" << value << TrianglePoint.m_TrianglePointIdentifier << attribute << "normal-x" << value << TrianglePoint.m_Normal[0] << attribute << "normal-y" << value << TrianglePoint.m_Normal[1] << attribute << "normal-z" << value << TrianglePoint.m_Normal[2];
	
	return XMLStream;
}

struct XMLTrianglePointReference
{
	unsigned long m_TrianglePointIdentifier;
};

inline XMLTrianglePointReference trianglepoint(unsigned long TrianglePointIdentifier)
{
	XMLTrianglePointReference TrianglePointReference;
	
	TrianglePointReference.m_TrianglePointIdentifier = TrianglePointIdentifier;
	
	return TrianglePointReference;
}

XMLStream & operator<<(XMLStream & XMLStream, const XMLTrianglePointReference & TrianglePointReference)
{
	XMLStream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << TrianglePointReference.m_TrianglePointIdentifier;
	
	return XMLStream;
}

#include "xmlparser.h"

enum
{
	LM_OGLE_ARB_point_parameters,
	LM_OGLE_LM_number_of_extensions
};

enum
{
	LITTLEM_VIEW_NONE = -1,
	LITTLEM_VIEW_FRONT = 0,
	LITTLEM_VIEW_LEFT = 1,
	LITTLEM_VIEW_BACK = 2,
	LITTLEM_VIEW_RIGHT = 3,
	LITTLEM_VIEW_TOP = 4,
	LITTLEM_VIEW_BOTTOM = 5
};

#include "point.h"
#include "triangle.h"
#include "triangle_point.h"

struct XMLModel
{
	XMLModel(const std::vector< Point * > & Points, const std::vector< TrianglePoint * > & TrianglePoints, const std::vector< Triangle * > & Triangles) :
		m_Points(Points),
		m_TrianglePoints(TrianglePoints),
		m_Triangles(Triangles)
	{
	}
	
	const std::vector< Point * > & m_Points;
	const std::vector< TrianglePoint * > & m_TrianglePoints;
	const std::vector< Triangle * > & m_Triangles;
};

inline XMLModel model(const std::vector< Point * > & Points, const std::vector< TrianglePoint * > & TrianglePoints, const std::vector< Triangle * > & Triangles)
{
	return XMLModel(Points, TrianglePoints, Triangles);
}

XMLStream & operator<<(XMLStream & XMLStream, XMLModel Model)
{
	XMLStream << element << "model";
	
	std::map< Point *, unsigned long > PointMap;
	std::map< TrianglePoint *, unsigned long > TrianglePointMap;
	
	for(std::vector< Point * >::size_type stPoint = 0; stPoint < Model.m_Points.size(); ++stPoint)
	{
		XMLStream << point(stPoint, Model.m_Points[stPoint]->GetPosition(), Model.m_Points[stPoint]->sGetName()) << end;
		PointMap[Model.m_Points[stPoint]] = stPoint;
	}
	for(std::vector< TrianglePoint * >::size_type stTrianglePoint = 0; stTrianglePoint < Model.m_TrianglePoints.size(); ++stTrianglePoint)
	{
		XMLStream << trianglepoint(stTrianglePoint, Model.m_TrianglePoints[stTrianglePoint]->m_Normal);
		XMLStream << point(PointMap[Model.m_TrianglePoints[stTrianglePoint]->m_pPoint]) << end;
		XMLStream << end;
		TrianglePointMap[Model.m_TrianglePoints[stTrianglePoint]] = stTrianglePoint;
	}
	for(std::vector< Triangle * >::size_type stTriangle = 0; stTriangle < Model.m_Triangles.size(); ++stTriangle)
	{
		XMLStream << triangle(stTriangle, Model.m_Triangles[stTriangle]->sGetName());
		XMLStream << trianglepoint(TrianglePointMap[Model.m_Triangles[stTriangle]->pGetTrianglePoint(1)]) << end;
		XMLStream << trianglepoint(TrianglePointMap[Model.m_Triangles[stTriangle]->pGetTrianglePoint(2)]) << end;
		XMLStream << trianglepoint(TrianglePointMap[Model.m_Triangles[stTriangle]->pGetTrianglePoint(3)]) << end;
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

class Camera : public Position
{
public:
	Quaternion m_Orientation;
	float m_fFieldOfView;
};

class CameraDescription
{
public:
	CameraDescription(void)
	{
	}
	
	CameraDescription(Camera * Camera) :
		Position(Camera->GetPosition()),
		Orientation(Camera->m_Orientation),
		FieldOfView(Camera->m_fFieldOfView)
	{
	}
	
	Vector3f Position;
	Quaternion Orientation;
	float FieldOfView;
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
	XMLStream << attribute << "field-of-view" << value << CameraDescription.FieldOfView;
	
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
			
			LightDescription.m_Position = Vector3f(ConvertToFloat(Attributes.find("position-x")->second), ConvertToFloat(Attributes.find("position-y")->second), ConvertToFloat(Attributes.find("position-z")->second));
			LightDescription.m_DiffuseColor = Vector4f(ConvertToFloat(Attributes.find("color-red")->second), ConvertToFloat(Attributes.find("color-green")->second), ConvertToFloat(Attributes.find("color-blue")->second), ConvertToFloat(Attributes.find("color-alpha")->second));
			LightDescription.m_bEnabled = ConvertToBool(Attributes.find("enabled")->second);
			
			m_LightDescriptions.push_back(LightDescription);
		}
		else if(sElementName == "camera")
		{
			CameraDescription CameraDescription;
			
			CameraDescription.Position = Vector3f(ConvertToFloat(Attributes.find("position-x")->second), ConvertToFloat(Attributes.find("position-y")->second), ConvertToFloat(Attributes.find("position-z")->second));
			CameraDescription.Orientation = Quaternion(ConvertToFloat(Attributes.find("orientation-w")->second), ConvertToFloat(Attributes.find("orientation-x")->second), ConvertToFloat(Attributes.find("orientation-y")->second), ConvertToFloat(Attributes.find("orientation-z")->second));
			CameraDescription.FieldOfView = ConvertToFloat(Attributes.find("field-of-view")->second);
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
Camera * g_pSelectedCamera = 0;
Camera * g_pCurrentCamera = 0;
GLsizei g_sWidth = 800;
GLsizei g_sHeight = 800;
int g_iLastMotionX = -1;
int g_iLastMotionY = -1;
int g_iMouseX = 0;
int g_iMouseY = 0;
bool g_bMoved = false;
std::string g_sExtensionString;
GLuint g_puiSelectionBuffer[1024];
bool g_bSnapping = false;
float g_fSnapFactor = 0.1;
int g_iMouseButtonFlags = 0;
std::deque< int > g_FreeLights;
UserInterface g_UserInterface;

#include "opengl_extension.h"

std::vector< OpenGLExtension > g_OpenGLExtensions;

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
	glViewport(0, 0, g_sWidth, g_sHeight);
}

void vSetupProjection(bool bInitialize = true)
{
	glMatrixMode(GL_PROJECTION);
	if(bInitialize == true)
	{
		glLoadIdentity();
	}
	if(g_pCurrentCamera != 0)
	{
		gluPerspective(g_pCurrentCamera->m_fFieldOfView, static_cast< GLfloat >(g_sWidth) / static_cast< GLfloat >(g_sHeight), 1.0f, 1000.0f);
	}
}

void vStartPicking(int iCursorX, int iCursorY)
{
	GLint piViewport[4];
	
	glSelectBuffer(1024, g_puiSelectionBuffer);
	glRenderMode(GL_SELECT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glGetIntegerv(GL_VIEWPORT, piViewport);
	gluPickMatrix(iCursorX, piViewport[3] - iCursorY, 5, 5, piViewport);
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

void vResetView(int iFixedViewIndex)
{
	if(g_pCurrentCamera != 0)
	{
		g_pCurrentCamera->m_fFieldOfView = 52.0f;
		switch(iFixedViewIndex)
		{
		case LITTLEM_VIEW_FRONT:
			{
				g_pCurrentCamera->SetPosition(0.0f, 0.0f, 4.0f);
				g_pCurrentCamera->m_Orientation.Identity();
				
				break;
			}
		case LITTLEM_VIEW_LEFT:
			{
				g_pCurrentCamera->SetPosition(-4.0f, 0.0f, 0.0f);
				g_pCurrentCamera->m_Orientation.RotationY(-M_PI_2);
				
				break;
			}
		case LITTLEM_VIEW_BACK:
			{
				g_pCurrentCamera->SetPosition(0.0f, 0.0f, -4.0f);
				g_pCurrentCamera->m_Orientation.RotationY(M_PI);
				
				break;
			}
		case LITTLEM_VIEW_RIGHT:
			{
				g_pCurrentCamera->SetPosition(4.0f, 0.0f, 0.0f);
				g_pCurrentCamera->m_Orientation.RotationY(M_PI_2);
				
				break;
			}
		case LITTLEM_VIEW_TOP:
			{
				g_pCurrentCamera->SetPosition(0.0f, 4.0f, 0.0f);
				g_pCurrentCamera->m_Orientation.RotationX(-M_PI_2);
				
				break;
			}
		case LITTLEM_VIEW_BOTTOM:
			{
				g_pCurrentCamera->SetPosition(0.0f, -4.0f, 0.0f);
				g_pCurrentCamera->m_Orientation.RotationX(M_PI_2);
				
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
	glOrtho(0.0, static_cast< GLfloat >(g_sWidth), 0.0, static_cast< GLfloat >(g_sHeight), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(0.0f, -g_sHeight, 0.0f);
	glColor3f(0.4f, 1.0f, 0.4f);
	
	std::stringstream ssStatusText;
	
	ssStatusText << "#Points: " << g_Points.size() << "   #TrianglePoints: " << g_TrianglePoints.size() << "   #Triangles: " << g_Triangles.size() << "   #Selected Points: " << g_SelectedPoints.size() << "   Width: " << g_sWidth << "   Height: " << g_sHeight << "   Front Face: ";
	
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
	if(g_bSnapping == true)
	{
		ssSnapInformationText << "on   SnapFactor: " << g_fSnapFactor;
	}
	else
	{
		ssSnapInformationText << "off";
	}
	vDrawTextAt(0, g_sHeight - 24, ssSnapInformationText.str());
	if(g_SelectedPoints.size() > 0)
	{
		std::stringstream ssPointInformationText;
		
		ssPointInformationText << "Point:  X: " << g_SelectedPoints.front()->GetX() << "   Y: " << g_SelectedPoints.front()->GetY() << "   Z: " << g_SelectedPoints.front()->GetZ() << "   Name: \"" << g_SelectedPoints.front()->sGetName() << "\"   #TrianglePoints: " << g_SelectedPoints.front()->m_TrianglePoints.size();
		vDrawTextAt(0, g_sHeight - 36, ssPointInformationText.str());
	}
	if(g_SelectedTriangles.size() > 0)
	{
		std::stringstream ssTriangleInformationText;
		
		ssTriangleInformationText << "Triangle:  Name: \"" << g_SelectedTriangles.front()->sGetName() << '"';
		vDrawTextAt(0, g_sHeight - 48, ssTriangleInformationText.str());
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
 *A precondition for this function is that a current camera is selected in g_pCurrentCamera.
 **/
void vPerformPicking(void)
{
	assert(g_pCurrentCamera != 0);
	
	vStartPicking(g_iMouseX, g_iMouseY);
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
		g_Cameras[stCamera]->DrawSelection();
		glEnd();
	}
	glPopName();
	glPopName();
	vStopPicking();
}

/**
 * @brief Displays the model.
 * 
 * A precondition for this function is that a current camera is selected in g_pCurrentCamera.
 **/
void vDisplayModel(void)
{
	assert(g_pCurrentCamera != 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(Matrix4f::CreateFromQuaternion(g_pCurrentCamera->m_Orientation).GetPointer());
	glTranslatef(-g_pCurrentCamera->GetX(), -g_pCurrentCamera->GetY(), -g_pCurrentCamera->GetZ());
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
		g_Cameras[stCamera]->Draw();
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
		Matrix4f Matrix(Matrix4f::CreateFromQuaternion(g_pCurrentCamera->m_Orientation.Conjugated()));
		
		for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
		{
			glPushMatrix();
			glTranslatef(g_SelectedPoints[stI]->GetX(), g_SelectedPoints[stI]->GetY(), g_SelectedPoints[stI]->GetZ());
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
	}
	if(g_pSelectedLight != 0)
	{
		glPushMatrix();
		glTranslatef(g_pSelectedLight->GetX(), g_pSelectedLight->GetY(), g_pSelectedLight->GetZ());
		glMultMatrixf(Matrix4f::CreateFromQuaternion(g_pCurrentCamera->m_Orientation.Conjugated()).GetPointer());
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, -0.05f, 0.0f);
		glVertex3f(0.05f, -0.05f, 0.0f);
		glVertex3f(0.05f, 0.05f, 0.0f);
		glVertex3f(-0.05f, 0.05f, 0.0f);
		glEnd();
		glPopMatrix();
	}
	if(g_pSelectedCamera != 0)
	{
		glPushMatrix();
		glTranslatef(g_pSelectedCamera->GetX(), g_pSelectedCamera->GetY(), g_pSelectedCamera->GetZ());
		glPushMatrix();
		glMultMatrixf(Matrix4f::CreateFromQuaternion(g_pCurrentCamera->m_Orientation.Conjugated()).GetPointer());
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, -0.05f, 0.0f);
		glVertex3f(0.05f, -0.05f, 0.0f);
		glVertex3f(0.05f, 0.05f, 0.0f);
		glVertex3f(-0.05f, 0.05f, 0.0f);
		glEnd();
		glPopMatrix();
		glMultMatrixf(Matrix4f::CreateFromQuaternion(g_pSelectedCamera->m_Orientation.Conjugated()).GetPointer());
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
		g_pHoveredCamera->Draw();
		glEnd();
		glPushMatrix();
		glTranslatef(g_pHoveredCamera->GetX(), g_pHoveredCamera->GetY(), g_pHoveredCamera->GetZ());
		glMultMatrixf(Matrix4f::CreateFromQuaternion(g_pHoveredCamera->m_Orientation.Conjugated()).GetPointer());
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
	if(g_pCurrentCamera != 0)
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
	glutSwapBuffers();
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
	if(pCamera == g_pCurrentCamera)
	{
		g_pCurrentCamera = 0;
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
	glutPostRedisplay();
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
	glutPostRedisplay();
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
	glutPostRedisplay();
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
	glutPostRedisplay();
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
	glutPostRedisplay();
}

/**
 * @brief Toggle the snapping option.
 * 
 * The snapping option defines whether movement operations on points will snap their positions to a grid.
 **/
void vToggleSnapping(void)
{
	g_bSnapping = !g_bSnapping;
	glutPostRedisplay();
}

void vAdjustFloatValue(float & fValue, float fDelta)
{
	if(g_bSnapping == true)
	{
		if(fDelta < 0)
		{
			if(fDelta > -g_fSnapFactor)
			{
				fDelta = -g_fSnapFactor;
			}
		}
		else
		{
			if(fDelta < g_fSnapFactor)
			{
				fDelta = g_fSnapFactor;
			}
		}
	}
	fValue += fDelta;
	if(g_bSnapping == true)
	{
		fValue = roundf(fValue / g_fSnapFactor) * g_fSnapFactor;
	}
}

float GetAdjustedFloatValue(float Value, float Delta)
{
	if(g_bSnapping == true)
	{
		if(Delta < 0)
		{
			if(Delta > -g_fSnapFactor)
			{
				Delta = -g_fSnapFactor;
			}
		}
		else
		{
			if(Delta < g_fSnapFactor)
			{
				Delta = g_fSnapFactor;
			}
		}
	}
	
	float Result(Value + Delta);
	
	if(g_bSnapping == true)
	{
		Result = roundf(Result / g_fSnapFactor) * g_fSnapFactor;
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

/**
 * @brief Return a littlem key code for the given character.
 **/
int iGetKeyCodeFromCharacter(unsigned char cKey)
{
	switch(cKey)
	{
	case GLUT_KEY_BACKSPACE: return LITTLEM_KEY_BACKSPACE;
	case GLUT_KEY_TABULATOR: return LITTLEM_KEY_TABULATOR;
	case GLUT_KEY_ENTER: return LITTLEM_KEY_ENTER;
	case GLUT_KEY_ESCAPE: return LITTLEM_KEY_ESCAPE;
	case GLUT_KEY_DELETE: return LITTLEM_KEY_DELETE;
	case ' ': return LITTLEM_KEY_SPACE;
	case '0': return LITTLEM_KEY_0;
	case '1': return LITTLEM_KEY_1;
	case '2': return LITTLEM_KEY_2;
	case '3': return LITTLEM_KEY_3;
	case '4': return LITTLEM_KEY_4;
	case '5': return LITTLEM_KEY_5;
	case '6': return LITTLEM_KEY_6;
	case '7': return LITTLEM_KEY_7;
	case '8': return LITTLEM_KEY_8;
	case '9': return LITTLEM_KEY_9;
	case 'a':
	case 'A': return LITTLEM_KEY_A;
	case 'b':
	case 'B': return LITTLEM_KEY_B;
	case 'c':
	case 'C': return LITTLEM_KEY_C;
	case 'd':
	case 'D': return LITTLEM_KEY_D;
	case 'e':
	case 'E': return LITTLEM_KEY_E;
	case 'f':
	case 'F': return LITTLEM_KEY_F;
	case 'g':
	case 'G': return LITTLEM_KEY_G;
	case 'h':
	case 'H': return LITTLEM_KEY_H;
	case 'i':
	case 'I': return LITTLEM_KEY_I;
	case 'j':
	case 'J': return LITTLEM_KEY_J;
	case 'k':
	case 'K': return LITTLEM_KEY_K;
	case 'l':
	case 'L': return LITTLEM_KEY_L;
	case 'm':
	case 'M': return LITTLEM_KEY_M;
	case 'n':
	case 'N': return LITTLEM_KEY_N;
	case 'o':
	case 'O': return LITTLEM_KEY_O;
	case 'p':
	case 'P': return LITTLEM_KEY_P;
	case 'q':
	case 'Q': return LITTLEM_KEY_Q;
	case 'r':
	case 'R': return LITTLEM_KEY_R;
	case 's':
	case 'S': return LITTLEM_KEY_S;
	case 't':
	case 'T': return LITTLEM_KEY_T;
	case 'u':
	case 'U': return LITTLEM_KEY_U;
	case 'v':
	case 'V': return LITTLEM_KEY_V;
	case 'w':
	case 'W': return LITTLEM_KEY_W;
	case 'x':
	case 'X': return LITTLEM_KEY_X;
	case 'y':
	case 'Y': return LITTLEM_KEY_Y;
	case 'z':
	case 'Z': return LITTLEM_KEY_Z;
	case '+': return LITTLEM_KEY_PLUS;
	case '-': return LITTLEM_KEY_MINUS;
	case '*': return LITTLEM_KEY_ASTERISK;
	case '<': return LITTLEM_KEY_LESS_THAN;
	case '>': return LITTLEM_KEY_GREATER_THAN;
	case '/': return LITTLEM_KEY_SLASH;
	case '[': return LITTLEM_KEY_LEFT_BRACKET;
	case ']': return LITTLEM_KEY_RIGHT_BRACKET;
	case '`': return LITTLEM_KEY_BACKTICK;
	case '.': return LITTLEM_KEY_FULL_STOP;
	}
	return LITTLEM_KEY_NONE;
}

/**
 * @brief Return a littlem key code for the given GLUT key code.
 **/
int iGetKeyCodeFromSpecialKey(int iKey)
{
	switch(iKey)
	{
	case GLUT_KEY_LEFT: return LITTLEM_KEY_LEFT;
	case GLUT_KEY_RIGHT: return LITTLEM_KEY_RIGHT;
	case GLUT_KEY_UP: return LITTLEM_KEY_UP;
	case GLUT_KEY_DOWN: return LITTLEM_KEY_DOWN;
	case GLUT_KEY_PAGE_UP: return LITTLEM_KEY_PAGE_UP;
	case GLUT_KEY_PAGE_DOWN: return LITTLEM_KEY_PAGE_DOWN;
	case GLUT_KEY_HOME: return LITTLEM_KEY_HOME;
	case GLUT_KEY_END: return LITTLEM_KEY_END;
	case GLUT_KEY_F1: return LITTLEM_KEY_F1;
	case GLUT_KEY_F2: return LITTLEM_KEY_F2;
	case GLUT_KEY_F3: return LITTLEM_KEY_F3;
	case GLUT_KEY_F4: return LITTLEM_KEY_F4;
	case GLUT_KEY_F5: return LITTLEM_KEY_F5;
	case GLUT_KEY_F6: return LITTLEM_KEY_F6;
	case GLUT_KEY_F12: return LITTLEM_KEY_F12;
	}
	return LITTLEM_KEY_NONE;
}

bool bIsAlphabetKeyCode(int iKeyCode)
{
	return (iKeyCode >= LITTLEM_KEY_A) && (iKeyCode <= LITTLEM_KEY_Z);
}

bool bIsDigitKeyCode(int iKeyCode)
{
	return (iKeyCode >= LITTLEM_KEY_0) && (iKeyCode <= LITTLEM_KEY_9);
}

bool bIsFloatNumberKeyCode(int iKeyCode)
{
	return bIsDigitKeyCode(iKeyCode) || (iKeyCode == LITTLEM_KEY_FULL_STOP) || (iKeyCode == LITTLEM_KEY_MINUS);
}

bool bIsAlphaNumericKeyCode(int iKeyCode)
{
	return (bIsAlphabetKeyCode(iKeyCode) == true) || (bIsDigitKeyCode(iKeyCode) == true);
}

char cGetCharacterFromKeyCode(int iKeyCode)
{
	if(bIsAlphabetKeyCode(iKeyCode) == true)
	{
		return 'a' + iKeyCode - LITTLEM_KEY_A;
	}
	else if(bIsDigitKeyCode(iKeyCode) == true)
	{
		return '0' + iKeyCode - LITTLEM_KEY_0;
	}
	else if(iKeyCode == LITTLEM_KEY_FULL_STOP)
	{
		return '.';
	}
	else if(iKeyCode == LITTLEM_KEY_MINUS)
	{
		return '-';
	}
	
	return '\0';
}

class PointsView : public KeyAcceptor
{
public:
	virtual bool bAcceptKey(int iKeyCode)
	{
		bool bKeyAccepted(false);
		
		switch(iKeyCode)
		{
		case LITTLEM_KEY_SPACE:
			{
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
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_TABULATOR:
			{
				if(g_SelectedPoints.size() > 0)
				{
					glutPostRedisplay();
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
				
				break;
			}
		case LITTLEM_KEY_D:
			{
				if(g_SelectedPoints.size() > 0)
				{
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				std::vector< Point * > NewSelectedPoints;
				
				for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
				{
					g_Points.push_back(new Point(*g_SelectedPoints[stI]));
					NewSelectedPoints.push_back(g_Points.back());
				}
				g_SelectedPoints = NewSelectedPoints;
				
				break;
			}
		case LITTLEM_KEY_N:
			{
				g_Points.push_back(new Point(-1.0f + 2 * (static_cast< double> (random()) / RAND_MAX), -1.0f + 2 * (static_cast< double> (random()) / RAND_MAX), -1.0f + 2 * (static_cast< double> (random()) / RAND_MAX)));
				bKeyAccepted = true;
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_T:
			{
				if(glutGetModifiers() == 0)
				{
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
						glutPostRedisplay();
						bKeyAccepted = true;
					}
				}
				
				break;
			}
		case LITTLEM_KEY_DELETE:
			{
				while(g_SelectedPoints.size() > 0)
				{
					vDeletePoint(g_SelectedPoints.front());
				}
				glutPostRedisplay();
				bKeyAccepted = true;
				
				break;
			}
		case LITTLEM_KEY_RIGHT:
			{
				if(g_SelectedPoints.size() > 0)
				{
					for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
					{
						MovePosition(g_SelectedPoints[stI], 0, 0.01);
					}
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_LEFT:
			{
				if(g_SelectedPoints.size() > 0)
				{
					for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
					{
						MovePosition(g_SelectedPoints[stI], 0, -0.01);
					}
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_UP:
			{
				if(g_SelectedPoints.size() > 0)
				{
					for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
					{
						MovePosition(g_SelectedPoints[stI], 1, 0.01);
					}
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_DOWN:
			{
				if(g_SelectedPoints.size() > 0)
				{
					for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
					{
						MovePosition(g_SelectedPoints[stI], 1, -0.01);
					}
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_PAGE_DOWN:
			{
				if(g_SelectedPoints.size() > 0)
				{
					for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
					{
						MovePosition(g_SelectedPoints[stI], 2, 0.01);
					}
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_PAGE_UP:
			{
				if(g_SelectedPoints.size() > 0)
				{
					for(std::vector< Point * >::size_type stI = 0; stI < g_SelectedPoints.size(); ++stI)
					{
						MovePosition(g_SelectedPoints[stI], 2, -0.01);
					}
					glutPostRedisplay();
					bKeyAccepted = true;
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
	virtual bool bAcceptKey(int iKeyCode)
	{
		bool bKeyAccepted(false);
		
		switch(iKeyCode)
		{
		case LITTLEM_KEY_I:
			{
				if(glutGetModifiers() == 0)
				{
					if(g_SelectedTriangles.size() > 0)
					{
						for(std::vector< Triangle * >::size_type stTriangle = 0; stTriangle < g_SelectedTriangles.size(); ++stTriangle)
						{
							g_SelectedTriangles[stTriangle]->vInvert();
						}
						glutPostRedisplay();
						bKeyAccepted = true;
					}
				}
				
				break;
			}
		case LITTLEM_KEY_S:
			{
				if(glutGetModifiers() == 0)
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
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_T:
			{
				if(glutGetModifiers() == 0)
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
						glutPostRedisplay();
						bKeyAccepted = true;
					}
				}
				
				break;
			}
		case LITTLEM_KEY_U:
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
							pNewTrianglePoint->m_Normal += pTrianglePoint->m_Normal;
							vExchangeTrianglePoint(g_SelectedTriangles[stTriangle], pTrianglePoint, pNewTrianglePoint);
						}
					}
					if(pNewTrianglePoint != 0)
					{
						pNewTrianglePoint->m_Normal.Normalize();
						glutPostRedisplay();
					}
				}
				bKeyAccepted = true;
				
				break;
			}
		case LITTLEM_KEY_ENTER:
			{
				if(g_SelectedTriangles.size() > 0)
				{
					for(std::vector< Triangle * >::size_type stTriangle = 0; stTriangle < g_SelectedTriangles.size(); ++stTriangle)
					{
						g_SelectedTriangles[stTriangle]->vRealignNormal();
					}
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_DELETE:
			{
				if(g_SelectedTriangles.size() > 0)
				{
					while(g_SelectedTriangles.size() > 0)
					{
						vDeleteTriangle(g_SelectedTriangles.front());
					}
					glutPostRedisplay();
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
	virtual bool bAcceptKey(int iKeyCode)
	{
		bool bKeyAccepted(false);
		
		switch(iKeyCode)
		{
		case LITTLEM_KEY_C:
			{
				if(glutGetModifiers() == 0)
				{
					Camera * pCamera(new Camera());
					
					pCamera->SetPosition(0.0f, 0.0f, 0.0f);
					pCamera->m_Orientation.Identity();
					pCamera->m_fFieldOfView = 52.0f;
					g_Cameras.push_back(pCamera);
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_SPACE:
			{
				if(glutGetModifiers() == 0)
				{
					if(g_pHoveredCamera != 0)
					{
						g_pCurrentCamera = g_pHoveredCamera;
						glutPostRedisplay();
						bKeyAccepted = true;
					}
					else if(g_pSelectedCamera != 0)
					{
						g_pCurrentCamera = g_pSelectedCamera;
						glutPostRedisplay();
						bKeyAccepted = true;
					}
				}
			}
		case LITTLEM_KEY_RIGHT:
			{
				if(g_pSelectedCamera != 0)
				{
					MovePosition(g_pSelectedCamera, 0, 0.01);
				}
				else if(g_pCurrentCamera != 0)
				{
					MovePosition(g_pCurrentCamera, 0, 0.01);
				}
				glutPostRedisplay();
				bKeyAccepted = true;
				
				break;
			}
		case LITTLEM_KEY_LEFT:
			{
				if(g_pSelectedCamera != 0)
				{
					MovePosition(g_pSelectedCamera, 0, -0.01);
				}
				else if(g_pCurrentCamera != 0)
				{
					MovePosition(g_pCurrentCamera, 0, -0.01);
				}
				glutPostRedisplay();
				bKeyAccepted = true;
				
				break;
			}
		case LITTLEM_KEY_UP:
			{
				if(g_pSelectedCamera != 0)
				{
					MovePosition(g_pSelectedCamera, 1, 0.01);
				}
				else if(g_pCurrentCamera != 0)
				{
					MovePosition(g_pCurrentCamera, 1, 0.01);
				}
				glutPostRedisplay();
				bKeyAccepted = true;
				
				break;
			}
		case LITTLEM_KEY_DOWN:
			{
				if(g_pSelectedCamera != 0)
				{
					MovePosition(g_pSelectedCamera, 1, -0.01);
				}
				else if(g_pCurrentCamera != 0)
				{
					MovePosition(g_pCurrentCamera, 1, -0.01);
				}
				glutPostRedisplay();
				bKeyAccepted = true;
				
				break;
			}
		case LITTLEM_KEY_PAGE_DOWN:
			{
				if(g_pSelectedCamera != 0)
				{
					MovePosition(g_pSelectedCamera, 2, 0.01);
				}
				else if(g_pCurrentCamera != 0)
				{
					MovePosition(g_pCurrentCamera, 2, 0.01);
				}
				glutPostRedisplay();
				bKeyAccepted = true;
				
				break;
			}
		case LITTLEM_KEY_PAGE_UP:
			{
				if(g_pSelectedCamera != 0)
				{
					MovePosition(g_pSelectedCamera, 2, -0.01);
				}
				else if(g_pCurrentCamera != 0)
				{
					MovePosition(g_pCurrentCamera, 2, -0.01);
				}
				glutPostRedisplay();
				bKeyAccepted = true;
				
				break;
			}
		case LITTLEM_KEY_HOME:
			{
				if(g_pSelectedCamera != 0)
				{
					g_pSelectedCamera->m_fFieldOfView += 1.0f;
				}
				else if(g_pCurrentCamera != 0)
				{
					g_pCurrentCamera->m_fFieldOfView += 1.0f;
					vSetupProjection();
				}
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_END:
			{
				if(g_pSelectedCamera != 0)
				{
					g_pSelectedCamera->m_fFieldOfView -= 1.0f;
				}
				else if(g_pCurrentCamera != 0)
				{
					g_pCurrentCamera->m_fFieldOfView -= 1.0f;
					vSetupProjection();
				}
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_F1:
			{
				// switch to normalized front view
				vResetView(LITTLEM_VIEW_FRONT);
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_F2:
			{
				// switch to normalized left view
				vResetView(LITTLEM_VIEW_LEFT);
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_F3:
			{
				// switch to normalized right view
				vResetView(LITTLEM_VIEW_RIGHT);
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_F4:
			{
				// switch to normalized back view
				vResetView(LITTLEM_VIEW_BACK);
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_F5:
			{
				// switch to normalized top view
				vResetView(LITTLEM_VIEW_TOP);
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_F6:
			{
				// switch to normalized bottom view
				vResetView(LITTLEM_VIEW_BOTTOM);
				glutPostRedisplay();
				
				break;
			}
		}
		
		return bKeyAccepted;
	}
};

Label g_CurrentView;

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
	
	virtual bool bAcceptKey(int iKeyCode)
	{
		bool bKeyAccepted(true);
		
		switch(iKeyCode)
		{
		case LITTLEM_KEY_C:
			{
				int iModifiers(glutGetModifiers());
				
				if(iModifiers == GLUT_ACTIVE_ALT)
				{
					vToggleCullFace();
				}
				else if(iModifiers == GLUT_ACTIVE_SHIFT)
				{
					vSetKeyAcceptor(&m_CameraView);
					g_CurrentView.vSetString("Camera View");
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_D:
			{
				int iModifiers(glutGetModifiers());
				
				if(iModifiers == GLUT_ACTIVE_ALT)
				{
					vToggleDepthTest();
				}
				
				break;
			}
		case LITTLEM_KEY_E:
			{
				int iModifiers(glutGetModifiers());
				
				if(iModifiers == GLUT_ACTIVE_ALT)
				{
					std::ofstream OutputFileStream("model.xml");
					XMLStream XMLStream(OutputFileStream);
					
					XMLStream << model(g_Points, g_TrianglePoints, g_Triangles) << end;
				}
				
				break;
			}
		case LITTLEM_KEY_F:
			{
				int iModifiers(glutGetModifiers());
				
				if(iModifiers == GLUT_ACTIVE_ALT)
				{
					vToggleFrontFace();
				}
				
				break;
			}
		case LITTLEM_KEY_I:
			{
				int iModifiers(glutGetModifiers());
				
				if(iModifiers == GLUT_ACTIVE_ALT)
				{
					std::cout << "Importing model.xml." << std::endl;
					
					std::ifstream InputFileStream("model.xml");
					MeshReader MeshReader(InputFileStream);
					
					MeshReader.parse();
					
					std::vector< Point * > Points(MeshReader.GetPoints());
					std::vector< TrianglePoint * > TrianglePoints(MeshReader.GetTrianglePoints());
					std::vector< Triangle * > Triangles(MeshReader.GetTriangles());
					
					copy(Points.begin(), Points.end(), back_inserter(g_Points));
					copy(TrianglePoints.begin(), TrianglePoints.end(), back_inserter(g_TrianglePoints));
					copy(Triangles.begin(), Triangles.end(), back_inserter(g_Triangles));
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_L:
			{
				int iModifiers(glutGetModifiers());
				
				if(iModifiers == GLUT_ACTIVE_ALT)
				{
					vToggleLighting();
				}
				else if(iModifiers == GLUT_ACTIVE_SHIFT)
				{
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
					
					const std::vector< LightDescription > & LightDescriptions(SceneReader.GetLightDescriptions());
					
					for(std::vector< LightDescription >::size_type stLightDescription = 0; stLightDescription < LightDescriptions.size(); ++stLightDescription)
					{
						Light * pLight(new Light());
						
						g_Lights.push_back(pLight);
						pLight->SetPosition(LightDescriptions[stLightDescription].m_Position[0], LightDescriptions[stLightDescription].m_Position[1], LightDescriptions[stLightDescription].m_Position[2]);
						pLight->vSetDiffuseColor(LightDescriptions[stLightDescription].m_DiffuseColor[0], LightDescriptions[stLightDescription].m_DiffuseColor[1], LightDescriptions[stLightDescription].m_DiffuseColor[2], LightDescriptions[stLightDescription].m_DiffuseColor[3]);
						if(LightDescriptions[stLightDescription].m_bEnabled == true)
						{
							pLight->vEnable();
						}
					}
					
					const std::vector< CameraDescription > & CameraDescriptions(SceneReader.GetCameraDescriptions());
					
					for(std::vector< CameraDescription >::size_type stCameraDescription = 0; stCameraDescription < CameraDescriptions.size(); ++stCameraDescription)
					{
						Camera * pCamera(new Camera());
						
						pCamera->SetPosition(CameraDescriptions[stCameraDescription].Position[0], CameraDescriptions[stCameraDescription].Position[1], CameraDescriptions[stCameraDescription].Position[2]);
						pCamera->m_Orientation = CameraDescriptions[stCameraDescription].Orientation;
						pCamera->m_fFieldOfView = CameraDescriptions[stCameraDescription].FieldOfView;
						g_Cameras.push_back(pCamera);
					}
					if(g_Cameras.size() > 0)
					{
						g_pCurrentCamera = g_Cameras.front();
						vSetupProjection();
					}
					glutPostRedisplay();
				}
				else
				{
					Light * pLight(new Light());
					
					g_Lights.push_back(pLight);
					pLight->SetPosition(0.0f, 0.0f, 0.0f);
					pLight->vSetDiffuseColor(1.0f, 1.0f, 1.0f);
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_M:
			{
				if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
				{
					vSetKeyAcceptor(0);
					g_CurrentView.vSetString("Model View");
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_P:
			{
				if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
				{
					vSetKeyAcceptor(&m_PointView);
					g_CurrentView.vSetString("Point View");
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_S:
			{
				int iModifiers(glutGetModifiers());
				
				if(iModifiers == GLUT_ACTIVE_ALT)
				{
					vToggleSnapping();
				}
				else if(iModifiers == GLUT_ACTIVE_SHIFT)
				{
					std::ofstream OutputFileStream("scene.xml");
					XMLStream XMLStream(OutputFileStream);
					
					XMLStream << element << "scene" << model(g_Points, g_TrianglePoints, g_Triangles) << end;
					for(std::vector< Light * >::size_type stLight = 0; stLight < g_Lights.size(); ++stLight)
					{
						XMLStream << light(g_Lights[stLight]) << end;
					}
					for(std::vector< Camera * >::size_type stCamera = 0; stCamera < g_Cameras.size(); ++stCamera)
					{
						XMLStream << camera(g_Cameras[stCamera]) << end;
					}
				}
				
				break;
			}
		case LITTLEM_KEY_T:
			{
				if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
				{
					vSetKeyAcceptor(&m_TriangleView);
					g_CurrentView.vSetString("Triangle View");
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_ENTER:
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
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_ESCAPE:
			{
				exit(0);
				
				break;
			}
		case LITTLEM_KEY_DELETE:
			{
				if(g_pSelectedLight != 0)
				{
					vDeleteLight(g_pSelectedLight);
				}
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_PLUS:
			{
				GLfloat fPointSize;
				
				glGetFloatv(GL_POINT_SIZE, &fPointSize);
				fPointSize += 1.0f;
				glPointSize(fPointSize);
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_MINUS:
			{
				GLfloat fPointSize;
				
				glGetFloatv(GL_POINT_SIZE, &fPointSize);
				fPointSize -= 1.0f;
				glPointSize(fPointSize);
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_ASTERISK:
			{
				int iModifiers(glutGetModifiers());
				
				if(iModifiers == GLUT_ACTIVE_ALT)
				{
					vTogglePointSmooth();
				}
				
				break;
			}
		case LITTLEM_KEY_LESS_THAN:
			{
				g_fSnapFactor *= 10;
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_GREATER_THAN:
			{
				g_fSnapFactor /= 10;
				glutPostRedisplay();
				
				break;
			}
		case LITTLEM_KEY_SLASH:
			{
				if(g_OpenGLExtensions[LM_OGLE_ARB_point_parameters].bIsActivated() == true)
				{
					GLfloat fPointFadeThresholdSize(0);
					
					glGetFloatv(GL_POINT_FADE_THRESHOLD_SIZE, &fPointFadeThresholdSize);
					if(fPointFadeThresholdSize == 1.0f)
					{
						GLfloat pfPointDistanceAttenuation[3] = { 0.01f, 0.009f, 0.3f};
						
						glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, pfPointDistanceAttenuation);
						glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, 3.0f);
					}
					else
					{
						GLfloat pfPointDistanceAttenuation[3] = { 1.0f, 0.0f, 0.0f};
						
						glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, pfPointDistanceAttenuation);
						glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, 1.0f);
					}
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_LEFT_BRACKET:
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
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_RIGHT_BRACKET:
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
					glutPostRedisplay();
				}
				
				break;
			}
		case LITTLEM_KEY_RIGHT:
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 0, 0.01);
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_LEFT:
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 0, -0.01);
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_UP:
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 1, 0.01);
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_DOWN:
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 1, -0.01);
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_PAGE_DOWN:
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 2, 0.01);
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_PAGE_UP:
			{
				if(g_pSelectedLight != 0)
				{
					MovePosition(g_pSelectedLight, 2, -0.01);
					glutPostRedisplay();
					bKeyAccepted = true;
				}
				
				break;
			}
		case LITTLEM_KEY_F12:
			{
				if(g_OpenGLExtensions[LM_OGLE_ARB_point_parameters].bIsActivated() == true)
				{
					GLfloat fPointSizeMin(0.0f);
					GLfloat fPointSizeMax(0.0f);
					GLfloat pfPointDistanceAttenuation[3] = { 0.0f, 0.0f, 0.0f };
					GLfloat fPointFadeThresholdSize(0.0f);
					
					glGetFloatv(GL_POINT_SIZE_MIN, &fPointSizeMin);
					CHECK_GL_ERROR();
					glGetFloatv(GL_POINT_SIZE_MAX, &fPointSizeMax);
					CHECK_GL_ERROR();
					glGetFloatv(GL_POINT_DISTANCE_ATTENUATION, pfPointDistanceAttenuation);
					CHECK_GL_ERROR();
					glGetFloatv(GL_POINT_FADE_THRESHOLD_SIZE, &fPointFadeThresholdSize);
					CHECK_GL_ERROR();
					std::cout << "Extension \"" << g_OpenGLExtensions[LM_OGLE_ARB_point_parameters].sGetName() << "\"\n";
					std::cout << "\tGL_POINT_SIZE_MIN = " << fPointSizeMin << "\n";
					std::cout << "\tGL_POINT_SIZE_MAX = " << fPointSizeMax << "\n";
					std::cout << "\tGL_POINT_DISTANCE_ATTENUATION = [ " << pfPointDistanceAttenuation[0] << " , " << pfPointDistanceAttenuation[1] << " , " << pfPointDistanceAttenuation[2] << " ]\n";
					std::cout << "\tGL_POINT_FADE_THRESHOLD_SIZE = " << fPointFadeThresholdSize << std::endl;
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

void vKey(unsigned char cKey, int iX, int iY)
{
	if(g_UserInterface.bAcceptKey(iGetKeyCodeFromCharacter(cKey)) == false)
	{
		std::cout << "character key not accepted: " << static_cast< int >(cKey) << " ['" << cKey << "']" << std::endl;
	}
}

void vSpecialKey(int iKey, int iX, int iY)
{
	if(g_UserInterface.bAcceptKey(iGetKeyCodeFromSpecialKey(iKey)) == false)
	{
		std::cout << "special key not accepted: " << iKey << std::endl;
	}
}

void vMouse(int iButton, int iState, int iX, int iY)
{
	switch(iButton)
	{
	case GLUT_LEFT_BUTTON:
		{
			if(iState == GLUT_DOWN)
			{
				g_iMouseButtonFlags |= LITTLEM_LEFT_MOUSE_BUTTON;
			}
			else if(iState == GLUT_UP)
			{
				g_iMouseButtonFlags &= ~LITTLEM_LEFT_MOUSE_BUTTON;
				if(g_pHoveredPoint != 0)
				{
					int iModifiers(glutGetModifiers());
					
					if(iModifiers == GLUT_ACTIVE_SHIFT)
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
					int iModifiers(glutGetModifiers());
					
					if(iModifiers == GLUT_ACTIVE_SHIFT)
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
					if(g_pSelectedCamera == g_pHoveredCamera)
					{
						g_pSelectedCamera = 0;
					}
					else
					{
						g_pSelectedCamera = g_pHoveredCamera;
					}
				}
				glutPostRedisplay();
			}
			
			break;
		}
	case GLUT_MIDDLE_BUTTON:
		{
			if(iState == GLUT_DOWN)
			{
				g_iMouseButtonFlags |= LITTLEM_MIDDLE_MOUSE_BUTTON;
				g_iLastMotionX = iX;
				g_iLastMotionY = iY;
			}
			else if(iState == GLUT_UP)
			{
				g_iMouseButtonFlags &= ~LITTLEM_MIDDLE_MOUSE_BUTTON;
			}
			
			break;
		}
	case GLUT_RIGHT_BUTTON:
		{
			if(iState == GLUT_DOWN)
			{
				g_iMouseButtonFlags |= LITTLEM_RIGHT_MOUSE_BUTTON;
				g_bMoved = false;
			}
			else if(iState == GLUT_UP)
			{
				g_iMouseButtonFlags &= ~LITTLEM_RIGHT_MOUSE_BUTTON;
				if(g_bMoved == false)
				{
					g_SelectedPoints.clear();
					g_SelectedTriangles.clear();
					g_pSelectedLight = 0;
					g_pSelectedCamera = 0;
					glutPostRedisplay();
				}
			}
			
			break;
		}
	case GLUT_WHEEL_UP:
		{
			if(g_pCurrentCamera != 0)
			{
				g_pCurrentCamera->SetZ(g_pCurrentCamera->GetZ() - 0.1f);
				glutPostRedisplay();
			}
			
			break;
		}
	case GLUT_WHEEL_DOWN:
		{
			if(g_pCurrentCamera != 0)
			{
				g_pCurrentCamera->SetZ(g_pCurrentCamera->GetZ() + 0.1f);
				glutPostRedisplay();
			}
			
			break;
		}
	}
}

void vPassiveMouseMotion(int iX, int iY)
{
	g_iMouseX = iX;
	g_iMouseY = iY;
	glutPostRedisplay();
}

void vMouseMotion(int iX, int iY)
{
	g_bMoved = true;
	g_iMouseX = iX;
	g_iMouseY = iY;
	
	int iDeltaX = iX - g_iLastMotionX;
	int iDeltaY = iY - g_iLastMotionY;
	
	g_iLastMotionX = iX;
	g_iLastMotionY = iY;
	if((g_iMouseButtonFlags == LITTLEM_MIDDLE_MOUSE_BUTTON) && (g_pCurrentCamera != 0))
	{
		g_pCurrentCamera->m_Orientation.RotateX(-0.005 * iDeltaY);
		g_pCurrentCamera->m_Orientation.RotateY(-0.005 * iDeltaX);
		glutPostRedisplay();
	}
}

void vReshape(int iWidth, int iHeight)
{
	g_sWidth = iWidth;
	g_sHeight = iHeight;
	vSetupViewport();
	vSetupProjection();
	glutPostRedisplay();
}

bool bIsExtensionAvailable(const std::string & sExtensionName)
{
	if((sExtensionName != "") || (sExtensionName.find(' ') == std::string::npos))
	{
		std::string::size_type stPos = g_sExtensionString.find(sExtensionName);
		
		if((stPos != std::string::npos) && ((stPos == 0) || (g_sExtensionString[stPos - 1] == ' ')))
		{
			return true;
		}
	}
	
	return false;
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

void vInit(void)
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
	g_pCurrentCamera = pCamera;
	vResetView(LITTLEM_VIEW_FRONT);
}

int main(int argc, char ** argv)
{
	// <static-initialization>
	g_OpenGLExtensions.resize(LM_OGLE_LM_number_of_extensions);
	g_OpenGLExtensions[LM_OGLE_ARB_point_parameters].vSetName("GL_ARB_point_parameters");
	// </static-initialization>
	
	srandom(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(g_sWidth, g_sHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("littlem - the light-weight 3D model editor");
	glutFullScreen();
	vInit();
	glutDisplayFunc(vDisplay);
	glutKeyboardFunc(vKey);
	glutSpecialFunc(vSpecialKey);
	glutMouseFunc(vMouse);
	glutMotionFunc(vMouseMotion);
	glutPassiveMotionFunc(vPassiveMouseMotion);
	glutReshapeFunc(vReshape);
	vSetupUserInterface();
	g_sExtensionString = reinterpret_cast< const char * >(glGetString(GL_EXTENSIONS));
	for(std::vector< OpenGLExtension >::iterator iOpenGLExtension = g_OpenGLExtensions.begin(); iOpenGLExtension != g_OpenGLExtensions.end(); ++iOpenGLExtension)
	{
		if(bIsExtensionAvailable(iOpenGLExtension->sGetName()) == true)
		{
			iOpenGLExtension->vActivate();
		}
	}
	if((argc >= 2) && (std::string(argv[1]) == "--info"))
	{
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "Extensions: " << glGetString(GL_EXTENSIONS) << std::endl;
		std::cout << std::endl << "littleM Extension Status:" << std::endl;
		for(std::vector< OpenGLExtension >::iterator iOpenGLExtension = g_OpenGLExtensions.begin(); iOpenGLExtension != g_OpenGLExtensions.end(); ++iOpenGLExtension)
		{
			std::cout << '\t' << iOpenGLExtension->sGetName() << ": ";
			if(iOpenGLExtension->bIsActivated() == true)
			{
				std::cout << "active" << std::endl;
			}
			else
			{
				std::cout << "inactive" << std::endl;
			}
		}
	}
	else
	{
		glutMainLoop();
	}
	
	return 0;
}
