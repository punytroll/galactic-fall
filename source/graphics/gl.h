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

#include <assert.h>
#include <GL/gl.h>

#define LoadOpenGLFunction(name) { \
		assert(__ ## name == 0); \
		ON_DEBUG(std::cout << "  Loading function \"" << #name << "\"." << std::endl); \
		__ ## name = reinterpret_cast< name ## Function >(glXGetProcAddressARB(reinterpret_cast< const GLubyte * >( #name ))); \
		assert(__ ## name != 0); \
	}

#define CheckGLError assert(glGetError() == GL_NO_ERROR);

typedef void (* glBeginFunction)(GLenum mode);
extern glBeginFunction __glBegin;
#define GLBegin(mode) { assert(__glBegin != 0); __glBegin(mode); }

typedef void (* glBindFramebufferFunction)(GLenum target, GLuint framebuffer);
extern glBindFramebufferFunction __glBindFramebuffer;
#define GLBindFramebuffer(target, framebuffer) { assert(__glBindFramebuffer != 0); __glBindFramebuffer(target, framebuffer); CheckGLError; }

typedef void (* glBindRenderbufferFunction)(GLenum target, GLuint renderbuffer);
extern glBindRenderbufferFunction __glBindRenderbuffer;
#define GLBindRenderbuffer(target, renderbuffer) { assert(__glBindRenderbuffer != 0); __glBindRenderbuffer(target, renderbuffer); CheckGLError; }

typedef void (* glBitmapFunction)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte * bitmap);
extern glBitmapFunction __glBitmap;
#define GLBitmap(width, height, xorig, yorig, xmove, ymove, bitmap) { assert(__glBitmap != 0); __glBitmap(width, height, xorig, yorig, xmove, ymove, bitmap); CheckGLError; }

typedef void (* glBlendFuncFunction)(GLenum sfactor, GLenum dfactor);
extern glBlendFuncFunction __glBlendFunc;
#define GLBlendFunc(sfactor, dfactor) { assert(__glBlendFunc != 0); __glBlendFunc(sfactor, dfactor); CheckGLError; }

typedef void (* glClearFunction)(GLbitfield mask);
extern glClearFunction __glClear;
#define GLClear(mask) { assert(__glClear != 0); __glClear(mask); CheckGLError; }

typedef void (* glClearColorFunction)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern glClearColorFunction __glClearColor;
#define GLClearColor(red, green, blue, alpha) { assert(__glClearColor != 0); __glClearColor(red, green, blue, alpha); CheckGLError; }

typedef void (* glColor3fFunction)(GLfloat red, GLfloat green, GLfloat blue);
extern glColor3fFunction __glColor3f;
#define GLColor3f(red, green, blue) { assert(__glColor3f != 0); __glColor3f(red, green, blue); }

typedef void (* glColor4fFunction)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern glColor4fFunction __glColor4f;
#define GLColor4f(red, green, blue, alpha) { assert(__glColor4f != 0); __glColor4f(red, green, blue, alpha); }

typedef void (* glColor4fvFunction)(const GLfloat * v);
extern glColor4fvFunction __glColor4fv;
#define GLColor4fv(v) { assert(__glColor4fv != 0); __glColor4fv(v); }

typedef void (* glDeleteFramebuffersFunction)(GLsizei n, GLuint * framebuffers);
extern glDeleteFramebuffersFunction __glDeleteFramebuffers;
#define GLDeleteFramebuffers(n, framebuffers) { assert(__glDeleteFramebuffers != 0); __glDeleteFramebuffers(n, framebuffers); CheckGLError; }

typedef void (* glDeleteRenderbuffersFunction)(GLsizei n, GLuint * renderbuffers);
extern glDeleteRenderbuffersFunction __glDeleteRenderbuffers;
#define GLDeleteRenderbuffers(n, renderbuffers) { assert(__glDeleteRenderbuffers != 0); __glDeleteRenderbuffers(n, renderbuffers); CheckGLError; }

typedef void (* glDisableFunction)(GLenum cap);
extern glDisableFunction __glDisable;
#define GLDisable(cap) { assert(__glDisable != 0); __glDisable(cap); CheckGLError; }

typedef void (* glEnableFunction)(GLenum cap);
extern glEnableFunction __glEnable;
#define GLEnable(cap) { assert(__glEnable != 0); __glEnable(cap); CheckGLError; }

typedef void (* glEndFunction)(void);
extern glEndFunction __glEnd;
#define GLEnd() { assert(__glEnd != 0); __glEnd(); CheckGLError; }

typedef void (* glFramebufferRenderbufferFunction)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern glFramebufferRenderbufferFunction __glFramebufferRenderbuffer;
#define GLFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer) { assert(__glFramebufferRenderbuffer != 0); __glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer); CheckGLError; }

typedef void (* glFramebufferTextureFunction)(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern glFramebufferTextureFunction __glFramebufferTexture;
#define GLFramebufferTexture(target, attachment, texture, level) { assert(__glFramebufferTexture != 0); __glFramebufferTexture(target, attachment, texture, level); CheckGLError; }

typedef void (* glGenFramebuffersFunction)(GLsizei n, GLuint * ids);
extern glGenFramebuffersFunction __glGenFramebuffers;
#define GLGenFramebuffers(n, ids) { assert(__glGenFramebuffers != 0); __glGenFramebuffers(n, ids); CheckGLError; }

typedef void (* glGenRenderbuffersFunction)(GLsizei n, GLuint * renderbuffers);
extern glGenRenderbuffersFunction __glGenRenderbuffers;
#define GLGenRenderbuffers(n, renderbuffers) { assert(__glGenRenderbuffers != 0); __glGenRenderbuffers(n, renderbuffers); CheckGLError; }

typedef void (* glGetIntegervFunction)(GLenum pname, GLint * params);
extern glGetIntegervFunction __glGetIntegerv;
#define GLGetIntegerv(pname, params) { assert(__glGetIntegerv != 0); __glGetIntegerv(pname, params); CheckGLError; }

typedef void (* glLightModelfvFunction)(GLenum pname, const GLfloat * params);
extern glLightModelfvFunction __glLightModelfv;
#define GLLightModelfv(pname, params) { assert(__glLightModelfv != 0); __glLightModelfv(pname, params); CheckGLError; }

typedef void (* glLoadIdentityFunction)(void);
extern glLoadIdentityFunction __glLoadIdentity;
#define GLLoadIdentity() { assert(__glLoadIdentity != 0); __glLoadIdentity(); CheckGLError; }

typedef void (* glLoadMatrixfFunction)(const GLfloat * m);
extern glLoadMatrixfFunction __glLoadMatrixf;
#define GLLoadMatrixf(m) { assert(__glLoadMatrixf != 0); __glLoadMatrixf(m); CheckGLError; }

typedef void (* glMaterialfFunction)(GLenum face, GLenum pname, GLfloat param);
extern glMaterialfFunction __glMaterialf;
#define GLMaterialf(face, pname, param) { assert(__glMaterialf != 0); __glMaterialf(face, pname, param); }

typedef void (* glMaterialfvFunction)(GLenum face, GLenum pname, const GLfloat * params);
extern glMaterialfvFunction __glMaterialfv;
#define GLMaterialfv(face, pname, params) { assert(__glMaterialfv != 0); __glMaterialfv(face, pname, params); }

typedef void (* glMatrixModeFunction)(GLenum mode);
extern glMatrixModeFunction __glMatrixMode;
#define GLMatrixMode(mode) { assert(__glMatrixMode != 0); __glMatrixMode(mode); CheckGLError; }

typedef void (* glNormal3fvFunction)(const GLfloat * v);
extern glNormal3fvFunction __glNormal3fv;
#define GLNormal3fv(v) { assert(__glNormal3fv != 0); __glNormal3fv(v); }

typedef void (* glOrthoFunction)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearVal, GLdouble farVal);
extern glOrthoFunction __glOrtho;
#define GLOrtho(left, right, bottom, top, nearVal, farVal) { assert(__glOrtho != 0); __glOrtho(left, right, bottom, top, nearVal, farVal); CheckGLError; }

typedef void (* glPixelStoreiFunction)(GLenum pname, GLint param);
extern glPixelStoreiFunction __glPixelStorei;
#define GLPixelStorei(pname, param) { assert(__glPixelStorei != 0); __glPixelStorei(pname, param); CheckGLError; }

typedef void (* glPopAttribFunction)(void);
extern glPopAttribFunction __glPopAttrib;
#define GLPopAttrib() { assert(__glPopAttrib != 0); __glPopAttrib(); CheckGLError; }

typedef void (* glPopMatrixFunction)(void);
extern glPopMatrixFunction __glPopMatrix;
#define GLPopMatrix() { assert(__glPopMatrix != 0); __glPopMatrix(); CheckGLError; }

typedef void (* glPushAttribFunction)(GLbitfield mask);
extern glPushAttribFunction __glPushAttrib;
#define GLPushAttrib(mask) { assert(__glPushAttrib != 0); __glPushAttrib(mask); CheckGLError; };

typedef void (* glPushMatrixFunction)(void);
extern glPushMatrixFunction __glPushMatrix;
#define GLPushMatrix() { assert(__glPushMatrix != 0); __glPushMatrix(); CheckGLError; }

typedef void (* glRasterPos2iFunction)(GLint x, GLint y);
extern glRasterPos2iFunction __glRasterPos2i;
#define GLRasterPos2i(x, y) { assert(__glRasterPos2i != 0); __glRasterPos2i(x, y); CheckGLError; }

typedef void (* glReadBufferFunction)(GLenum mode);
extern glReadBufferFunction __glReadBuffer;
#define GLReadBuffer(mode) { assert(__glReadBuffer != 0); __glReadBuffer(mode); CheckGLError; }

typedef void (* glReadPixelsFunction)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data);
extern glReadPixelsFunction __glReadPixels;
#define GLReadPixels(x, y, width, height, format, type, data) { assert(__glReadPixels != 0); __glReadPixels(x, y, width, height, format, type, data); CheckGLError; }

typedef void (* glRenderbufferStorageFunction)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern glRenderbufferStorageFunction __glRenderbufferStorage;
#define GLRenderbufferStorage(target, internalformat, width, height) { assert(__glRenderbufferStorage != 0); __glRenderbufferStorage(target, internalformat, width, height); CheckGLError; }

typedef void (* glRotatefFunction)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern glRotatefFunction __glRotatef;
#define GLRotatef(angle, x, y, z) { assert(__glRotatef != 0); __glRotatef(angle, x, y, z); CheckGLError; }

typedef void (* glScalefFunction)(GLfloat x, GLfloat y, GLfloat z);
extern glScalefFunction __glScalef;
#define GLScalef(x, y, z) { assert(__glScalef != 0); __glScalef(x, y, z); CheckGLError; }

typedef void (* glTexCoord2fFunction)(GLfloat s, GLfloat t);
extern glTexCoord2fFunction __glTexCoord2f;
#define GLTexCoord2f(s, t) { assert(__glTexCoord2f != 0); __glTexCoord2f(s, t); }

typedef void (* glTexParameteriFunction)(GLenum target, GLenum pname, GLint param);
extern glTexParameteriFunction __glTexParameteri;
#define GLTexParameteri(target, pname, param) { assert(__glTexParameteri != 0); __glTexParameteri(target, pname, param); CheckGLError; }

typedef void (* glTranslatefFunction)(GLfloat x, GLfloat y, GLfloat z);
extern glTranslatefFunction __glTranslatef;
#define GLTranslatef(x, y, z) { assert(__glTranslatef != 0); __glTranslatef(x, y, z); CheckGLError; }

typedef void (* glVertex2fFunction)(GLfloat x, GLfloat y);
extern glVertex2fFunction __glVertex2f;
#define GLVertex2f(x, y) { assert(__glVertex2f != 0); __glVertex2f(x, y); }

typedef void (* glVertex2fvFunction)(const GLfloat * v);
extern glVertex2fvFunction __glVertex2fv;
#define GLVertex2fv(v) { assert(__glVertex2fv != 0); __glVertex2fv(v); }

typedef void (* glVertex3fFunction)(GLfloat x, GLfloat y, GLfloat z);
extern glVertex3fFunction __glVertex3f;
#define GLVertex3f(x, y, z) { assert(__glVertex3f != 0); __glVertex3f(x, y, z); }

typedef void (* glVertex3fvFunction)(const GLfloat * v);
extern glVertex3fvFunction __glVertex3fv;
#define GLVertex3fv(v) { assert(__glVertex3fv != 0); __glVertex3fv(v); }

typedef void (* glViewportFunction)(GLint x, GLint y, GLsizei width, GLsizei height);
extern glViewportFunction __glViewport;
#define GLViewport(x, y, width, height) { assert(__glViewport != 0); __glViewport(x, y, width, height); CheckGLError; }
