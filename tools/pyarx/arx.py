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

from struct import unpack
from weakref import ref

class Data(object):
	def __init__(self):
		self.__content = None
		self.__compression_type = None
		self.__compressed_length = None
		self.__decompressed_length = None
	
	def get_content(self):
		assert isinstance(self.__content, bytes)
		return self.__content
	
	def get_compressed_length(self):
		assert isinstance(self.__compressed_length, int)
		return self.__compressed_length
	
	def get_compression_type(self):
		assert isinstance(self.__compression_type, int)
		return self.__compression_type
	
	def get_decompressed_length(self):
		assert isinstance(self.__decompressed_length, int)
		return self.__decompressed_length
	
	def set_content(self, content):
		assert isinstance(content, bytes)
		self.__content = content
	
	def set_compressed_length(self, compressed_length):
		assert isinstance(compressed_length, int)
		self.__compressed_length = compressed_length
	
	def set_compression_type(self, compression_type):
		assert isinstance(compression_type, int)
		self.__compression_type = compression_type
	
	def set_decompressed_length(self, decompressed_length):
		assert isinstance(decompressed_length, int)
		self.__decompressed_length = decompressed_length

class Version(object):
	def __init__(self):
		self.__major_number = None
		self.__minor_number = None
		self.__revision_number = None
		self.__candidate_number = None
	
	def get_major_number(self):
		assert isinstance(self.__major_number, int)
		return self.__major_number
	
	def get_minor_number(self):
		assert isinstance(self.__minor_number, int)
		return self.__minor_number
	
	def get_revision_number(self):
		assert isinstance(self.__revision_number, int)
		return self.__revision_number
	
	def get_candidate_number(self):
		assert isinstance(self.__candidate_number, int)
		return self.__candidate_number
	
	def set_major_number(self, major_number):
		assert isinstance(major_number, int) == True
		self.__major_number = major_number
	
	def set_minor_number(self, minor_number):
		assert isinstance(minor_number, int) == True
		self.__minor_number = minor_number
	
	def set_revision_number(self, revision_number):
		assert isinstance(revision_number, int) == True
		self.__revision_number = revision_number
	
	def set_candidate_number(self, candidate_number):
		assert isinstance(candidate_number, int) == True
		self.__candidate_number = candidate_number

class Relation(object):
	def __init__(self):
		self.__item_identifiers = list()
		self.__name = None
	
	def add_item_identifier(self, item_identifier):
		assert isinstance(self.__item_identifiers, list)
		assert isinstance(item_identifier, int)
		self.__item_identifiers.append(item_identifier)
	
	def get_item_count(self):
		assert isinstance(self.__item_identifiers, list)
		return len(self.__item_identifiers)
	
	def get_item_identifiers(self):
		assert isinstance(self.__item_identifiers, list)
		return self.__item_identifiers
	
	def get_name(self):
		assert isinstance(self.__name, str)
		return self.__name
	
	def set_name(self, name):
		assert isinstance(name, str)
		self.__name = name

class Item(object):
	def __init__(self):
		self.__data = Data()
		self.__identifier = None
		self.__name = None
		self.__relations = dict()
		self.__sub_type = None
		self.__type = None
		self.__version = Version()
	
	def add_item_identifier(self, relation_name, item_identifier):
		assert isinstance(self.__relations, dict) and isinstance(relation_name, str) and isinstance(item_identifier, int)
		if relation_name not in self.__relations:
			relation = Relation()
			relation.set_name(relation_name)
			self.__relations[relation_name] = relation
		else:
			relation = self.__relations[relation_name]
		relation.add_item_identifier(item_identifier)
	
	def get_data(self):
		assert isinstance(self.__data, Data)
		return self.__data
	
	def get_identifier(self):
		assert isinstance(self.__identifier, int)
		return self.__identifier
	
	def get_name(self):
		assert isinstance(self.__name, str)
		return self.__name
	
	def get_relation_from_name(self, relation_name):
		assert isinstance(self.__relations, dict)
		assert isinstance(relation_name, str)
		if relation_name in self.__relations:
			return self.__relations[relation_name]
		else:
			return None
	
	def get_relations(self):
		assert isinstance(self.__relations, dict)
		return self.__relations.values()
	
	def get_sub_type(self):
		assert isinstance(self.__sub_type, int)
		return self.__sub_type
	
	def get_type(self):
		assert isinstance(self.__type, int)
		return self.__type
	
	def get_version(self):
		assert isinstance(self.__version, Version)
		return self.__version
	
	def set_identifier(self, identifier):
		assert isinstance(identifier, int)
		self.__identifier = identifier
	
	def set_name(self, name):
		assert isinstance(name, str)
		self.__name = name
	
	def set_sub_type(self, sub_type):
		assert isinstance(sub_type, int)
		self.__sub_type = sub_type
	
	def set_type(self, type):
		assert isinstance(type, int)
		self.__type = type

class Archive(object):
	def __init__(self):
		self.__items = list()
		self.__root_item_identifier = None
		self.__format_version = Version()
	
	def load(self, file_path):
		with open(file_path, "rb") as file:
			archive_header = file.read(12)
			major_number, minor_number, revision_number, candidate_number, root_item_identifier, number_of_items = unpack("!BBBBiI", archive_header)
			self.__format_version.set_major_number(major_number)
			self.__format_version.set_minor_number(minor_number)
			self.__format_version.set_revision_number(revision_number)
			self.__format_version.set_candidate_number(candidate_number)
			if self.__format_version.get_major_number() == 2 and self.__format_version.get_minor_number() == 1 and self.__format_version.get_revision_number() == 0 and self.__format_version.get_candidate_number() == 0:
				self.__load_2_1_0_0(file, number_of_items)
			else:
				print("Could not load an archive of format version " + str(self.__format_version.get_major_number()) + "." + str(self.__format_version.get_minor_number()) + "." + str(self.__format_version.get_revision_number()) + "." + str(self.__format_version.get_candidate_number()) + ".")
				return False
			if root_item_identifier == -1:
				self.__root_item_identifier = None
			else:
				self.__root_item_identifier = root_item_identifier
		return True
	
	def __load_2_1_0_0(self, file, number_of_archive_items):
		while number_of_archive_items > 0:
			number_of_archive_items -= 1
			item_header = file.read(36)
			identifier, type, sub_type, major_number, minor_number, revision_number, candidate_number, data_compression_type, name_length, data_decompressed_length, data_compressed_length, structure_length = unpack("!iiiBBBBIIIII", item_header)
			item = Item()
			item.set_identifier(identifier)
			item.set_type(type)
			item.set_sub_type(sub_type)
			item.get_version().set_major_number(major_number)
			item.get_version().set_minor_number(minor_number)
			item.get_version().set_revision_number(revision_number)
			item.get_version().set_candidate_number(candidate_number)
			name = file.read(name_length + 1).decode()
			item.set_name(name)
			while structure_length > 0:
				relation_name, byte_length = self.__read_string_from_file(file)
				structure_length -= byte_length
				relation_header = file.read(4)
				structure_length -= 4
				number_of_relation_items = unpack("!I", relation_header)[0]
				while number_of_relation_items > 0:
					raw_item_identifier = file.read(4)
					structure_length -= 4
					number_of_relation_items -= 1
					item_identifier = unpack("!I", raw_item_identifier)[0]
					item.add_item_identifier(relation_name, item_identifier)
			structure = file.read(structure_length)
			if data_compression_type == 0:
				data = file.read(data_decompressed_length)
			else:
				data = file.read(data_compressed_length)
			item.get_data().set_compression_type(data_compression_type)
			item.get_data().set_compressed_length(data_compressed_length)
			item.get_data().set_decompressed_length(data_decompressed_length)
			item.get_data().set_content(data)
			self.__items.append(item)
	
	def __read_string_from_file(self, file):
		length = 0
		byte = file.read(1)
		raw_name = bytes()
		while byte != b'\x00':
			length += 1
			raw_name += byte
			byte = file.read(1)
		length += 1
		return (raw_name.decode(), length)
	
	def get_item_by_identifier(self, item_identifier):
		for item in self.__items:
			if item.get_identifier() == item_identifier:
				return item
		return None
	
	def get_items(self):
		assert isinstance(self.__items, list)
		return self.__items
	
	def get_number_of_items(self):
		assert isinstance(self.__items, list)
		return len(self.__items)
	
	def get_root_item_identifier(self):
		assert self.__root_item_identifier == None or isinstance(self.__root_item_identifier, int)
		return self.__root_item_identifier
	
	def get_format_version(self):
		assert isinstance(self.__format_version, Version)
		return self.__format_version
	
	def has_item_by_identifier(self, item_identifier):
		for item in self.__items:
			if item.get_identifier() == item_identifier:
				return True
		return False
	
	def save(self, file_path):
		pass
