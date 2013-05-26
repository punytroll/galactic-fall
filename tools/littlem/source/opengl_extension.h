#ifndef LITTLEM_OPENGL_EXTENSION_H
#define LITTLEM_OPENGL_EXTENSION_H

#include <string>

class OpenGLExtension
{
public:
	OpenGLExtension(void);
	void vActivate(void);
	void vDeactivate(void);
	const std::string & sGetName(void) const;
	void vSetName(const std::string & sName);
	bool bIsActivated(void) const;
private:
	std::string m_sName;
	bool m_bIsActivated;
};

#endif
