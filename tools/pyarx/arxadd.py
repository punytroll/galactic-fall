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

from argparse import Action, ArgumentParser
from arx import *

def get_integer_from_hexadecimal_string(hexdecimal_string):
	return int(hexdecimal_string, 16)

def get_version_from_version_string(version_string):
	result = None
	parts = version_string.split(".")
	if len(parts) == 4:
		result = Version()
		result.set_major_number(get_integer_from_hexadecimal_string(parts[0]))
		result.set_minor_number(get_integer_from_hexadecimal_string(parts[1]))
		result.set_revision_number(get_integer_from_hexadecimal_string(parts[2]))
		result.set_candidate_number(get_integer_from_hexadecimal_string(parts[3]))
	return result

class HexadecimalString(Action):
	def __call__(self, parser, namespace, values, option_string=None):
		setattr(namespace, self.dest, get_integer_from_hexadecimal_string(values))

class VersionString(Action):
	def __call__(self, parser, namespace, values, option_string=None):
		version = get_version_from_version_string(values)
		if version != None:
			setattr(namespace, self.dest, version)
		else:
			raise ValueError("Version string is not valid.")

default_version = Version()
default_version.set_major_number(0)
default_version.set_minor_number(0)
default_version.set_revision_number(0)
default_version.set_candidate_number(0)
parser = ArgumentParser()
parser.add_argument("archive-file")
parser.add_argument("--data-file", dest="item-data-file", help="The items data will be read from this file.")
parser.add_argument("--identifier", dest="item-identifier", action=HexadecimalString, help="The item identifier given in hexadecimal form.")
parser.add_argument("--name", dest="item-name")
parser.add_argument("--type", dest="item-type", action=HexadecimalString, help="The item type given in hexadecimal form.")
parser.add_argument("--sub-type", dest="item-sub-type", action=HexadecimalString, help="The item sub type given in hexadecimal form.")
parser.add_argument("--version", dest="item-version", action=VersionString, help="The item version given in hexadecimal version form.")
parser.add_argument("--relation", dest="item-relations", action="append", default=[])
parser.add_argument("--root-item", dest="root-item", action="store_true")
arguments = vars(parser.parse_args())
if arguments["archive-file"] != None:
	archive = Archive()
	archive.load(arguments["archive-file"])
	item = Item()
	if arguments["item-identifier"] != None:
		item.set_identifier(arguments["item-identifier"])
	if arguments["item-name"] != None:
		item.set_name(arguments["item-name"])
	if arguments["item-type"] != None:
		item.set_type(arguments["item-type"])
	if arguments["item-sub-type"] != None:
		item.set_sub_type(arguments["item-sub-type"])
	if arguments["item-version"] != None:
		item.get_version().set_major_number(arguments["item-version"].get_major_number())
		item.get_version().set_minor_number(arguments["item-version"].get_minor_number())
		item.get_version().set_revision_number(arguments["item-version"].get_revision_number())
		item.get_version().set_candidate_number(arguments["item-version"].get_candidate_number())
	archive.register_item(item)
	for add_to_relation in arguments["item-relations"]:
		if "::" in add_to_relation:
			item_description, relation = add_to_relation.split("::")
		else:
			item_description = add_to_relation
			relation = "child"
		parent_item = None
		try:
			item_identifier = get_integer_from_hexadecimal_string(item_description)
			parent_item = archive.get_item_by_identifier(item_identifier)
		except ValueError as exception:
			parent_item = archive.get_item_by_path(item_description)
		if parent_item != None:
			parent_item.add_item_identifier(relation, item.get_identifier())
	if arguments["item-data-file"] != None:
		file = open(arguments["item-data-file"], "rb")
		item.set_decompressed_data(file.read())
		file.close()
	if arguments["root-item"] == True:
		archive.set_root_item_identifier(item.get_identifier())
	archive.save(arguments["archive-file"])
else:
	parser.error("No archive file name was given.")
