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
	// setters
	void SetAISecondsPerFrame(float AISecondsPerFrame)
	{
		m_AISecondsPerFrame = AISecondsPerFrame;
	}
	
	void SetDispatchedMessagesPerFrame(u4byte DispatchedMessagesPerFrame)
	{
		m_DispatchedMessagesPerFrame = DispatchedMessagesPerFrame;
	}
	
	void SetGraphicsSecondsPerFrame(float GraphicsSecondsPerFrame)
	{
		m_GraphicsSecondsPerFrame = GraphicsSecondsPerFrame;
	}
	
	void SetMessagingSecondsPerFrame(float MessagingSecondsPerFrame)
	{
		m_MessageSecondsPerFrame = MessagingSecondsPerFrame;
	}
	
	void SetPhysicsSecondsPerFrame(float PhysicsSecondsPerFrame)
	{
		m_PhysicsSecondsPerFrame = PhysicsSecondsPerFrame;
	}
	
	void SetTotalSecondsPerFrame(float TotalSecondsPerFrame)
	{
		m_TotalSecondsPerFrame = TotalSecondsPerFrame;
		m_FramesPerSecond = 1.0f / TotalSecondsPerFrame;
	}
	
	void SetTotalSecondsPerFrameProcessing(float TotalSecondsPerFrameProcessing)
	{
		m_TotalSecondsPerFrameProcessing = TotalSecondsPerFrameProcessing;
	}
	
	// getters
	float GetFramesPerSecond(void) const
	{
		return m_FramesPerSecond;
	}
	
	float GetAISecondsPerFrame(void) const
	{
		return m_AISecondsPerFrame;
	}
	
	u4byte GetDispatchedMessagesPerFrame(void) const
	{
		return m_DispatchedMessagesPerFrame;
	}
	
	float GetGraphicsSecondsPerFrame(void) const
	{
		return m_GraphicsSecondsPerFrame;
	}
	
	float GetMessagingSecondsPerFrame(void) const
	{
		return m_MessageSecondsPerFrame;
	}
	
	float GetPhysicsSecondsPerFrame(void) const
	{
		return m_PhysicsSecondsPerFrame;
	}
	
	float GetTotalSecondsPerFrame(void) const
	{
		return m_TotalSecondsPerFrame;
	}
	
	float GetTotalSecondsPerFrameProcessing(void) const
	{
		return m_TotalSecondsPerFrameProcessing;
	}
private:
	float m_FramesPerSecond;
	float m_AISecondsPerFrame;
	u4byte m_DispatchedMessagesPerFrame;
	float m_GraphicsSecondsPerFrame;
	float m_MessageSecondsPerFrame;
	float m_PhysicsSecondsPerFrame;
	float m_TotalSecondsPerFrame;
	float m_TotalSecondsPerFrameProcessing;
};

#endif
