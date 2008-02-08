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
	float GetFramesPerSecond(void) const;
	float GetFrameToFrameSecondsThisFrame(void) const;
	float GetGraphicsSecondsThisFrame(void) const;
	float GetMessagingSecondsThisFrame(void) const;
	float GetPhysicsSecondsThisFrame(void) const;
	float GetProcessingSecondsThisFrame(void) const;
	u4byte GetShipsInCurrentSystemThisFrame(void) const;
	u4byte GetShotsInCurrentSystemThisFrame(void) const;
	// setters
	void SetAISecondsThisFrame(float AISecondsThisFrame);
	void SetCommoditiesInCurrentSystemThisFrame(u4byte CommoditiesInCurrentSystemThisFrame);
	void SetDispatchedMessagesThisFrame(u4byte DispatchedMessagesThisFrame);
	void SetFrameToFrameSecondsThisFrame(float FrameToFrameSecondsThisFrame);
	void SetGraphicsSecondsThisFrame(float GraphicsSecondsThisFrame);
	void SetMessagingSecondsThisFrame(float MessagingSecondsThisFrame);
	void SetPhysicsSecondsThisFrame(float PhysicsSecondsThisFrame);
	void SetProcessingSecondsThisFrame(float ProcessingSecondsThisFrame);
	void SetShipsInCurrentSystemThisFrame(u4byte ShipsInCurrentSystemThisFrame);
	void SetShotsInCurrentSystemThisFrame(u4byte ShotsInCurrentSystemThisFrame);
private:
	float m_AISecondsThisFrame;
	u4byte m_CommoditiesInCurrentSystemThisFrame;
	u4byte m_DispatchedMessagesThisFrame;
	float m_FramesPerSecond;
	float m_FrameToFrameSecondsThisFrame;
	float m_GraphicsSecondsThisFrame;
	float m_MessageSecondsThisFrame;
	float m_PhysicsSecondsThisFrame;
	float m_ProcessingSecondsThisFrame;
	u4byte m_ShipsInCurrentSystemThisFrame;
	u4byte m_ShotsInCurrentSystemThisFrame;
};

inline float SystemStatistics::GetAISecondsThisFrame(void) const
{
	return m_AISecondsThisFrame;
}

inline u4byte SystemStatistics::GetCommoditiesInCurrentSystemThisFrame(void) const
{
	return m_CommoditiesInCurrentSystemThisFrame;
}

inline u4byte SystemStatistics::GetDispatchedMessagesThisFrame(void) const
{
	return m_DispatchedMessagesThisFrame;
}

inline float SystemStatistics::GetFramesPerSecond(void) const
{
	return m_FramesPerSecond;
}

inline float SystemStatistics::GetFrameToFrameSecondsThisFrame(void) const
{
	return m_FrameToFrameSecondsThisFrame;
}

inline float SystemStatistics::GetGraphicsSecondsThisFrame(void) const
{
	return m_GraphicsSecondsThisFrame;
}

inline float SystemStatistics::GetMessagingSecondsThisFrame(void) const
{
	return m_MessageSecondsThisFrame;
}

inline float SystemStatistics::GetPhysicsSecondsThisFrame(void) const
{
	return m_PhysicsSecondsThisFrame;
}

inline float SystemStatistics::GetProcessingSecondsThisFrame(void) const
{
	return m_ProcessingSecondsThisFrame;
}

inline u4byte SystemStatistics::GetShipsInCurrentSystemThisFrame(void) const
{
	return m_ShipsInCurrentSystemThisFrame;
}

inline u4byte SystemStatistics::GetShotsInCurrentSystemThisFrame(void) const
{
	return m_ShotsInCurrentSystemThisFrame;
}

inline void SystemStatistics::SetAISecondsThisFrame(float AISecondsThisFrame)
{
	m_AISecondsThisFrame = AISecondsThisFrame;
}

inline void SystemStatistics::SetCommoditiesInCurrentSystemThisFrame(u4byte CommoditiesInCurrentSystemThisFrame)
{
	m_CommoditiesInCurrentSystemThisFrame = CommoditiesInCurrentSystemThisFrame;
}

inline void SystemStatistics::SetDispatchedMessagesThisFrame(u4byte DispatchedMessagesThisFrame)
{
	m_DispatchedMessagesThisFrame = DispatchedMessagesThisFrame;
}

inline void SystemStatistics::SetFrameToFrameSecondsThisFrame(float FrameToFrameSecondsThisFrame)
{
	m_FrameToFrameSecondsThisFrame = FrameToFrameSecondsThisFrame;
	m_FramesPerSecond = 1.0f / FrameToFrameSecondsThisFrame;
}

inline void SystemStatistics::SetGraphicsSecondsThisFrame(float GraphicsSecondsThisFrame)
{
	m_GraphicsSecondsThisFrame = GraphicsSecondsThisFrame;
}

inline void SystemStatistics::SetMessagingSecondsThisFrame(float MessagingSecondsThisFrame)
{
	m_MessageSecondsThisFrame = MessagingSecondsThisFrame;
}

inline void SystemStatistics::SetPhysicsSecondsThisFrame(float PhysicsSecondsThisFrame)
{
	m_PhysicsSecondsThisFrame = PhysicsSecondsThisFrame;
}

inline void SystemStatistics::SetProcessingSecondsThisFrame(float ProcessingSecondsThisFrame)
{
	m_ProcessingSecondsThisFrame = ProcessingSecondsThisFrame;
}

inline void SystemStatistics::SetShipsInCurrentSystemThisFrame(u4byte ShipsInCurrentSystemThisFrame)
{
	m_ShipsInCurrentSystemThisFrame = ShipsInCurrentSystemThisFrame;
}

inline void SystemStatistics::SetShotsInCurrentSystemThisFrame(u4byte ShotsInCurrentSystemThisFrame)
{
	m_ShotsInCurrentSystemThisFrame = ShotsInCurrentSystemThisFrame;
}

#endif
