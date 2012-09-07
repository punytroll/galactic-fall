#include <stdlib.h>

#include <GL/glx.h>

#include <iomanip>
#include <iostream>
#include <sstream>

void AppendSeparated(std::string & String, const std::string & Append, const std::string & Separator)
{
	if(String.length() == 0)
	{
		String = Append;
	}
	else
	{
		String += Separator;
		String += Append;
	}
}

std::string GetCSVLine(Display * Display, GLXFBConfig & Configuration)
{
	std::stringstream Result;
	int AttributeValue;
	
	glXGetFBConfigAttrib(Display, Configuration, GLX_FBCONFIG_ID, &AttributeValue);
	Result << "0x" << std::hex << std::right << std::setw(4) << std::setfill('0') << AttributeValue << std::dec << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_BUFFER_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_LEVEL, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_DOUBLEBUFFER, &AttributeValue);
	Result << ((AttributeValue == True) ? ("True") : ("False")) << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_STEREO, &AttributeValue);
	Result << ((AttributeValue == True) ? ("True") : ("False")) << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_AUX_BUFFERS, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_RED_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_GREEN_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_BLUE_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_ALPHA_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_DEPTH_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_STENCIL_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_ACCUM_RED_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_ACCUM_GREEN_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_ACCUM_BLUE_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_ACCUM_ALPHA_SIZE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_SAMPLE_BUFFERS, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_SAMPLES, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_RENDER_TYPE, &AttributeValue);
	
	std::string RenderType;
	
	if((AttributeValue & GLX_RGBA_BIT) == GLX_RGBA_BIT)
	{
		AppendSeparated(RenderType, "RGBA", ";");
	}
	if((AttributeValue & GLX_RGBA_FLOAT_BIT_ARB) == GLX_RGBA_FLOAT_BIT_ARB)
	{
		AppendSeparated(RenderType, "RGBA float", ";");
	}
	Result << RenderType << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_DRAWABLE_TYPE, &AttributeValue);
	
	std::string DrawableType;
	
	if((AttributeValue & GLX_WINDOW_BIT) == GLX_WINDOW_BIT)
	{
		AppendSeparated(DrawableType, "Window", ";");
	}
	if((AttributeValue & GLX_PIXMAP_BIT) == GLX_PIXMAP_BIT)
	{
		AppendSeparated(DrawableType, "Pixmap", ";");
	}
	if((AttributeValue & GLX_PBUFFER_BIT) == GLX_PBUFFER_BIT)
	{
		AppendSeparated(DrawableType, "PBuffer", ";");
	}
	Result << DrawableType << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_X_RENDERABLE, &AttributeValue);
	Result << ((AttributeValue == True) ? ("True") : ("False")) << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_X_VISUAL_TYPE, &AttributeValue);
	if(AttributeValue == GLX_TRUE_COLOR)
	{
		Result << "TrueColor" << ",";
	}
	else if(AttributeValue == GLX_DIRECT_COLOR)
	{
		Result << "DirectColor" << ",";
	}
	else if(AttributeValue == GLX_PSEUDO_COLOR)
	{
		Result << "PseudoColor" << ",";
	}
	else if(AttributeValue == GLX_STATIC_COLOR)
	{
		Result << "StaticColor" << ",";
	}
	else if(AttributeValue == GLX_GRAY_SCALE)
	{
		Result << "GrayScale" << ",";
	}
	else if(AttributeValue == GLX_STATIC_GRAY)
	{
		Result << "StaticGray" << ",";
	}
	else if(AttributeValue == GLX_NONE)
	{
		Result << "None" << ",";
	}
	glXGetFBConfigAttrib(Display, Configuration, GLX_CONFIG_CAVEAT, &AttributeValue);
	if(AttributeValue == GLX_NONE)
	{
		Result << "None" << ",";
	}
	else if(AttributeValue == GLX_SLOW_CONFIG)
	{
		Result << "Slow" << ",";
	}
	else if(AttributeValue == GLX_NON_CONFORMANT_CONFIG)
	{
		Result << "Non-conformant" << ",";
	}
	glXGetFBConfigAttrib(Display, Configuration, GLX_TRANSPARENT_TYPE, &AttributeValue);
	if(AttributeValue == GLX_NONE)
	{
		Result << "None" << ",";
	}
	else if(AttributeValue == GLX_TRANSPARENT_RGB)
	{
		Result << "RGB" << ",";
	}
	else if(AttributeValue == GLX_TRANSPARENT_INDEX)
	{
		Result << "Index" << ",";
	}
	glXGetFBConfigAttrib(Display, Configuration, GLX_TRANSPARENT_INDEX_VALUE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_TRANSPARENT_RED_VALUE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_TRANSPARENT_GREEN_VALUE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_TRANSPARENT_BLUE_VALUE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_TRANSPARENT_ALPHA_VALUE, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_MAX_PBUFFER_WIDTH, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_MAX_PBUFFER_HEIGHT, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_MAX_PBUFFER_PIXELS, &AttributeValue);
	Result << AttributeValue << ",";
	glXGetFBConfigAttrib(Display, Configuration, GLX_VISUAL_ID, &AttributeValue);
	if(AttributeValue == 0)
	{
		Result << "None";
	}
	else
	{
		Result << "0x" << std::hex << std::right << std::setw(4) << std::setfill('0') << AttributeValue << std::dec;
	}
	
	return Result.str();
}

int main(int argc, char ** argv)
{
	Display * Display = XOpenDisplay(0);
	
	if(Display == 0)
	{
		std::cerr << "Coud not open the default display." << std::endl;
		exit(1);
	}
	
	int ErrorBase(0);
	int EventBase(0);
	
	if(glXQueryExtension(Display, &ErrorBase, &EventBase) == false)
	{
		std::cerr << "The GLX extension is not available." << std::endl;
		exit(1);
	}
	
	int MajorVersionNumber(0);
	int MinorVersionNumber(0);
	
	if(glXQueryVersion(Display, &MajorVersionNumber, &MinorVersionNumber) == false)
	{
		std::cerr << "Checking the GLX version failed." << std::endl;
		exit(1);
	}
	else
	{
		if((MajorVersionNumber < 1) || ((MajorVersionNumber == 1) && (MinorVersionNumber < 4)))
		{
			std::cerr << "The GLX version " << MajorVersionNumber << "." << MinorVersionNumber << " is below the minimal required version number 1.4." << std::endl;
			exit(1);
		}
	}
	
	int ScreenNumber(DefaultScreen(Display));
	int NumberOfConfigurations(0);
	unsigned int RequestedAttributes[] =
	{
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_STEREO, GLX_DONT_CARE,
		GLX_SAMPLES, GLX_DONT_CARE,
		GLX_SAMPLE_BUFFERS, GLX_DONT_CARE,
		GLX_X_RENDERABLE, True,
		GLX_TRANSPARENT_TYPE, GLX_DONT_CARE,
		GLX_STENCIL_SIZE, GLX_DONT_CARE,
		GLX_BUFFER_SIZE, 32,
		GLX_DOUBLEBUFFER, True,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_AUX_BUFFERS, GLX_DONT_CARE,
		0
	};
	GLXFBConfig * Configurations(glXChooseFBConfig(Display, ScreenNumber, reinterpret_cast< const int * >(RequestedAttributes), &NumberOfConfigurations));
	
	for(int ConfigurationIndex = 0; ConfigurationIndex < NumberOfConfigurations; ++ConfigurationIndex)
	{
		std::cout << GetCSVLine(Display, Configurations[ConfigurationIndex]) << std::endl;
	}
	
	return 0;
}
