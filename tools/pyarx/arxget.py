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

import functools
import operator
import os
import sys
from arx import Archive

def get_hexadecimal_string(number):
	return hex(number)[2:]

def get_boolean_string(boolean):
	if boolean == True:
		return "true"
	else:
		return "false"

def get_riffled_string(string_list, separator):
	if len(string_list) == 0:
		return ""
	elif len(string_list) == 1:
		return string_list[0]
	else:
		return functools.reduce(lambda x, y: x + separator + y, string_list)

def get_archive_item_identifiers(archive):
	return [item.get_identifier() for item in archive.get_items()]

def get_archive_item_identifier_strings(archive):
	return list(map(get_hexadecimal_string, get_archive_item_identifiers(archive)))

def parse_arguments(arguments):
	result = dict()
	if len(arguments) == 0 or arguments[0] == "--help":
		print("Usage: arxget <path> (<query>)*")
		print("")
		print("Examples for path and query:")
		print("  '/some/path/archive.arx'")
		print("     -> gives information about an archive at an absolute path")
		print("")
		print("  'some/path/archive.arx' 'iid=1234'")
		print("     -> gives information about a specific item in an archive with a relative path")
		print("")
		print("  '/some/path/archive.arx' 'iid=1234' 'relation=child'")
		print("     -> gives information about the \"child\" relation of an item")
		print("")
		print("  '/some/path/archive.arx' 'iid=1234' 'relation=child' '=name'")
		print("     -> gives only the name of the \"child\" relation of an item")
		print("")
		print("No warranty, whatsoever. Copyright by Hagen Möbius, 2012")
		return None
	else:
		result["archive_file_path"] = arguments[0]
		result["queries"] = dict()
		for query_parameter in arguments[1:]:
			try:
				name, value = query_parameter.split('=', 1)
				result["queries"][name] = value
			except:
				continue
		return result

def get_version_string(version):
	result = ""
	if version.get_major_number() == None and version.get_minor_number() == None and version.get_revision_number() == None and version.get_candidate_number() == None:
		return "<none>"
	else:
		return str(version.get_major_number()) + "." + str(version.get_minor_number()) + "." + str(version.get_revision_number()) + "." + str(version.get_candidate_number())

def get_archive_format_version_string(archive):
	return get_version_string(archive.get_format_version())

def get_archive_item_count_string(archive):
	return str(archive.get_number_of_items())

def get_archive_item_identifiers_string(archive):
	item_identifier_strings = get_archive_item_identifier_strings(archive)
	if len(item_identifier_strings) == 0:
		return "<none>"
	else:
		return get_riffled_string(item_identifier_strings, " ")

def get_archive_root_item_identifier_string(archive):
	root_item_identifier = archive.get_root_item_identifier()
	if root_item_identifier == None:
		return "<none>"
	else:
		return get_hexadecimal_string(root_item_identifier)

def get_item_data_compressed_length_string(item):
	compressed_length = item.get_data().get_compressed_length()
	if compressed_length == None:
		return "<none>"
	else:
		return str(compressed_length)

def get_item_data_compression_type_string(item):
	compression_type = item.get_data().get_compression_type()
	if compression_type == None:
		return "<none>"
	else:
		return str(compression_type)

def get_item_data_decompressed_length_string(item):
	return str(item.get_data().get_decompressed_length())

def get_item_data_string(item):
	return str(item.get_data().get_content())

def get_item_identifier_string(item):
	return get_hexadecimal_string(item.get_identifier())

def get_item_name_string(item):
	return "\"" + item.get_name() + "\""

def get_item_relation_count_string(item):
	return str(len(item.get_relations()))

def get_item_relation_name_strings(item):
	return [relation.get_name() for relation in item.get_relations()]

def get_item_relation_names_string(item):
	relation_name_strings = get_item_relation_name_strings(item)
	if len(relation_name_strings) == 0:
		return "<none>"
	else:
		return get_riffled_string(relation_name_strings, " ")

def get_item_sub_type_string(item):
	sub_type = item.get_sub_type()
	if sub_type == None:
		return "<none>"
	else:
		return get_hexadecimal_string(item.get_sub_type())

def get_item_type_string(item):
	type = item.get_type()
	if type == None:
		return "<none>"
	else:
		return get_hexadecimal_string(item.get_type())

def get_item_version_string(item):
	return get_version_string(item.get_version())

def get_relation_number_of_items_string(relation):
	return str(relation.get_number_of_items())

def get_relation_item_identifier_strings(relation):
	return list(map(get_hexadecimal_string, relation.get_item_identifiers()))

def get_relation_item_identifiers_string(relation):
	item_identifier_strings = get_relation_item_identifier_strings(relation)
	if len(item_identifier_strings) == 0:
		return "<none>"
	else:
		return get_riffled_string(item_identifier_strings, " ")

def get_relation_name_string(relation):
	return relation.get_name()

def print_information_from_archive(archive, queries):
	if "iid" in queries:
		item_identifier = int(queries["iid"], 16)
		item = archive.get_item_by_identifier(item_identifier)
		if item != None:
			print_information_from_item(item, queries)
		else:
			print("An item with the identifier " + get_hexadecimal_string(item_identifier) + " does not exist.")

	elif "path" in queries:
		pass
	elif "" in queries:
		if queries[""] == "format-version":
			print(get_archive_format_version_string(archive))
		elif queries[""] == "item-count":
			print(get_archive_item_count_string(archive))
		elif queries[""] == "item-identifiers":
			print(get_archive_item_identifiers_string(archive))
		elif queries[""] == "root-item-identifier":
			print(get_archive_root_item_identifier_string(archive))
		else:
			print("Unrecognized data name '" + queries[""] + "'.")
	else:
		print("format-version: " + get_archive_format_version_string(archive))
		print("item-count: " + get_archive_item_count_string(archive))
		print("item-identifiers: " + get_archive_item_identifiers_string(archive))
		print("root-item-identifier: " + get_archive_root_item_identifier_string(archive))

def print_information_from_item(item, queries):
	if "relation" in queries:
		relation_name = queries["relation"]
		relation = item.get_relation_from_name(relation_name)
		if relation != None:
			print_information_from_relation(relation, queries)
		else:
			print("A relation with the name " + relation_name + " does not exist.")
	elif "" in queries:
		if queries[""] == "identifier":
			print(get_item_identifier_string(item))
		elif queries[""] == "name":
			print(get_item_name_string(item))
		elif queries[""] == "type":
			print(get_item_type_string(item))
		elif queries[""] == "sub-type":
			print(get_item_sub_type_string(item))
		elif queries[""] == "version":
			print(get_item_version_string(item))
		elif queries[""] == "data-compression-type":
			print(get_item_data_compression_type_string(item))
		elif queries[""] == "data-compressed-length":
			print(get_item_data_compressed_length_string(item))
		elif queries[""] == "data-decompressed-length":
			print(get_item_data_decompressed_length_string(item))
		elif queries[""] == "data":
			print(get_item_data_string(item))
		elif queries[""] == "relation-count":
			print(get_item_relation_count_string(item))
		elif queries[""] == "relation-names":
			print(get_item_relation_names_string(item))
		else:
			print("Unrecognized data name '" + queries[""] + "'.")
	else:
		print("identifier: " + get_item_identifier_string(item))
		print("name: " + get_item_name_string(item))
		print("type: " + get_item_type_string(item))
		print("sub-type: " + get_item_sub_type_string(item))
		print("version: " + get_item_version_string(item))
		print("data-compression-type: " + get_item_data_compression_type_string(item))
		print("data-compressed-length: " + get_item_data_compressed_length_string(item))
		print("data-decompressed-length: " + get_item_data_decompressed_length_string(item))
		print("data: " + get_item_data_string(item))
		print("relation-count: " + get_item_relation_count_string(item))
		print("relation-names: " + get_item_relation_names_string(item))

def print_information_from_relation(relation, queries):
	if "" in queries:
		if queries[""] == "name":
			print(get_relation_name_string(relation))
		elif queries[""] == "item-count":
			print(get_relation_number_of_items_string(relation))
		elif queries[""] == "item-identifiers":
			print(get_relation_item_identifiers_string(relation))
		else:
			print("Unrecognized data name '" + queries[""] + "'.")
	else:
		print("name: " + get_relation_name_string(relation))
		print("item-count: " + get_relation_number_of_items_string(relation))
		print("item-identifiers: " + get_relation_item_identifiers_string(relation))

if __name__ == "__main__":
	arguments = parse_arguments(sys.argv[1:])
	if arguments != None:
		archive = Archive()
		if archive.load(arguments["archive_file_path"]) == True:
			print_information_from_archive(archive, arguments["queries"])
