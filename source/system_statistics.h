/**
 * galactic-fall
 * Copyright (C) 2008-2019  Aram Altschudjian, Hagen Möbius
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

#include <cstdint>

class SystemStatistics
{
public:
	// getters
	float GetAISecondsThisFrame(void) const;
	std::uint32_t GetCommoditiesInCurrentSystemThisFrame(void) const;
	std::uint32_t GetDispatchedMessagesThisFrame(void) const;
	float GetFramesPerSecond(void) const;
	float GetFrameToFrameSecondsThisFrame(void) const;
	float GetGraphicsSecondsThisFrame(void) const;
	float GetMessagingSecondsThisFrame(void) const;
	float GetPhysicsSecondsThisFrame(void) const;
	float GetProcessingSecondsThisFrame(void) const;
	std::uint32_t GetShipsInCurrentSystemThisFrame(void) const;
	std::uint32_t GetShotsInCurrentSystemThisFrame(void) const;
	// setters
	void SetAISecondsThisFrame(float AISecondsThisFrame);
	void SetCommoditiesInCurrentSystemThisFrame(std::uint32_t CommoditiesInCurrentSystemThisFrame);
	void SetDispatchedMessagesThisFrame(std::uint32_t DispatchedMessagesThisFrame);
	void SetFrameToFrameSecondsThisFrame(float FrameToFrameSecondsThisFrame);
	void SetGraphicsSecondsThisFrame(float GraphicsSecondsThisFrame);
	void SetMessagingSecondsThisFrame(float MessagingSecondsThisFrame);
	void SetPhysicsSecondsThisFrame(float PhysicsSecondsThisFrame);
	void SetProcessingSecondsThisFrame(float ProcessingSecondsThisFrame);
	void SetShipsInCurrentSystemThisFrame(std::uint32_t ShipsInCurrentSystemThisFrame);
	void SetShotsInCurrentSystemThisFrame(std::uint32_t ShotsInCurrentSystemThisFrame);
	// modifiers
	void NextFrame(void);
private:
	float _AISecondsThisFrame;
	std::uint32_t _CommoditiesInCurrentSystemThisFrame;
	std::uint32_t _DispatchedMessagesThisFrame;
	float _FramesPerSecond;
	float _FrameToFrameSecondsThisFrame;
	float _GraphicsSecondsThisFrame;
	float _MessageSecondsThisFrame;
	float _PhysicsSecondsThisFrame;
	float _ProcessingSecondsThisFrame;
	std::uint32_t _ShipsInCurrentSystemThisFrame;
	std::uint32_t _ShotsInCurrentSystemThisFrame;
};

inline float SystemStatistics::GetAISecondsThisFrame(void) const
{
	return _AISecondsThisFrame;
}

inline std::uint32_t SystemStatistics::GetCommoditiesInCurrentSystemThisFrame(void) const
{
	return _CommoditiesInCurrentSystemThisFrame;
}

inline std::uint32_t SystemStatistics::GetDispatchedMessagesThisFrame(void) const
{
	return _DispatchedMessagesThisFrame;
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

inline float SystemStatistics::GetPhysicsSecondsThisFrame(void) const
{
	return _PhysicsSecondsThisFrame;
}

inline float SystemStatistics::GetProcessingSecondsThisFrame(void) const
{
	return _ProcessingSecondsThisFrame;
}

inline std::uint32_t SystemStatistics::GetShipsInCurrentSystemThisFrame(void) const
{
	return _ShipsInCurrentSystemThisFrame;
}

inline std::uint32_t SystemStatistics::GetShotsInCurrentSystemThisFrame(void) const
{
	return _ShotsInCurrentSystemThisFrame;
}

inline void SystemStatistics::SetAISecondsThisFrame(float AISecondsThisFrame)
{
	_AISecondsThisFrame = AISecondsThisFrame;
}

inline void SystemStatistics::SetCommoditiesInCurrentSystemThisFrame(std::uint32_t CommoditiesInCurrentSystemThisFrame)
{
	_CommoditiesInCurrentSystemThisFrame = CommoditiesInCurrentSystemThisFrame;
}

inline void SystemStatistics::SetDispatchedMessagesThisFrame(std::uint32_t DispatchedMessagesThisFrame)
{
	_DispatchedMessagesThisFrame = DispatchedMessagesThisFrame;
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

inline void SystemStatistics::SetPhysicsSecondsThisFrame(float PhysicsSecondsThisFrame)
{
	_PhysicsSecondsThisFrame = PhysicsSecondsThisFrame;
}

inline void SystemStatistics::SetProcessingSecondsThisFrame(float ProcessingSecondsThisFrame)
{
	_ProcessingSecondsThisFrame = ProcessingSecondsThisFrame;
}

inline void SystemStatistics::SetShipsInCurrentSystemThisFrame(std::uint32_t ShipsInCurrentSystemThisFrame)
{
	_ShipsInCurrentSystemThisFrame = ShipsInCurrentSystemThisFrame;
}

inline void SystemStatistics::SetShotsInCurrentSystemThisFrame(std::uint32_t ShotsInCurrentSystemThisFrame)
{
	_ShotsInCurrentSystemThisFrame = ShotsInCurrentSystemThisFrame;
}

#endif
