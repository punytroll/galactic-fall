/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#ifndef GRAPHICS_GL_H
#define GRAPHICS_GL_H

#include <assert.h>
#include <GL/gl.h>

#define LoadOpenGLFunction(name) { \
		assert(__ ## name == nullptr); \
		ON_DEBUG(std::cout << "  Loading function \"" << #name << "\"." << std::endl); \
		__ ## name = reinterpret_cast< name ## Function >(glXGetProcAddressARB(reinterpret_cast< const GLubyte * >( #name ))); \
		assert(__ ## name != nullptr); \
	}

#define CheckGLError assert(glGetError() == GL_NO_ERROR);

typedef void (* glBeginFunction)(GLenum mode);
extern glBeginFunction __glBegin;
#define GLBegin(mode) { assert(__glBegin != nullptr); __glBegin(mode); }

typedef void (* glBindBufferFunction)(GLenum target, GLuint buffer);
extern glBindBufferFunction __glBindBuffer;
#define GLBindBuffer(target, buffer) { assert(__glBindBuffer != nullptr); __glBindBuffer(target, buffer); CheckGLError; }

typedef void (* glBindFramebufferFunction)(GLenum target, GLuint framebuffer);
extern glBindFramebufferFunction __glBindFramebuffer;
#define GLBindFramebuffer(target, framebuffer) { assert(__glBindFramebuffer != nullptr); __glBindFramebuffer(target, framebuffer); CheckGLError; }

typedef void (* glBindRenderbufferFunction)(GLenum target, GLuint renderbuffer);
extern glBindRenderbufferFunction __glBindRenderbuffer;
#define GLBindRenderbuffer(target, renderbuffer) { assert(__glBindRenderbuffer != nullptr); __glBindRenderbuffer(target, renderbuffer); CheckGLError; }

typedef void (* glBindTextureFunction)(GLenum target, GLuint texture);
extern glBindTextureFunction __glBindTexture;
#define GLBindTexture(target, texture) { assert(__glBindTexture != nullptr); __glBindTexture(target, texture); CheckGLError; }

typedef void (* glBindVertexArrayFunction)(GLuint array);
extern glBindVertexArrayFunction __glBindVertexArray;
#define GLBindVertexArray(array) { assert(__glBindVertexArray != nullptr); __glBindVertexArray(array); CheckGLError; }

typedef void (* glBitmapFunction)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte * bitmap);
extern glBitmapFunction __glBitmap;
#define GLBitmap(width, height, xorig, yorig, xmove, ymove, bitmap) { assert(__glBitmap != nullptr); __glBitmap(width, height, xorig, yorig, xmove, ymove, bitmap); CheckGLError; }

typedef void (* glBlendFuncFunction)(GLenum sfactor, GLenum dfactor);
extern glBlendFuncFunction __glBlendFunc;
#define GLBlendFunc(sfactor, dfactor) { assert(__glBlendFunc != nullptr); __glBlendFunc(sfactor, dfactor); CheckGLError; }

typedef void (* glBufferDataFunction)(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
extern glBufferDataFunction __glBufferData;
#define GLBufferData(target, size, data, usage) { assert(__glBufferData != nullptr); __glBufferData(target, size, data, usage); CheckGLError; }

typedef void (* glCallListsFunction)(GLsizei n, GLenum type, const GLvoid * lists);
extern glCallListsFunction __glCallLists;
#define GLCallLists(n, type, lists) { assert(__glCallLists != nullptr); __glCallLists(n, type, lists); CheckGLError; }

typedef GLenum (* glCheckFramebufferStatusFunction)(GLenum target);
extern glCheckFramebufferStatusFunction __glCheckFramebufferStatus;
#ifdef NDEBUG
#define GLCheckFramebufferStatus(target) __glCheckFramebufferStatus(target); CheckGLError;
#else
inline GLenum GLCheckFramebufferStatus(GLenum target)
{
	GLenum Result;
	
	assert(__glCheckFramebufferStatus != nullptr);
	Result = __glCheckFramebufferStatus(target);
	CheckGLError;
	
	return Result;
}
#endif

typedef void (* glClearFunction)(GLbitfield mask);
extern glClearFunction __glClear;
#define GLClear(mask) { assert(__glClear != nullptr); __glClear(mask); CheckGLError; }

typedef void (* glClearColorFunction)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern glClearColorFunction __glClearColor;
#define GLClearColor(red, green, blue, alpha) { assert(__glClearColor != nullptr); __glClearColor(red, green, blue, alpha); CheckGLError; }

typedef void (* glClipPlaneFunction)(GLenum plane, const GLdouble * equation);
extern glClipPlaneFunction __glClipPlane;
#define GLClipPlane(plane, equation) { assert(__glClipPlane != nullptr); __glClipPlane(plane, equation); CheckGLError; }

typedef void (* glColor3fFunction)(GLfloat red, GLfloat green, GLfloat blue);
extern glColor3fFunction __glColor3f;
#define GLColor3f(red, green, blue) { assert(__glColor3f != nullptr); __glColor3f(red, green, blue); }

typedef void (* glColor4fFunction)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern glColor4fFunction __glColor4f;
#define GLColor4f(red, green, blue, alpha) { assert(__glColor4f != nullptr); __glColor4f(red, green, blue, alpha); }

typedef void (* glColor4fvFunction)(const GLfloat * v);
extern glColor4fvFunction __glColor4fv;
#define GLColor4fv(v) { assert(__glColor4fv != nullptr); __glColor4fv(v); }

typedef void (* glDeleteFramebuffersFunction)(GLsizei n, GLuint * framebuffers);
extern glDeleteFramebuffersFunction __glDeleteFramebuffers;
#define GLDeleteFramebuffers(n, framebuffers) { assert(__glDeleteFramebuffers != nullptr); __glDeleteFramebuffers(n, framebuffers); CheckGLError; }

typedef void (* glDeleteListsFunction)(GLuint list, GLsizei range);;
extern glDeleteListsFunction __glDeleteLists;
#define GLDeleteLists(list, range) { assert(__glDeleteLists != nullptr); __glDeleteLists(list, range); CheckGLError; }

typedef void (* glDeleteRenderbuffersFunction)(GLsizei n, GLuint * renderbuffers);
extern glDeleteRenderbuffersFunction __glDeleteRenderbuffers;
#define GLDeleteRenderbuffers(n, renderbuffers) { assert(__glDeleteRenderbuffers != nullptr); __glDeleteRenderbuffers(n, renderbuffers); CheckGLError; }

typedef void (* glDeleteTexturesFunction)(GLsizei n, const GLuint * textures);
extern glDeleteTexturesFunction __glDeleteTextures;
#define GLDeleteTextures(n, textures) { assert(__glDeleteTextures != nullptr); __glDeleteTextures(n, textures); CheckGLError; }

typedef void (* glDisableFunction)(GLenum cap);
extern glDisableFunction __glDisable;
#define GLDisable(cap) { assert(__glDisable != nullptr); __glDisable(cap); CheckGLError; }

typedef void (* glDrawArraysFunction)(GLenum mode, GLint first, GLsizei count);
extern glDrawArraysFunction __glDrawArrays;
#define GLDrawArrays(mode, first, count) { assert(__glDrawArrays != nullptr); __glDrawArrays(mode, first, count); CheckGLError; }

typedef void (* glEnableFunction)(GLenum cap);
extern glEnableFunction __glEnable;
#define GLEnable(cap) { assert(__glEnable != nullptr); __glEnable(cap); CheckGLError; }

typedef void (* glEnableVertexAttribArrayFunction)(GLuint index);
extern glEnableVertexAttribArrayFunction __glEnableVertexAttribArray;
#define GLEnableVertexAttribArray(index) { assert(__glEnableVertexAttribArray != nullptr); __glEnableVertexAttribArray(index); CheckGLError; }

typedef void (* glEndFunction)(void);
extern glEndFunction __glEnd;
#define GLEnd() { assert(__glEnd != nullptr); __glEnd(); CheckGLError; }

typedef void (* glEndListFunction)(void);
extern glEndListFunction __glEndList;
#define GLEndList() { assert(__glEndList != nullptr); __glEndList(); CheckGLError; }

typedef void (* glFramebufferRenderbufferFunction)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern glFramebufferRenderbufferFunction __glFramebufferRenderbuffer;
#define GLFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer) { assert(__glFramebufferRenderbuffer != nullptr); __glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer); CheckGLError; }

typedef void (* glFramebufferTextureFunction)(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern glFramebufferTextureFunction __glFramebufferTexture;
#define GLFramebufferTexture(target, attachment, texture, level) { assert(__glFramebufferTexture != nullptr); __glFramebufferTexture(target, attachment, texture, level); CheckGLError; }

typedef void (* glGenBuffersFunction)(GLsizei n, GLuint * buffers);
extern glGenBuffersFunction __glGenBuffers;
#define GLGenBuffers(n, buffers) { assert(__glGenBuffers != nullptr); __glGenBuffers(n, buffers); CheckGLError; }

typedef void (* glGenFramebuffersFunction)(GLsizei n, GLuint * ids);
extern glGenFramebuffersFunction __glGenFramebuffers;
#define GLGenFramebuffers(n, ids) { assert(__glGenFramebuffers != nullptr); __glGenFramebuffers(n, ids); CheckGLError; }

typedef GLuint (* glGenListsFunction)(GLsizei range);
extern glGenListsFunction __glGenLists;
#ifdef NDEBUG
#define GLGenLists(range) __glGenLists(range); CheckGLError;
#else
inline GLuint GLGenLists(GLsizei range)
{
	GLuint Result;
	
	assert(__glGenLists != nullptr);
	Result = __glGenLists(range);
	CheckGLError;
	
	return Result;
}
#endif

typedef void (* glGenRenderbuffersFunction)(GLsizei n, GLuint * renderbuffers);
extern glGenRenderbuffersFunction __glGenRenderbuffers;
#define GLGenRenderbuffers(n, renderbuffers) { assert(__glGenRenderbuffers != nullptr); __glGenRenderbuffers(n, renderbuffers); CheckGLError; }

typedef void (* glGenTexturesFunction)(GLsizei n, GLuint * textures);
extern glGenTexturesFunction __glGenTextures;
#define GLGenTextures(n, textures) { assert(__glGenTextures != nullptr); __glGenTextures(n, textures); CheckGLError; }

typedef void (* glGenVertexArraysFunction)(GLsizei n, GLuint * arrays);
extern glGenVertexArraysFunction __glGenVertexArrays;
#define GLGenVertexArrays(n, arrays) { assert(__glGenVertexArrays != nullptr); __glGenVertexArrays(n, arrays); CheckGLError; }

typedef void (* glGetIntegervFunction)(GLenum pname, GLint * params);
extern glGetIntegervFunction __glGetIntegerv;
#define GLGetIntegerv(pname, params) { assert(__glGetIntegerv != nullptr); __glGetIntegerv(pname, params); CheckGLError; }

typedef const GLubyte * (* glGetStringFunction)(GLenum name);
extern glGetStringFunction __glGetString;
#ifdef NDEBUG
#define GLGetString(name) __glGetString(name); CheckGLError;
#else
inline const GLubyte * GLGetString(GLenum name)
{
	const GLubyte * Result;
	
	assert(__glGetString != nullptr);
	Result = __glGetString(name);
	CheckGLError;
	
	return Result;
}
#endif

typedef void (* glLightfvFunction)(GLenum light, GLenum pname, const GLfloat * params);
extern glLightfvFunction __glLightfv;
#define GLLightfv(light, pname, params) { assert(__glLightfv != nullptr); __glLightfv(light, pname, params); CheckGLError; }

typedef void (* glLightModelfvFunction)(GLenum pname, const GLfloat * params);
extern glLightModelfvFunction __glLightModelfv;
#define GLLightModelfv(pname, params) { assert(__glLightModelfv != nullptr); __glLightModelfv(pname, params); CheckGLError; }

typedef void (* glListBaseFunction)(GLuint base);
extern glListBaseFunction __glListBase;
#define GLListBase(base) { assert(__glListBase != nullptr); __glListBase(base); CheckGLError; }

typedef void (* glLoadIdentityFunction)(void);
extern glLoadIdentityFunction __glLoadIdentity;
#define GLLoadIdentity() { assert(__glLoadIdentity != nullptr); __glLoadIdentity(); CheckGLError; }

typedef void (* glLoadMatrixfFunction)(const GLfloat * m);
extern glLoadMatrixfFunction __glLoadMatrixf;
#define GLLoadMatrixf(m) { assert(__glLoadMatrixf != nullptr); __glLoadMatrixf(m); CheckGLError; }

typedef void (* glMaterialfFunction)(GLenum face, GLenum pname, GLfloat param);
extern glMaterialfFunction __glMaterialf;
#define GLMaterialf(face, pname, param) { assert(__glMaterialf != nullptr); __glMaterialf(face, pname, param); }

typedef void (* glMaterialfvFunction)(GLenum face, GLenum pname, const GLfloat * params);
extern glMaterialfvFunction __glMaterialfv;
#define GLMaterialfv(face, pname, params) { assert(__glMaterialfv != nullptr); __glMaterialfv(face, pname, params); }

typedef void (* glMatrixModeFunction)(GLenum mode);
extern glMatrixModeFunction __glMatrixMode;
#define GLMatrixMode(mode) { assert(__glMatrixMode != nullptr); __glMatrixMode(mode); CheckGLError; }

typedef void (* glMultMatrixfFunction)(const GLfloat * m);
extern glMultMatrixfFunction __glMultMatrixf;
#define GLMultMatrixf(m) { assert(__glMultMatrixf != nullptr); __glMultMatrixf(m); CheckGLError; }

typedef void (* glNewListFunction)(GLuint list, GLenum mode);
extern glNewListFunction __glNewList;
#define GLNewList(list, mode) { assert(__glNewList != nullptr); __glNewList(list, mode); CheckGLError; }

typedef void (* glNormal3fvFunction)(const GLfloat * v);
extern glNormal3fvFunction __glNormal3fv;
#define GLNormal3fv(v) { assert(__glNormal3fv != nullptr); __glNormal3fv(v); }

typedef void (* glOrthoFunction)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearVal, GLdouble farVal);
extern glOrthoFunction __glOrtho;
#define GLOrtho(left, right, bottom, top, nearVal, farVal) { assert(__glOrtho != nullptr); __glOrtho(left, right, bottom, top, nearVal, farVal); CheckGLError; }

typedef void (* glPixelStoreiFunction)(GLenum pname, GLint param);
extern glPixelStoreiFunction __glPixelStorei;
#define GLPixelStorei(pname, param) { assert(__glPixelStorei != nullptr); __glPixelStorei(pname, param); CheckGLError; }

typedef void (* glPopAttribFunction)(void);
extern glPopAttribFunction __glPopAttrib;
#define GLPopAttrib() { assert(__glPopAttrib != nullptr); __glPopAttrib(); CheckGLError; }

typedef void (* glPopMatrixFunction)(void);
extern glPopMatrixFunction __glPopMatrix;
#define GLPopMatrix() { assert(__glPopMatrix != nullptr); __glPopMatrix(); CheckGLError; }

typedef void (* glPushAttribFunction)(GLbitfield mask);
extern glPushAttribFunction __glPushAttrib;
#define GLPushAttrib(mask) { assert(__glPushAttrib != nullptr); __glPushAttrib(mask); CheckGLError; };

typedef void (* glPushMatrixFunction)(void);
extern glPushMatrixFunction __glPushMatrix;
#define GLPushMatrix() { assert(__glPushMatrix != nullptr); __glPushMatrix(); CheckGLError; }

typedef void (* glRasterPos2iFunction)(GLint x, GLint y);
extern glRasterPos2iFunction __glRasterPos2i;
#define GLRasterPos2i(x, y) { assert(__glRasterPos2i != nullptr); __glRasterPos2i(x, y); CheckGLError; }

typedef void (* glReadBufferFunction)(GLenum mode);
extern glReadBufferFunction __glReadBuffer;
#define GLReadBuffer(mode) { assert(__glReadBuffer != nullptr); __glReadBuffer(mode); CheckGLError; }

typedef void (* glReadPixelsFunction)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data);
extern glReadPixelsFunction __glReadPixels;
#define GLReadPixels(x, y, width, height, format, type, data) { assert(__glReadPixels != nullptr); __glReadPixels(x, y, width, height, format, type, data); CheckGLError; }

typedef void (* glRenderbufferStorageFunction)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern glRenderbufferStorageFunction __glRenderbufferStorage;
#define GLRenderbufferStorage(target, internalformat, width, height) { assert(__glRenderbufferStorage != nullptr); __glRenderbufferStorage(target, internalformat, width, height); CheckGLError; }

typedef void (* glRotatefFunction)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern glRotatefFunction __glRotatef;
#define GLRotatef(angle, x, y, z) { assert(__glRotatef != nullptr); __glRotatef(angle, x, y, z); CheckGLError; }

typedef void (* glScalefFunction)(GLfloat x, GLfloat y, GLfloat z);
extern glScalefFunction __glScalef;
#define GLScalef(x, y, z) { assert(__glScalef != nullptr); __glScalef(x, y, z); CheckGLError; }

typedef void (* glTexCoord2fFunction)(GLfloat s, GLfloat t);
extern glTexCoord2fFunction __glTexCoord2f;
#define GLTexCoord2f(s, t) { assert(__glTexCoord2f != nullptr); __glTexCoord2f(s, t); }

typedef void (* glTexImage2DFunction)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data);
extern glTexImage2DFunction __glTexImage2D;
#define GLTexImage2D(target, level, internalFormat, width, height, border, format, type, data) { assert(__glTexImage2D != nullptr); __glTexImage2D(target, level, internalFormat, width, height, border, format, type, data); CheckGLError; }

typedef void (* glTexParameteriFunction)(GLenum target, GLenum pname, GLint param);
extern glTexParameteriFunction __glTexParameteri;
#define GLTexParameteri(target, pname, param) { assert(__glTexParameteri != nullptr); __glTexParameteri(target, pname, param); CheckGLError; }

typedef void (* glTranslatefFunction)(GLfloat x, GLfloat y, GLfloat z);
extern glTranslatefFunction __glTranslatef;
#define GLTranslatef(x, y, z) { assert(__glTranslatef != nullptr); __glTranslatef(x, y, z); CheckGLError; }

typedef void (* glVertex2fFunction)(GLfloat x, GLfloat y);
extern glVertex2fFunction __glVertex2f;
#define GLVertex2f(x, y) { assert(__glVertex2f != nullptr); __glVertex2f(x, y); }

typedef void (* glVertex2fvFunction)(const GLfloat * v);
extern glVertex2fvFunction __glVertex2fv;
#define GLVertex2fv(v) { assert(__glVertex2fv != nullptr); __glVertex2fv(v); }

typedef void (* glVertex3fFunction)(GLfloat x, GLfloat y, GLfloat z);
extern glVertex3fFunction __glVertex3f;
#define GLVertex3f(x, y, z) { assert(__glVertex3f != nullptr); __glVertex3f(x, y, z); }

typedef void (* glVertex3fvFunction)(const GLfloat * v);
extern glVertex3fvFunction __glVertex3fv;
#define GLVertex3fv(v) { assert(__glVertex3fv != nullptr); __glVertex3fv(v); }

typedef void (* glVertexAttribPointerFunction)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
extern glVertexAttribPointerFunction __glVertexAttribPointer;
#define GLVertexAttribPointer(index, size, type, normalized, stride, pointer) { assert(__glVertexAttribPointer != nullptr); __glVertexAttribPointer(index, size, type, normalized, stride, pointer); CheckGLError; }

typedef void (* glViewportFunction)(GLint x, GLint y, GLsizei width, GLsizei height);
extern glViewportFunction __glViewport;
#define GLViewport(x, y, width, height) { assert(__glViewport != nullptr); __glViewport(x, y, width, height); CheckGLError; }

#endif
