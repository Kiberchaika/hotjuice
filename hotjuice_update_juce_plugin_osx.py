#!/usr/bin/env python3

import os
import sys

from pbxproj.XcodeProject import *
from pbxproj import PBXShellScriptBuildPhase
# from pbxproj import FileOptions



class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

# loading project settings
sys.path.append(os.getcwd())
try:
    from project_settings import *
except :
    print(f"{bcolors.FAIL}Project.settings.py not found in ", os.getcwd(),f"{bcolors.ENDC}")
    print(f"{bcolors.FAIL}Please create project_settings.py with project settings as in the template{bcolors.ENDC}")
    quit()


print("working dir:", os.getcwd())
print("__file__:", __file__)
hotjuice_path = os.path.dirname(os.path.realpath(__file__))
print("hotjuice path:", os.path.dirname(os.path.realpath(__file__)))
if len(sys.argv) < 2:
    print(f"{bcolors.FAIL}please provide the name of oF plugin folder to update as the argument to this script{bcolors.ENDC}")
    # print ("please provide the name of oF plugin folder to update as the argument to this script")
    quit()
else:
    print("argument 1:", sys.argv[1])

project_path = ""

files = os.listdir(os.path.join(os.getcwd(), sys.argv[1] + "/Builds/MacOSX"))
for f in files:
    if (f[-9:] == "xcodeproj"):
        project_path = os.path.join(os.getcwd(), sys.argv[1] + "/Builds/MacOSX/" + f)
        print("found project: ", project_path)

if (project_path == ""):
    print(f"{bcolors.FAIL}didn't find JUCE made project at directory {sys.argv[1]} {bcolors.ENDC}")
    # print ("please provide the name of oF plugin folder to update as the argument to this script")
    quit()


path_to_xcode_project = project_path + "/project.pbxproj"
print(path_to_xcode_project)

project = XcodeProject.load(path_to_xcode_project)

# adding hotjuice files

group = project.get_or_create_group('hotjuice_source')

# Searching for target to compile the cpps

hotjuice_cpp_target_name = ""
for obj in project.objects.get_objects_in_section('PBXProject'):
    for t in obj.targets:
        target_name = project.objects[t].name
        if target_name.find("Shared")>=0:
            print ("target found: ", target_name)
            hotjuice_cpp_target_name = target_name



os.walk(hotjuice_path + "/src")
for root, dirs, files in os.walk(hotjuice_path + "/src", topdown=False):
   for name in files:
       # print("adding ", os.path.join(root, name), "...")
       #  if (name[-3:] == 'cpp'):
       #  print("cpp:", os.path.join(root, name))
        file_options = FileOptions(code_sign_on_copy=False, embed_framework=False, weak=True, create_build_files=True)
        project.add_file(os.path.join(root, name), file_options=file_options, parent=group, target_name=hotjuice_cpp_target_name)
        # else:
        #     print("not cpp:", os.path.join(root, name), name[-3:])

# project.add_folder(hotjuice_path + "/src", parent=group)


project.add_header_search_paths(hotjuice_path + "/src")

project.save()

# deleting pycache files
os.system('find . | grep -E "(__pycache__|\.pyc|\.pyo$)" | xargs rm -rf')

print(f"{bcolors.OKBLUE}successfully updated JUCE project {sys.argv[1]} to work with hotjuice!{bcolors.ENDC}")
