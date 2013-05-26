#include "opengl_extension.h"

OpenGLExtension::OpenGLExtension(void) :
	m_sName(""),
	m_bIsActivated(false)
{
}

void OpenGLExtension::vActivate(void)
{
	m_bIsActivated = true;
}

void OpenGLExtension::vDeactivate(void)
{
	m_bIsActivated = false;
}

const std::string & OpenGLExtension::sGetName(void) const
{
	return m_sName;
}

void OpenGLExtension::vSetName(const std::string & sName)
{
	m_sName = sName;
}

bool OpenGLExtension::bIsActivated(void) const
{
	return m_bIsActivated;
}
