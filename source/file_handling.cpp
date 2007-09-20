/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#include <dirent.h>
#include <sys/stat.h>

#include "file_handling.h"

bool CreateDirectory(const std::string & Path)
{
	return mkdir(Path.c_str(), 0777) == 0;
}

std::vector< std::string > GetDirectoryEntries(const std::string & Path)
{
	std::vector< std::string > Entries;
	DIR * Directory(opendir(Path.c_str()));
	
	if(Directory != 0)
	{
		while(true)
		{
			dirent * DirectoryEntry(readdir(Directory));
			
			if(DirectoryEntry == 0)
			{
				break;
			}
			if((std::string(".") != DirectoryEntry->d_name) && (std::string("..") != DirectoryEntry->d_name))
			{
				Entries.push_back(DirectoryEntry->d_name);
			}
		}
		closedir(Directory);
	}
	
	return Entries;
}

bool IsExistingDirectory(const std::string & Path)
{
	struct stat Stat;
	
	if(stat(Path.c_str(), &Stat) == -1)
	{
		return false;
	}
	if(S_ISDIR(Stat.st_mode) == false)
	{
		return false;
	}
	
	return true;
}
