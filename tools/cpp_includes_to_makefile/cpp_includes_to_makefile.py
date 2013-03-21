from argparse import ArgumentParser
import os
import re
import sys

def get_variable(file_name):
	return "$(" + get_variable_name(file_name) + ")"

def get_variable_name(file_name):
	return file_name.translate(str.maketrans("./", "__"))

def get_makefile_statement(input_file_name, dependency_file_names):
	result = get_variable_name(input_file_name)
	result += " = \\\n\t"
	result += input_file_name
	for dependency_file_name in dependency_file_names:
		result += " \\\n\t"
		result += get_variable(dependency_file_name)
	result += "\n"
	return result

def is_sorted(list, compare = lambda x, y: x <= y):
	for index, element in enumerate(list[1:]):
		if compare(list[index], element) == False:
			return False
	return True

parser = ArgumentParser()
parser.add_argument("input-file")
arguments = vars(parser.parse_args())
if arguments["input-file"] != None:
	input_file_path = arguments["input-file"]
	input_file_directory = os.path.dirname(input_file_path)
	include_pattern = re.compile("^#include \"([/_\.a-z]+)\"$")
	dependency_file_names = list()
	for line in open(input_file_path):
		match = include_pattern.match(line)
		if match != None:
			dependency_file_path = os.path.normpath(os.path.join(input_file_directory, match.group(1)))
			dependency_file_names.append(dependency_file_path)
	sys.stdout.write(get_makefile_statement(input_file_path, sorted(dependency_file_names)))
