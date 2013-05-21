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

typedef void (* glBindFramebufferFunction)(GLenum target, GLuint framebuffer);
extern glBindFramebufferFunction __glBindFramebuffer;
#define GLBindFramebuffer(target, framebuffer) { assert(__glBindFramebuffer != 0); __glBindFramebuffer(target, framebuffer); CheckGLError; }

typedef void (* glBindRenderbufferFunction)(GLenum target, GLuint renderbuffer);
extern glBindRenderbufferFunction __glBindRenderbuffer;
#define GLBindRenderbuffer(target, renderbuffer) { assert(__glBindRenderbuffer != 0); __glBindRenderbuffer(target, renderbuffer); CheckGLError; }

typedef void (* glColor4fvFunction)(const GLfloat * v);
extern glColor4fvFunction __glColor4fv;
#define GLColor4fv(v) { assert(__glColor4fv != 0); __glColor4fv(v); }

typedef void (* glDeleteFramebuffersFunction)(GLsizei n, GLuint * framebuffers);
extern glDeleteFramebuffersFunction __glDeleteFramebuffers;
#define GLDeleteFramebuffers(n, framebuffers) { assert(__glDeleteFramebuffers != 0); __glDeleteFramebuffers(n, framebuffers); CheckGLError; }

typedef void (* glDeleteRenderbuffersFunction)(GLsizei n, GLuint * renderbuffers);
extern glDeleteRenderbuffersFunction __glDeleteRenderbuffers;
#define GLDeleteRenderbuffers(n, renderbuffers) { assert(__glDeleteRenderbuffers != 0); __glDeleteRenderbuffers(n, renderbuffers); CheckGLError; }

typedef void (* glEnableFunction)(GLenum cap);
extern glEnableFunction __glEnable;
#define GLEnable(cap) { assert(__glEnable != 0); __glEnable(cap); CheckGLError; }

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

typedef void (* glRenderbufferStorageFunction)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern glRenderbufferStorageFunction __glRenderbufferStorage;
#define GLRenderbufferStorage(target, internalformat, width, height) { assert(__glRenderbufferStorage != 0); __glRenderbufferStorage(target, internalformat, width, height); CheckGLError; }

typedef void (* glVertex2fFunction)(GLfloat x, GLfloat y);
extern glVertex2fFunction __glVertex2f;
#define GLVertex2f(x, y) { assert(__glVertex2f != 0); __glVertex2f(x, y); }

typedef void (* glVertex3fvFunction)(const GLfloat * v);
extern glVertex3fvFunction __glVertex3fv;
#define GLVertex3fv(v) { assert(__glVertex3fv != 0); __glVertex3fv(v); }

typedef void (* glViewportFunction)(GLint x, GLint y, GLsizei width, GLsizei height);
extern glViewportFunction __glViewport;
#define GLViewport(x, y, width, height) { assert(__glViewport != 0); __glViewport(x, y, width, height); CheckGLError; }
