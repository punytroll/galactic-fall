#include <stdlib.h>

#include <GL/glx.h>

#include <iomanip>
#include <iostream>

int main(int argc, char ** argv)
{
	std::cout << "Opening display." << std::endl;
	
	Display * Display = XOpenDisplay(0);
	
	if(Display == 0)
	{
		std::cerr << "Coud not open the default display." << std::endl;
		exit(1);
	}
	std::cout << "Checking for GLX availability." << std::endl;
	
	int ErrorBase(0);
	int EventBase(0);
	
	if(glXQueryExtension(Display, &ErrorBase, &EventBase) == false)
	{
		std::cerr << "The GLX extension is not available." << std::endl;
		exit(1);
	}
	std::cout << "Checking for GLX version." << std::endl;
	
	int MajorVersionNumber(0);
	int MinorVersionNumber(0);
	
	if(glXQueryVersion(Display, &MajorVersionNumber, &MinorVersionNumber) == false)
	{
		std::cerr << "Checking the GLX version failed." << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "  The GLX version is " << MajorVersionNumber << "." << MinorVersionNumber << "." << std::endl;
		if((MajorVersionNumber < 1) || ((MajorVersionNumber == 1) && (MinorVersionNumber < 4)))
		{
			std::cerr << "The GLX version " << MajorVersionNumber << "." << MinorVersionNumber << " is below the minimal required version number 1.4." << std::endl;
			exit(1);
		}
	}
	std::cout << "Getting default screen." << std::endl;
	
	int ScreenNumber(DefaultScreen(Display));
	int NumberOfConfigurations(0);
	GLXFBConfig * Configurations(glXGetFBConfigs(Display, ScreenNumber, &NumberOfConfigurations));
	
	for(int ConfigurationIndex = 0; ConfigurationIndex < NumberOfConfigurations; ++ConfigurationIndex)
	{
		std::cout << "GLXFBConfig[" << ConfigurationIndex << "]:" << std::endl;
		
		int AttributeValue(0);
		
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_DRAWABLE_TYPE, &AttributeValue) == Success)
		{
			std::cout << "  GLX_DRAWABLE_TYPE: 0x" << std::hex << std::setw(8) << std::right << std::setfill('0') << AttributeValue << std::dec << std::endl;
			if((AttributeValue & GLX_WINDOW_BIT) == GLX_WINDOW_BIT)
			{
				std::cout << "     GLX_WINDOW_BIT (0x" << std::hex << std::setw(8) << std::right << std::setfill('0') << GLX_WINDOW_BIT << std::dec << ')' << std::endl;
			}
			if((AttributeValue & GLX_PIXMAP_BIT) == GLX_PIXMAP_BIT)
			{
				std::cout << "     GLX_PIXMAP_BIT (0x" << std::hex << std::setw(8) << std::right << std::setfill('0') << GLX_PIXMAP_BIT << std::dec << ')' << std::endl;
			}
			if((AttributeValue & GLX_PBUFFER_BIT) == GLX_PBUFFER_BIT)
			{
				std::cout << "     GLX_PBUFFER_BIT (0x" << std::hex << std::setw(8) << std::right << std::setfill('0') << GLX_PBUFFER_BIT << std::dec << ')' << std::endl;
			}
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_DRAWABLE_TYPE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_RENDER_TYPE, &AttributeValue) == Success)
		{
			std::cout << "  GLX_RENDER_TYPE: 0x" << std::hex << std::setw(8) << std::right << std::setfill('0') << AttributeValue << std::dec << std::endl;
			if((AttributeValue & GLX_RGBA_BIT) == GLX_RGBA_BIT)
			{
				std::cout << "     GLX_RGBA_BIT (0x" << std::hex << std::setw(8) << std::right << std::setfill('0') << GLX_RGBA_BIT << std::dec << ')' << std::endl;
			}
			if((AttributeValue & GLX_COLOR_INDEX_BIT) == GLX_COLOR_INDEX_BIT)
			{
				std::cout << "     GLX_COLOR_INDEX_BIT (0x" << std::hex << std::setw(8) << std::right << std::setfill('0') << GLX_COLOR_INDEX_BIT << std::dec << ')' << std::endl;
			}
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_RENDER_TYPE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_BUFFER_SIZE, &AttributeValue) == Success)
		{
			std::cout << "  GLX_BUFFER_SIZE: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_BUFFER_SIZE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_RED_SIZE, &AttributeValue) == Success)
		{
			std::cout << "  GLX_RED_SIZE: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_RED_SIZE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_GREEN_SIZE, &AttributeValue) == Success)
		{
			std::cout << "  GLX_GREEN_SIZE: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_GREEN_SIZE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_BLUE_SIZE, &AttributeValue) == Success)
		{
			std::cout << "  GLX_BLUE_SIZE: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_BLUE_SIZE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_ALPHA_SIZE, &AttributeValue) == Success)
		{
			std::cout << "  GLX_ALPHA_SIZE: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_ALPHA_SIZE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_DEPTH_SIZE, &AttributeValue) == Success)
		{
			std::cout << "  GLX_DEPTH_SIZE: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_DEPTH_SIZE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_STENCIL_SIZE, &AttributeValue) == Success)
		{
			std::cout << "  GLX_STENCIL_SIZE: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_STENCIL_SIZE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_DOUBLEBUFFER, &AttributeValue) == Success)
		{
			std::cout << "  GLX_DOUBLEBUFFER: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_DOUBLEBUFFER\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_SAMPLE_BUFFERS, &AttributeValue) == Success)
		{
			std::cout << "  GLX_SAMPLE_BUFFERS: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_SAMPLE_BUFFERS\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_SAMPLES, &AttributeValue) == Success)
		{
			std::cout << "  GLX_SAMPLES: " << AttributeValue << std::endl;
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_SAMPLES\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_X_RENDERABLE, &AttributeValue) == Success)
		{
			if(AttributeValue == True)
			{
				std::cout << "  GLX_X_RENDERABLE: True" << std::endl;
			}
			else
			{
				std::cout << "  GLX_X_RENDERABLE: False" << std::endl;
			}
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_X_RENDERABLE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_X_VISUAL_TYPE, &AttributeValue) == Success)
		{
			if(AttributeValue == GLX_NONE)
			{
				std::cout << "  GLX_X_VISUAL_TYPE: GLX_NONE" << std::endl;
			}
			else if(AttributeValue == GLX_TRUE_COLOR)
			{
				std::cout << "  GLX_X_VISUAL_TYPE: GLX_TRUE_COLOR" << std::endl;
			}
			else if(AttributeValue == GLX_DIRECT_COLOR)
			{
				std::cout << "  GLX_X_VISUAL_TYPE: GLX_DIRECT_COLOR" << std::endl;
			}
			else if(AttributeValue == GLX_PSEUDO_COLOR)
			{
				std::cout << "  GLX_X_VISUAL_TYPE: GLX_PSEUDO_COLOR" << std::endl;
			}
			else if(AttributeValue == GLX_STATIC_COLOR)
			{
				std::cout << "  GLX_X_VISUAL_TYPE: GLX_STATIC_COLOR" << std::endl;
			}
			else if(AttributeValue == GLX_GRAY_SCALE)
			{
				std::cout << "  GLX_X_VISUAL_TYPE: GLX_GRAY_SCALE" << std::endl;
			}
			else if(AttributeValue == GLX_STATIC_GRAY)
			{
				std::cout << "  GLX_X_VISUAL_TYPE: GLX_STATIC_GRAY" << std::endl;
			}
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_X_VISUAL_TYPE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		if(glXGetFBConfigAttrib(Display, Configurations[ConfigurationIndex], GLX_CONFIG_CAVEAT, &AttributeValue) == Success)
		{
			if(AttributeValue == GLX_NONE)
			{
				std::cout << "  GLX_CONFIG_CAVEAT: GLX_NONE" << std::endl;
			}
			else if(AttributeValue == GLX_SLOW_CONFIG)
			{
				std::cout << "  GLX_CONFIG_CAVEAT: GLX_SLOW_CONFIG" << std::endl;
			}
			else if(AttributeValue == GLX_NON_CONFORMANT_CONFIG)
			{
				std::cout << "  GLX_CONFIG_CAVEAT: GLX_NON_CONFORMANT_CONFIG" << std::endl;
			}
		}
		else
		{
			std::cerr << "Could not get attribute \"GLX_X_VISUAL_TYPE\" for GLXFBConfig[" << ConfigurationIndex << "]." << std::endl;
		}
		std::cout << std::endl;
	}
	
	return 0;
}
