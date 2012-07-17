# coding: utf-8

'''
' Copyright (C) 2012  Hagen Möbius
' 
' This program is free software; you can redistribute it and/or
' modify it under the terms of the GNU General Public License
' as published by the Free Software Foundation; either version 2
' of the License, or (at your option) any later version.
' 
' This program is distributed in the hope that it will be useful,
' but WITHOUT ANY WARRANTY; without even the implied warranty of
' MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
' GNU General Public License for more details.
'
' You should have received a copy of the GNU General Public License
' along with this program; if not, write to the Free Software
' Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
'''

'''
' This is version 0.0.1 of the pyarx python suite.
'''

import os
import sys
from arx import Archive

def parse_arguments(arguments):
	result = dict()
	if len(arguments) == 0 or arguments[0] == "--help":
		print("Usage: arxtouch.py <arx archive> (<uii>)*")
		print("uii is a unique identifier for an item in the archive given in hexadecimal form")
		print("")
		print("No warranty, whatsoever. Copyright by Hagen Möbius, 2012")
		return None
	else:
		result["archive_file_path"] = os.path.abspath(arguments[0])
		return result

if __name__ == "__main__":
	arguments = parse_arguments(sys.argv[1:])
	if arguments != None:
		archive = Archive()
		if arguments["archive_file_path"] != None:
			archive.load(arguments["archive_file_path"]);
		archive.save(arguments["archive_file_path"])
