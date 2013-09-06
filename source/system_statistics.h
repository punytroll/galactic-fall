/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
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

#ifndef SYSTEM_STATISTICS_H
#define SYSTEM_STATISTICS_H

#include "type_definitions.h"

class SystemStatistics
{
public:
	// getters
	float GetAISecondsThisFrame(void) const;
	u4byte GetCommoditiesInCurrentSystemThisFrame(void) const;
	u4byte GetDispatchedMessagesThisFrame(void) const;
	float GetFontSecondsThisFrame(void) const;
	float GetFramesPerSecond(void) const;
	float GetFrameToFrameSecondsThisFrame(void) const;
	float GetGraphicsSecondsThisFrame(void) const;
	float GetMessagingSecondsThisFrame(void) const;
	u4byte GetParticleSystemsThisFrame(void) const;
	u4byte GetParticlesThisFrame(void) const;
	float GetPhysicsSecondsThisFrame(void) const;
	float GetProcessingSecondsThisFrame(void) const;
	u4byte GetShipsInCurrentSystemThisFrame(void) const;
	u4byte GetShotsInCurrentSystemThisFrame(void) const;
	// setters
	void SetAISecondsThisFrame(float AISecondsThisFrame);
	void SetCommoditiesInCurrentSystemThisFrame(u4byte CommoditiesInCurrentSystemThisFrame);
	void SetDispatchedMessagesThisFrame(u4byte DispatchedMessagesThisFrame);
	void SetFontSecondsThisFrame(float FontSecondsThisFrame);
	void SetFrameToFrameSecondsThisFrame(float FrameToFrameSecondsThisFrame);
	void SetGraphicsSecondsThisFrame(float GraphicsSecondsThisFrame);
	void SetMessagingSecondsThisFrame(float MessagingSecondsThisFrame);
	void SetParticleSystemsThisFrame(u4byte ParticleSystemsThisFrame);
	void SetParticlesThisFrame(u4byte ParticlesThisFrame);
	void SetPhysicsSecondsThisFrame(float PhysicsSecondsThisFrame);
	void SetProcessingSecondsThisFrame(float ProcessingSecondsThisFrame);
	void SetShipsInCurrentSystemThisFrame(u4byte ShipsInCurrentSystemThisFrame);
	void SetShotsInCurrentSystemThisFrame(u4byte ShotsInCurrentSystemThisFrame);
private:
	float _AISecondsThisFrame;
	u4byte _CommoditiesInCurrentSystemThisFrame;
	u4byte _DispatchedMessagesThisFrame;
	float _FontSecondsThisFrame;
	float _FramesPerSecond;
	float _FrameToFrameSecondsThisFrame;
	float _GraphicsSecondsThisFrame;
	float _MessageSecondsThisFrame;
	u4byte _ParticleSystemsThisFrame;
	u4byte _ParticlesThisFrame;
	float _PhysicsSecondsThisFrame;
	float _ProcessingSecondsThisFrame;
	u4byte _ShipsInCurrentSystemThisFrame;
	u4byte _ShotsInCurrentSystemThisFrame;
};

inline float SystemStatistics::GetAISecondsThisFrame(void) const
{
	return _AISecondsThisFrame;
}

inline u4byte SystemStatistics::GetCommoditiesInCurrentSystemThisFrame(void) const
{
	return _CommoditiesInCurrentSystemThisFrame;
}

inline u4byte SystemStatistics::GetDispatchedMessagesThisFrame(void) const
{
	return _DispatchedMessagesThisFrame;
}

inline float SystemStatistics::GetFontSecondsThisFrame(void) const
{
	return _FontSecondsThisFrame;
}

inline float SystemStatistics::GetFramesPerSecond(void) const
{
	return _FramesPerSecond;
}

inline float SystemStatistics::GetFrameToFrameSecondsThisFrame(void) const
{
	return _FrameToFrameSecondsThisFrame;
}

inline float SystemStatistics::GetGraphicsSecondsThisFrame(void) const
{
	return _GraphicsSecondsThisFrame;
}

inline float SystemStatistics::GetMessagingSecondsThisFrame(void) const
{
	return _MessageSecondsThisFrame;
}

inline u4byte SystemStatistics::GetParticleSystemsThisFrame(void) const
{
	return _ParticleSystemsThisFrame;
}

inline u4byte SystemStatistics::GetParticlesThisFrame(void) const
{
	return _ParticlesThisFrame;
}

inline float SystemStatistics::GetPhysicsSecondsThisFrame(void) const
{
	return _PhysicsSecondsThisFrame;
}

inline float SystemStatistics::GetProcessingSecondsThisFrame(void) const
{
	return _ProcessingSecondsThisFrame;
}

inline u4byte SystemStatistics::GetShipsInCurrentSystemThisFrame(void) const
{
	return _ShipsInCurrentSystemThisFrame;
}

inline u4byte SystemStatistics::GetShotsInCurrentSystemThisFrame(void) const
{
	return _ShotsInCurrentSystemThisFrame;
}

inline void SystemStatistics::SetAISecondsThisFrame(float AISecondsThisFrame)
{
	_AISecondsThisFrame = AISecondsThisFrame;
}

inline void SystemStatistics::SetCommoditiesInCurrentSystemThisFrame(u4byte CommoditiesInCurrentSystemThisFrame)
{
	_CommoditiesInCurrentSystemThisFrame = CommoditiesInCurrentSystemThisFrame;
}

inline void SystemStatistics::SetDispatchedMessagesThisFrame(u4byte DispatchedMessagesThisFrame)
{
	_DispatchedMessagesThisFrame = DispatchedMessagesThisFrame;
}

inline void SystemStatistics::SetFontSecondsThisFrame(float FontSecondsThisFrame)
{
	_FontSecondsThisFrame = FontSecondsThisFrame;
}

inline void SystemStatistics::SetFrameToFrameSecondsThisFrame(float FrameToFrameSecondsThisFrame)
{
	_FrameToFrameSecondsThisFrame = FrameToFrameSecondsThisFrame;
	_FramesPerSecond = 1.0f / FrameToFrameSecondsThisFrame;
}

inline void SystemStatistics::SetGraphicsSecondsThisFrame(float GraphicsSecondsThisFrame)
{
	_GraphicsSecondsThisFrame = GraphicsSecondsThisFrame;
}

inline void SystemStatistics::SetMessagingSecondsThisFrame(float MessagingSecondsThisFrame)
{
	_MessageSecondsThisFrame = MessagingSecondsThisFrame;
}

inline void SystemStatistics::SetParticleSystemsThisFrame(u4byte ParticleSystemsThisFrame)
{
	_ParticleSystemsThisFrame = ParticleSystemsThisFrame;
}

inline void SystemStatistics::SetParticlesThisFrame(u4byte ParticlesThisFrame)
{
	_ParticlesThisFrame = ParticlesThisFrame;
}

inline void SystemStatistics::SetPhysicsSecondsThisFrame(float PhysicsSecondsThisFrame)
{
	_PhysicsSecondsThisFrame = PhysicsSecondsThisFrame;
}

inline void SystemStatistics::SetProcessingSecondsThisFrame(float ProcessingSecondsThisFrame)
{
	_ProcessingSecondsThisFrame = ProcessingSecondsThisFrame;
}

inline void SystemStatistics::SetShipsInCurrentSystemThisFrame(u4byte ShipsInCurrentSystemThisFrame)
{
	_ShipsInCurrentSystemThisFrame = ShipsInCurrentSystemThisFrame;
}

inline void SystemStatistics::SetShotsInCurrentSystemThisFrame(u4byte ShotsInCurrentSystemThisFrame)
{
	_ShotsInCurrentSystemThisFrame = ShotsInCurrentSystemThisFrame;
}

#endif
