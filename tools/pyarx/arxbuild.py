# coding: utf-8

'''
' Copyright (C) 2026  Hagen Möbius
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
import json

class MessageException(Exception):
    pass

def first_pass(manifest_item, archive_item, archive):
    if archive_item is None:
        archive_item = Item()
    if "name" in manifest_item:
        if archive_item.get_name() is None:
            archive_item.set_name(manifest_item["name"])
        elif archive_item.get_name() != manifest_item["name"]:
            raise MessageException(f"manifest item name {manifest_item["name"]} not found in archive")
    if "type" in manifest_item:
        if archive_item.get_type() is None:
            archive_item.set_type(manifest_item["type"])
        elif archive_item.get_type() != manifest_item["type"]:
            raise MessageException(f"manifest item type {manifest_item["type"]} not found in archive")
    if "sub_type" in manifest_item:
        if archive_item.get_sub_type() is None:
            archive_item.set_sub_type(manifest_item["sub_type"])
        elif archive_item.get_sub_type() != manifest_item["sub_type"]:
            raise MessageException(f"manifest item sub_type {manifest_item["sub_type"]} not found in archive")
    if "relations" in manifest_item:
        for manifest_relation in manifest_item["relations"]:
            if "name" in manifest_relation:
                archive_relation = archive_item.get_relation_from_name(manifest_relation["name"])
                if archive_relation is not None:
                    if "items" in manifest_relation:
                        archive_relation_items = [archive.get_item_by_identifier(item_identifier) for item_identifier in archive_relation.get_item_identifiers()]
                        archive_relation_items_by_name = {}
                        for archive_relation_item in archive_relation_items:
                            if archive_relation_item.get_name() not in archive_relation_items_by_name:
                                archive_relation_items_by_name[archive_relation_item.get_name()] = []
                            archive_relation_items_by_name[archive_relation_item.get_name()].append(archive_relation_item)
                        for manifest_relation_item in manifest_relation["items"]:
                            if "name" in manifest_relation_item:
                                if manifest_relation_item["name"] in archive_relation_items_by_name:
                                    if len(archive_relation_items_by_name[manifest_relation_item["name"]]) == 1:
                                        first_pass(manifest_relation_item, archive_relation_items_by_name[manifest_relation_item["name"]][0], archive)
                                    elif len(archive_relation_items_by_name[manifest_relation_item["name"]]) == 0:
                                        raise MessageException(f"archive relation has no items with name {manifest_relation_item["name"]}")
                                    else:
                                        raise MessageException(f"archive relation has multiple items with name {manifest_relation_item["name"]}")
                            else:
                                raise MessageException("manifest relation items must have a name")
            else:
                raise MessageException("manifest relation must have a name")
    return archive_item

def second_pass(manifest_item, archive_item, archive):
    assert archive_item is not None
    if "relations" in manifest_item:
        for manifest_relation in manifest_item["relations"]:
            if "name" in manifest_relation:
                archive_relation = archive_item.get_relation_from_name(manifest_relation["name"])
                archive_relation_items = []
                if archive_relation is not None:
                    archive_relation_items = [archive.get_item_by_identifier(item_identifier) for item_identifier in archive_relation.get_item_identifiers()]
                if "paths" in manifest_relation:
                    for manifest_relation_path in manifest_relation["paths"]:
                        manifest_relation_path_item = archive.get_item_by_path(manifest_relation_path)
                        if manifest_relation_path_item is None:
                            raise MessageException(f"archive does not contain manifest relation path {manifest_relation_path}")
                        if manifest_relation_path_item not in archive_relation_items:
                            print(f"Adding item \"{manifest_relation_path_item.get_name()}\" (from path {manifest_relation_path}\") to item \"{archive_item.get_name()}\" relation \"{manifest_relation["name"]}\".")
                            archive_item.add_item_identifier(manifest_relation["name"], manifest_relation_path_item.get_identifier())
                if "items" in manifest_relation:
                    archive_relation_items_by_name = {}
                    for archive_relation_item in archive_relation_items:
                        if archive_relation_item.get_name() not in archive_relation_items_by_name:
                            archive_relation_items_by_name[archive_relation_item.get_name()] = []
                        archive_relation_items_by_name[archive_relation_item.get_name()].append(archive_relation_item)
                    for manifest_relation_item in manifest_relation["items"]:
                        if "name" in manifest_relation_item:
                            if manifest_relation_item["name"] in archive_relation_items_by_name:
                                if len(archive_relation_items_by_name[manifest_relation_item["name"]]) == 1:
                                    second_pass(manifest_relation_item, archive_relation_items_by_name[manifest_relation_item["name"]][0], archive)
                                elif len(archive_relation_items_by_name[manifest_relation_item["name"]]) == 0:
                                    raise MessageException(f"archive relation has no items with name {manifest_relation_item["name"]}")
                                else:
                                    raise MessageException(f"archive relation has multiple items with name {manifest_relation_item["name"]}")
                        else:
                            raise MessageException("manifest relation items must have a name")

parser = ArgumentParser()
parser.add_argument("input_file")
parser.add_argument("manifest_file")
parser.add_argument("output_file")
arguments = vars(parser.parse_args())
if arguments["input_file"] != None:
    if arguments["manifest_file"] != None:
        archive = Archive()
        archive.load(arguments["input_file"])
        with open(arguments["manifest_file"]) as manifest_file:
            manifest_root_item = json.load(manifest_file)
        archive_root_item = archive.get_root_item()
        archive_root_item = first_pass(manifest_root_item, archive_root_item, archive)
        if archive.get_root_item() is None:
            archive.set_root_item(archive_root_item)
        second_pass(manifest_root_item, archive_root_item, archive)
        archive.save(arguments["output_file"])
    else:
        parser.error("No manifest file name was given.")
else:
    parser.error("No archive file name was given.")
