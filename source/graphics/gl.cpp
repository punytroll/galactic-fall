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

#include "gl.h"

glBindFramebufferFunction __glBindFramebuffer(0);
glBindRenderbufferFunction __glBindRenderbuffer(0);
glEnableFunction __glEnable(0);
glFramebufferRenderbufferFunction __glFramebufferRenderbuffer(0);
glFramebufferTextureFunction __glFramebufferTexture(0);
glGenFramebuffersFunction __glGenFramebuffers(0);
glGenRenderbuffersFunction __glGenRenderbuffers(0);
glGetIntegervFunction __glGetIntegerv(0);
glLightModelfvFunction __glLightModelfv(0);
glLoadIdentityFunction __glLoadIdentity(0);
glMatrixModeFunction __glMatrixMode(0);
glRenderbufferStorageFunction __glRenderbufferStorage(0);
glViewportFunction __glViewport(0);
