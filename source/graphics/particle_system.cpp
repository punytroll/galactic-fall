/**
 * galactic-fall
 * Copyright (C) 2013-2019  Hagen Möbius
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

#include <stdexcept>

#include "particle_system.h"
#include "particle_system_node.h"

Graphics::ParticleSystem::ParticleSystem(void) :
	_IsDone(false)
{
}

Graphics::ParticleSystem::~ParticleSystem(void)
{
	while(_ParticleSystemNodes.empty() == false)
	{
		(*_ParticleSystemNodes.begin())->Destroy();
	}
}

void Graphics::ParticleSystem::Update(float Seconds)
{
	for(std::vector< std::string >::const_iterator ScriptLine = _SystemScript.begin(); ScriptLine != _SystemScript.end(); ++ScriptLine)
	{
		if(*ScriptLine == "countdown")
		{
			_TimeUntilDeath -= Seconds;
		}
		else if(*ScriptLine == "kill-old")
		{
			if(_TimeUntilDeath < 0.0)
			{
				_IsDone = true;
				
				return;
			}
		}
		else if(*ScriptLine == "move")
		{
			SetPosition(GetPosition() + _Velocity.Scaled(Seconds));
		}
		else if(*ScriptLine == "update-particles")
		{
			std::list< Graphics::ParticleSystem::Particle >::iterator ParticleIterator(_Particles.begin());
			
			while(ParticleIterator != _Particles.end())
			{
				bool Forward(true);
				
				for(std::vector< std::string >::const_iterator ScriptLine = _ParticleScript.begin(); ScriptLine != _ParticleScript.end(); ++ScriptLine)
				{
					if(*ScriptLine == "countdown")
					{
						ParticleIterator->_TimeUntilDeath -= Seconds;
					}
					else if(*ScriptLine == "kill-old")
					{
						if(ParticleIterator->_TimeUntilDeath < 0.0)
						{
							ParticleIterator = _Particles.erase(ParticleIterator);
							Forward = false;
							
							break;
						}
					}
					else if(*ScriptLine == "move")
					{
						ParticleIterator->_Position.Translate(ParticleIterator->_Velocity * Seconds);
					}
					else if(*ScriptLine == "fade")
					{
						ParticleIterator->_Opacity = ParticleIterator->_TimeUntilDeath / ParticleIterator->_Lifetime;
					}
					else
					{
						throw std::runtime_error("Unknown particle command '" + *ScriptLine + "'.");
					}
				}
				if(Forward == true)
				{
					++ParticleIterator;
				}
			}
		}
		else if(*ScriptLine == "kill-empty")
		{
			if(_Particles.empty() == true)
			{
				_IsDone = true;
				
				return;
			}
		}
		else
		{
			throw std::runtime_error("Unknown particle system command '" + *ScriptLine + "'.");
		}
	}
	for(auto ParticleSystemNode : _ParticleSystemNodes)
	{
		ParticleSystemNode->SetPosition(_Position);
	}
}

void Graphics::ParticleSystem::AddParticle(const Graphics::ParticleSystem::Particle & Particle)
{
	_Particles.push_back(Particle);
}

void Graphics::ParticleSystem::AddSystemScriptLine(const std::string & SystemScriptLine)
{
	_SystemScript.push_back(SystemScriptLine);
}

void Graphics::ParticleSystem::AddParticleScriptLine(const std::string & ParticleScriptLine)
{
	_ParticleScript.push_back(ParticleScriptLine);
}
