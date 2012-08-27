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

from random import randint
from struct import pack, unpack

class InvalidOperation(Exception):
	pass

class Data(object):
	def __init__(self):
		self.__content = None
		self.__compression_type = None
		self.__compressed_length = None
		self.__decompressed_length = None
	
	def get_content(self):
		assert isinstance(self.__content, bytes) == True
		return self.__content
	
	def get_compressed_length(self):
		assert isinstance(self.__compressed_length, int) == True or self.__compressed_length == None
		return self.__compressed_length
	
	def get_compression_type(self):
		assert isinstance(self.__compression_type, int) == True or self.__compression_type == None
		return self.__compression_type
	
	def get_decompressed_length(self):
		assert isinstance(self.__decompressed_length, int) == True
		return self.__decompressed_length
	
	def has_content(self):
		return self.__content != None
	
	def is_compressed(self):
		return self.__compression_type != None
	
	def set_decompressed_content(self, decompressed_content):
		assert isinstance(decompressed_content, bytes) == True
		self.__content = decompressed_content
		self.__decompressed_length = len(decompressed_content)
		self.__compressed_length = None
		self.__compression_type = None
	
	def set_compressed_content(self, compressed_content, compression_type, decompressed_length):
		assert isinstance(compressed_content, bytes) == True
		self.__content = compressed_content
		self.__decompressed_length = decompressed_length
		self.__compressed_length = len(compressed_content)
		self.__compression_type = compression_type

class Version(object):
	def __init__(self):
		self.__major_number = None
		self.__minor_number = None
		self.__revision_number = None
		self.__candidate_number = None
	
	def get_major_number(self):
		assert isinstance(self.__major_number, int) == True or self.__major_number == None
		return self.__major_number
	
	def get_minor_number(self):
		assert isinstance(self.__minor_number, int) == True or self.__minor_number == None
		return self.__minor_number
	
	def get_revision_number(self):
		assert isinstance(self.__revision_number, int) == True or self.__revision_number == None
		return self.__revision_number
	
	def get_candidate_number(self):
		assert isinstance(self.__candidate_number, int) == True or self.__candidate_number == None
		return self.__candidate_number
	
	def set_major_number(self, major_number):
		assert isinstance(major_number, int) == True or major_number == None
		self.__major_number = major_number
	
	def set_minor_number(self, minor_number):
		assert isinstance(minor_number, int) == True or minor_number == None
		self.__minor_number = minor_number
	
	def set_revision_number(self, revision_number):
		assert isinstance(revision_number, int) == True or revision_number == None
		self.__revision_number = revision_number
	
	def set_candidate_number(self, candidate_number):
		assert isinstance(candidate_number, int) == True or candidate_number == None
		self.__candidate_number = candidate_number

class Relation(object):
	def __init__(self):
		self.__item_identifiers = list()
		self.__name = None
	
	def add_item_identifier(self, item_identifier):
		assert isinstance(self.__item_identifiers, list) == True
		assert isinstance(item_identifier, int) == True
		self.__item_identifiers.append(item_identifier)
	
	def get_item_identifiers(self):
		assert isinstance(self.__item_identifiers, list) == True
		return self.__item_identifiers
	
	def get_name(self):
		assert isinstance(self.__name, str) == True
		return self.__name
	
	def get_number_of_items(self):
		assert isinstance(self.__item_identifiers, list) == True
		return len(self.__item_identifiers)
	
	def set_name(self, name):
		assert isinstance(name, str) == True
		self.__name = name

class Item(object):
	def __init__(self):
		self.__archive = None
		self.__data = Data()
		self.__identifier = None
		self.__name = None
		self.__relations = dict()
		self.__sub_type = None
		self.__type = None
		self.__version = Version()
	
	def add_item_identifier(self, relation_name, item_identifier):
		assert isinstance(self.__relations, dict) == True and isinstance(relation_name, str) == True and isinstance(item_identifier, int) == True
		if relation_name not in self.__relations:
			relation = Relation()
			relation.set_name(relation_name)
			self.__relations[relation_name] = relation
		else:
			relation = self.__relations[relation_name]
		relation.add_item_identifier(item_identifier)
	
	def get_archive(self):
		return self.__archive
	
	def get_data(self):
		assert isinstance(self.__data, Data) == True
		return self.__data
	
	def get_identifier(self):
		return self.__identifier
	
	def get_name(self):
		assert isinstance(self.__name, str) == True or self.__name == None
		return self.__name
	
	def get_relation_from_name(self, relation_name):
		assert isinstance(self.__relations, dict) == True
		assert isinstance(relation_name, str) == True
		if relation_name in self.__relations:
			return self.__relations[relation_name]
		else:
			return None
	
	def get_relations(self):
		assert isinstance(self.__relations, dict) == True
		return self.__relations.values()
	
	def get_sub_type(self):
		assert (isinstance(self.__sub_type, int) == True) or (self.__sub_type == None)
		return self.__sub_type
	
	def get_type(self):
		assert (isinstance(self.__type, int) == True) or (self.__type == None)
		return self.__type
	
	def get_version(self):
		assert isinstance(self.__version, Version) == True
		return self.__version
	
	def set_archive(self, archive):
		assert archive == None or isinstance(archive, Archive) == True
		if archive == None:
			if self.__archive != None:
				self.__archive.unregister_item(self)
				self.__archive = None
			else:
				raise InvalidOperation("This item is not registered at any archive.")
		else:
			if self.__archive == None:
				self.__archive = archive
				archive.register_item(self)
			else:
				raise InvalidOperation("This item is already registered at an archive.")
	
	def set_compressed_data(self, compressed_data, compression_type):
		assert isinstance(compressed_data, bytes) == True
		assert isinstance(compression_type, int) == True
		self.__data.set_compressed_content(compressed_data, compression_type)
	
	def set_decompressed_data(self, decompressed_data):
		assert isinstance(decompressed_data, bytes) == True
		self.__data.set_decompressed_content(decompressed_data)
	
	def set_identifier(self, identifier):
		assert isinstance(identifier, int) == True
		if self.__archive == None:
			self.__identifier = identifier
		elif identifier != None:
			archive = self.__archive
			self.__archive.unregister_item(self)
			self.__identifier = identifier
			self.__archive = archive
			self.__archive.register_item(self)
		else:
			raise InvalidOperation("This item is registered at an archive and must have an identifier.")
	
	def set_name(self, name):
		assert isinstance(name, str) == True or name == None
		self.__name = name
	
	def set_sub_type(self, sub_type):
		assert ((isinstance(sub_type, int) == True) and (sub_type >= 0) and (sub_type < 0xffffffff)) or (sub_type == None)
		self.__sub_type = sub_type
	
	def set_type(self, type):
		assert ((isinstance(type, int) == True) and (type >= 0) and (type < 0xffffffff)) or (type == None)
		self.__type = type

class Archive(object):
	def __init__(self):
		self.__items = dict()
		self.__root_item_identifier = None
		self.__format_version = Version()
	
	def load(self, file_path):
		with open(file_path, "rb") as file:
			archive_header = file.read(12)
			major_number, minor_number, revision_number, candidate_number, root_item_identifier, number_of_items = unpack("!BBBBII", archive_header)
			self.__format_version.set_major_number(major_number)
			self.__format_version.set_minor_number(minor_number)
			self.__format_version.set_revision_number(revision_number)
			self.__format_version.set_candidate_number(candidate_number)
			if self.__format_version.get_major_number() == 2 and self.__format_version.get_minor_number() == 1 and self.__format_version.get_revision_number() == 0 and self.__format_version.get_candidate_number() == 0:
				self.__load_2_1_0_0(file, number_of_items)
			else:
				print("Could not load an archive of format version " + str(self.__format_version.get_major_number()) + "." + str(self.__format_version.get_minor_number()) + "." + str(self.__format_version.get_revision_number()) + "." + str(self.__format_version.get_candidate_number()) + ".")
				return False
			if root_item_identifier == 0xffffffff:
				self.__root_item_identifier = None
			else:
				self.__root_item_identifier = root_item_identifier
		return True
	
	def __load_2_1_0_0(self, file, number_of_archive_items):
		while number_of_archive_items > 0:
			number_of_archive_items -= 1
			item_header = file.read(36)
			identifier, type, sub_type, major_number, minor_number, revision_number, candidate_number, data_compression_type, name_length, data_decompressed_length, data_compressed_length, structure_length = unpack("!IIIBBBBIIIII", item_header)
			if identifier not in self.__items:
				item = Item()
				item.set_identifier(identifier)
				if type == 0xffffffff:
					item.set_type(None)
				else:
					item.set_type(type)
				if sub_type == 0xffffffff:
					item.set_sub_type(None)
				else:
					item.set_sub_type(sub_type)
				if major_number == 0 and minor_number == 0 and revision_number == 0 and candidate_number == 0:
					item.get_version().set_major_number(None)
					item.get_version().set_minor_number(None)
					item.get_version().set_revision_number(None)
					item.get_version().set_candidate_number(None)
				else:
					item.get_version().set_major_number(major_number)
					item.get_version().set_minor_number(minor_number)
					item.get_version().set_revision_number(revision_number)
					item.get_version().set_candidate_number(candidate_number)
				name = file.read(name_length).decode()
				item.set_name(name)
				file.read(1)
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
				if data_compression_type == 0:
					item.set_decompressed_data(file.read(data_decompressed_length))
				else:
					item.set_compressed_data(file.read(data_compressed_length), data_compression_type, data_decompressed_length)
				self.register_item(item)
	
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
		assert isinstance(self.__items, dict) == True
		if item_identifier in self.__items:
			return self.__items[item_identifier]
		return None
	
	def get_item_by_path(self, item_path):
		assert isinstance(self.__items, dict) == True
		assert isinstance(item_path, str) == True
		if self.__root_item_identifier != None:
			if len(item_path) == 0:
				return self.get_item_by_identifier(self.__root_item_identifier)
			elif item_path[0] == "/":
				path = self.__parse_path(item_path)[1:-1]
				item = self.get_item_by_identifier(self.__root_item_identifier)
				while item != None and len(path) > 0:
					part = path.pop(0)
					relation = item.get_relation_from_name(part[0])
					if relation != None:
						for child_identifier in relation.get_item_identifiers():
							child = self.get_item_by_identifier(child_identifier)
							candidate = None
							if child != None and child.get_name() == part[1]:
								candidate = child
								break
						if candidate != None:
							item = candidate
						else:
							return None
					else:
						return None
				return item
			else:
				return None
		else:
			return None
	
	def __parse_path(self, path):
		assert isinstance(path, str) == True
		result = list()
		parts = path.split("/")
		parts = [part.split("::") if "::" in part else [part, "child"] for part in parts]
		result.append([None, parts[0][0]])
		last_relation = None
		for part in parts:
			if last_relation != None:
				result.append([last_relation, part[0]])
			last_relation = part[1]
		result.append([last_relation, None])
		return result
	
	def get_items(self):
		assert isinstance(self.__items, dict) == True
		return self.__items.values()
	
	def __get_new_item_identifier(self):
		result = randint(0, 0xffffffff)
		while result in self.__items:
			result = randint(0, 0xffffffff)
		return result
	
	def get_number_of_items(self):
		assert isinstance(self.__items, dict) == True
		return len(self.__items)
	
	def get_root_item_identifier(self):
		assert self.__root_item_identifier == None or isinstance(self.__root_item_identifier, int) == True
		return self.__root_item_identifier
	
	def get_format_version(self):
		assert isinstance(self.__format_version, Version)
		return self.__format_version
	
	def has_item_by_identifier(self, item_identifier):
		assert isinstance(self.__items, dict) == True
		return item_identifier in self.__items
	
	def register_item(self, item):
		assert isinstance(item, Item) == True
		assert isinstance(self.__items, dict) == True
		if item.get_archive() == None:
			if item.get_identifier() == None:
				item.set_identifier(self.__get_new_item_identifier())
				item.set_archive(self)
			elif item.get_identifier() not in self.__items:
				item.set_archive(self)
			else:
				raise InvalidOperation("This item identifier is already registered at this archive.")
		elif item.get_archive() == self:
			assert item.get_identifier() != None
			if item.get_identifier() not in self.__items:
				self.__items[item.get_identifier()] = item
			else:
				raise InvalidOperation("This item is already registered at this archive.")
		else:
			raise InvalidOperation("This item is already registered at another archive.")
	
	def save(self, file_path):
		with open(file_path, "wb") as file:
			self.__save_2_1_0_0(file)
	
	def __save_2_1_0_0(self, file):
		file.write(pack("!BBBB", 2, 1, 0, 0))
		if self.__root_item_identifier != None:
			file.write(pack("!I", self.__root_item_identifier))
		else:
			file.write(pack("!I", 0xffffffff))
		file.write(pack("!I", self.get_number_of_items()))
		for item in self.__items.values():
			file.write(pack("!I", item.get_identifier()))
			if item.get_type() != None:
				file.write(pack("!I", item.get_type()))
			else:
				file.write(pack("!I", 0xffffffff))
			if item.get_sub_type() != None:
				file.write(pack("!I", item.get_sub_type()))
			else:
				file.write(pack("!I", 0xffffffff))
			if item.get_version().get_major_number() != None:
				file.write(pack("!B", item.get_version().get_major_number()))
			else:
				file.write(pack("!B", 0x00))
			if item.get_version().get_minor_number() != None:
				file.write(pack("!B", item.get_version().get_minor_number()))
			else:
				file.write(pack("!B", 0x00))
			if item.get_version().get_revision_number() != None:
				file.write(pack("!B", item.get_version().get_revision_number()))
			else:
				file.write(pack("!B", 0x00))
			if item.get_version().get_candidate_number() != None:
				file.write(pack("!B", item.get_version().get_candidate_number()))
			else:
				file.write(pack("!B", 0x00))
			if item.get_data().is_compressed() == True:
				file.write(pack("!I", item.get_data().get_compression_type()))
			else:
				file.write(pack("!I", 0))
			if item.get_name() != None:
				name = item.get_name().encode()
			else:
				name = "".encode()
			file.write(pack("!I", len(name)))
			if item.get_data().has_content() == True:
				file.write(pack("!I", item.get_data().get_decompressed_length()))
				if item.get_data().is_compressed() == True:
					file.write(pack("!I", item.get_data().get_compressed_length()))
				else:
					file.write(pack("!I", 0))
			else:
				file.write(pack("!II", 0, 0))
			structure = bytes()
			for relation in item.get_relations():
				structure += relation.get_name().encode()
				structure += b"\x00"
				structure += pack("!I", relation.get_number_of_items())
				for item_identifier in relation.get_item_identifiers():
					structure += pack("!I", item_identifier)
			file.write(pack("!I", len(structure)))
			file.write(name)
			file.write(b"\x00")
			file.write(structure)
			if item.get_data().has_content() == True:
				file.write(item.get_data().get_content())
	
	def unregister_item(self, item):
		assert isinstance(item, Item) == True
		assert item.get_identifier() != None
		if item.get_archive() == self:
			if item.get_identifier() in self.__items:
				del self.__items[item.get_identifier()]
				item.set_archive(None)
		else:
			raise InvalidOperation()
	
	def set_root_item_identifier(self, root_item_identifier):
		assert isinstance(root_item_identifier, int) == True
		self.__root_item_identifier = root_item_identifier

if __name__ == "__main__":
	archive = Archive()
	item = Item()
	archive.register_item(item)
	assert item.get_identifier() != None
	identifier = item.get_identifier()
	assert item.get_archive() == archive
	assert archive.get_number_of_items() == 1
	assert archive.has_item_by_identifier(item.get_identifier()) == True
	assert archive.get_item_by_identifier(item.get_identifier()) == item
	archive.unregister_item(item)
	assert item.get_identifier() == identifier
	assert item.get_archive() == None
	assert archive.get_number_of_items() == 0
	assert archive.has_item_by_identifier(item.get_identifier()) == False
	assert archive.get_item_by_identifier(item.get_identifier()) == None
	item.set_archive(archive)
	assert item.get_archive() == archive
	assert item.get_identifier() == identifier
	assert archive.get_number_of_items() == 1
	assert archive.has_item_by_identifier(item.get_identifier()) == True
	assert archive.get_item_by_identifier(item.get_identifier()) == item
	item.set_identifier(12)
	identifier = 12
	assert item.get_archive() == archive
	assert item.get_identifier() == identifier
	assert archive.get_number_of_items() == 1
	assert archive.has_item_by_identifier(item.get_identifier()) == True
	assert archive.get_item_by_identifier(item.get_identifier()) == item
	item.set_archive(None)
	assert item.get_identifier() == identifier
	assert item.get_archive() == None
	assert archive.get_number_of_items() == 0
	assert archive.has_item_by_identifier(item.get_identifier()) == False
	assert archive.get_item_by_identifier(item.get_identifier()) == None
	archive.register_item(item)
	assert item.get_archive() == archive
	assert item.get_identifier() == identifier
	assert archive.get_number_of_items() == 1
	assert archive.has_item_by_identifier(item.get_identifier()) == True
	assert archive.get_item_by_identifier(item.get_identifier()) == item
	archive.register_item(item)
