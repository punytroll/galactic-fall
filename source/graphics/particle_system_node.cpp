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

#include <list>

#include "../globals.h"
#include "../system_statistics.h"
#include "engine.h"
#include "gl.h"
#include "particle_system.h"
#include "particle_system_node.h"
#include "render_context.h"
#include "scene.h"
#include "style.h"
#include "texture.h"
#include "texture_manager.h"

const Graphics::Texture * g_ParticleTexture(nullptr);

Graphics::ParticleSystemNode::ParticleSystemNode(void) :
	_ParticleSystem(nullptr)
{
	SetBlendFunction(GL_SRC_ALPHA, GL_ONE);
	SetUseBlending(true);
	SetUseDepthTest(true);
}

Graphics::ParticleSystemNode::~ParticleSystemNode(void)
{
	if(_ParticleSystem != nullptr)
	{
		_ParticleSystem->_ParticleSystemNodes.erase(this);
		_ParticleSystem = nullptr;
	}
}
	
void Graphics::ParticleSystemNode::SetParticleSystem(Graphics::ParticleSystem * ParticleSystem)
{
	assert(_ParticleSystem == nullptr);
	assert(ParticleSystem != nullptr);
	_ParticleSystem = ParticleSystem;
	_ParticleSystem->_ParticleSystemNodes.insert(this);
}

void Graphics::ParticleSystemNode::Begin(Graphics::RenderContext * RenderContext)
{
	if(g_ParticleTexture == nullptr)
	{
		g_ParticleTexture = GetScene()->GetEngine()->GetTextureManager()->Get("particle");
		assert(g_ParticleTexture != nullptr);
	}
	
	auto Style{new Graphics::Style()};
	
	RenderContext->SetStyle(Style);
	RenderContext->SetTexture(g_ParticleTexture);
	GLDepthMask(false);
	Graphics::Node::Begin(RenderContext);
}

void Graphics::ParticleSystemNode::Draw(Graphics::RenderContext * RenderContext)
{
	g_SystemStatistics->SetParticleSystemsDrawnThisFrame(g_SystemStatistics->GetParticleSystemsDrawnThisFrame() + 1);
	assert(_ParticleSystem != nullptr);
	
	if(_ParticleSystem->GetParticles().empty() == false)
	{
		GLuint VertexArray{0};
		
		GLGenVertexArrays(1, &VertexArray);
		GLBindVertexArray(VertexArray);
		
		const GLsizei NumberOfVertices{static_cast< GLsizei >(4 * _ParticleSystem->GetParticles().size())};
		float Colors[4 * NumberOfVertices];
		float Vertices[3 * NumberOfVertices];
		float TextureCoordinates[2 * NumberOfVertices];
		unsigned short Indices[NumberOfVertices + _ParticleSystem->GetParticles().size() - 1];
		int ParticleIndex{0};
		int IndexIndex{0};
		
		for(auto Particle : _ParticleSystem->GetParticles())
		{
			g_SystemStatistics->SetParticlesDrawnThisFrame(g_SystemStatistics->GetParticlesDrawnThisFrame() + 1);
			if(IndexIndex > 0)
			{
				Indices[IndexIndex++] = 0xFFFF;
			}
			Colors[ParticleIndex * 16 + 0] = Particle._Color.GetRed();
			Colors[ParticleIndex * 16 + 1] = Particle._Color.GetGreen();
			Colors[ParticleIndex * 16 + 2] = Particle._Color.GetBlue();
			Colors[ParticleIndex * 16 + 3] = Particle._Color.GetOpacity();
			Vertices[ParticleIndex * 12 + 0] = Particle._Position[0] - Particle._Size;
			Vertices[ParticleIndex * 12 + 1] = Particle._Position[1] - Particle._Size;
			Vertices[ParticleIndex * 12 + 2] = Particle._Position[2];
			TextureCoordinates[ParticleIndex * 8 + 0] = 0.0f;
			TextureCoordinates[ParticleIndex * 8 + 1] = 0.0f;
			Indices[IndexIndex++] = ParticleIndex * 4 + 0;
			Colors[ParticleIndex * 16 + 4] = Particle._Color.GetRed();
			Colors[ParticleIndex * 16 + 5] = Particle._Color.GetGreen();
			Colors[ParticleIndex * 16 + 6] = Particle._Color.GetGreen();
			Colors[ParticleIndex * 16 + 7] = Particle._Color.GetOpacity();
			Vertices[ParticleIndex * 12 + 3] = Particle._Position[0] + Particle._Size;
			Vertices[ParticleIndex * 12 + 4] = Particle._Position[1] - Particle._Size;
			Vertices[ParticleIndex * 12 + 5] = Particle._Position[2];
			TextureCoordinates[ParticleIndex * 8 + 2] = 1.0f;
			TextureCoordinates[ParticleIndex * 8 + 3] = 0.0f;
			Indices[IndexIndex++] = ParticleIndex * 4 + 1;
			Colors[ParticleIndex * 16 + 8] = Particle._Color.GetRed();
			Colors[ParticleIndex * 16 + 9] = Particle._Color.GetGreen();
			Colors[ParticleIndex * 16 + 10] = Particle._Color.GetGreen();
			Colors[ParticleIndex * 16 + 11] = Particle._Color.GetOpacity();
			Vertices[ParticleIndex * 12 + 6] = Particle._Position[0] + Particle._Size;
			Vertices[ParticleIndex * 12 + 7] = Particle._Position[1] + Particle._Size;
			Vertices[ParticleIndex * 12 + 8] = Particle._Position[2];
			TextureCoordinates[ParticleIndex * 8 + 4] = 1.0f;
			TextureCoordinates[ParticleIndex * 8 + 5] = 1.0f;
			Indices[IndexIndex++] = ParticleIndex * 4 + 2;
			Colors[ParticleIndex * 16 + 12] = Particle._Color.GetRed();
			Colors[ParticleIndex * 16 + 13] = Particle._Color.GetGreen();
			Colors[ParticleIndex * 16 + 14] = Particle._Color.GetGreen();
			Colors[ParticleIndex * 16 + 15] = Particle._Color.GetOpacity();
			Vertices[ParticleIndex * 12 + 9] = Particle._Position[0] - Particle._Size;
			Vertices[ParticleIndex * 12 + 10] = Particle._Position[1] + Particle._Size;
			Vertices[ParticleIndex * 12 + 11] = Particle._Position[2];
			TextureCoordinates[ParticleIndex * 8 + 6] = 0.0f;
			TextureCoordinates[ParticleIndex * 8 + 7] = 1.0f;
			Indices[IndexIndex++] = ParticleIndex * 4 + 3;
			ParticleIndex += 1;
		}
		
		GLuint Buffers[4];
		
		GLGenBuffers(4, Buffers);
		// index buffer
		GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[0]);
		GLBufferData(GL_ELEMENT_ARRAY_BUFFER, (NumberOfVertices + _ParticleSystem->GetParticles().size() - 1) * sizeof(unsigned short), Indices, GL_STREAM_DRAW);
		// vertex coordinates
		GLBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
		GLBufferData(GL_ARRAY_BUFFER, 3 * NumberOfVertices * sizeof(GLfloat), Vertices, GL_STREAM_DRAW);
		GLVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		GLEnableVertexAttribArray(0);
		// texture coordinates
		GLBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
		GLBufferData(GL_ARRAY_BUFFER, 2 * NumberOfVertices * sizeof(GLfloat), TextureCoordinates, GL_STREAM_DRAW);
		GLVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		GLEnableVertexAttribArray(1);
		// colors
		GLBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
		GLBufferData(GL_ARRAY_BUFFER, 4 * NumberOfVertices * sizeof(GLfloat), Colors, GL_STREAM_DRAW);
		GLVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
		GLEnableVertexAttribArray(2);
		assert(RenderContext != nullptr);
		assert(RenderContext->GetStyle() != nullptr);
		RenderContext->GetStyle()->SetProgramIdentifier("particle");
		RenderContext->ActivateProgram();
		GLEnable(GL_PRIMITIVE_RESTART);
		GLPrimitiveRestartIndex(0xFFFF);
		GLDrawElements(GL_TRIANGLE_FAN, NumberOfVertices + _ParticleSystem->GetParticles().size() - 1, GL_UNSIGNED_SHORT, NULL);
		GLBindVertexArray(0);
		GLDeleteBuffers(4, Buffers);
		GLDeleteVertexArrays(1, &VertexArray);
		RenderContext->DeactivateProgram();
	}
}

void Graphics::ParticleSystemNode::End(Graphics::RenderContext * RenderContext)
{
	Graphics::Node::End(RenderContext);
	GLDepthMask(true);
	RenderContext->SetTexture(nullptr);
	
	auto Style{RenderContext->GetStyle()};
	
	RenderContext->SetStyle(nullptr);
	delete Style;
}
