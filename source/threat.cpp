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

#include <float.h>

#include "threat.h"

Reference< Object > * Threat::GetObjectWithHighestThreat(void)
{
	Reference< Object > * ObjectWithHighestThreat(0);
	float HighestThreat(FLT_MIN);
	std::deque< std::pair< Reference< Object >, float > >::iterator ThreatIterator(m_Threats.begin());
	
	while(ThreatIterator != m_Threats.end())
	{
		if(ThreatIterator->first.IsValid() == true)
		{
			if(ThreatIterator->second > HighestThreat)
			{
				ObjectWithHighestThreat = &(ThreatIterator->first);
				HighestThreat = ThreatIterator->second;
			}
			++ThreatIterator;
		}
		else
		{
			ThreatIterator = m_Threats.erase(ThreatIterator);
		}
	}
	
	return ObjectWithHighestThreat;
}

void Threat::ModifyThreat(const Reference< Object > & ThreateningObject, float DeltaThreat)
{
	std::deque< std::pair< Reference< Object >, float > >::iterator ThreatIterator(m_Threats.begin());
	
	while((ThreatIterator != m_Threats.end()) && (ThreatIterator->first != ThreateningObject))
	{
		if(ThreatIterator->first.IsValid() == true)
		{
			++ThreatIterator;
		}
		else
		{
			ThreatIterator = m_Threats.erase(ThreatIterator);
		}
	}
	if(ThreatIterator != m_Threats.end())
	{
		ThreatIterator->second += DeltaThreat;
	}
	else
	{
		m_Threats.push_back(std::make_pair(ThreateningObject, DeltaThreat));
	}
}
