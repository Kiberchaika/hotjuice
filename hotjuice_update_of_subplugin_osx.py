#!/usr/bin/env python3

import os
import sys

print("python path:", sys.executable)

from pbxproj import XcodeProject
from pbxproj import PBXShellScriptBuildPhase


# loading project settings
sys.path.append(os.getcwd())
try:
    from project_settings import *
except :
    print("project.settings.py not found in ", os.getcwd())
    print("please create project_settings.py with project settings as in the template")
    quit()


print("working dir:", os.getcwd())
print("__file__:", __file__)
hotjuice_path = os.path.dirname(os.path.realpath(__file__))
print("hotjuice path:", os.path.dirname(os.path.realpath(__file__)))
if len(sys.argv) < 2:
    print ("please provide the name of oF plugin folder to update as the argument to this script")
    quit()
else:
    print("argument 1:", sys.argv[1])
path_to_xcode_project = os.path.join(os.getcwd(), sys.argv[1]) + "/" +  sys.argv[1] + ".xcodeproj" + "/project.pbxproj"
print(path_to_xcode_project)

# Updating product type

filereader = open(path_to_xcode_project, 'r')
filetext = filereader.read()
filereader.close()
if (filetext.find('com.apple.product-type.application')):
    print("replacing product type...")
filetext = filetext.replace('com.apple.product-type.application', 'com.apple.product-type.library.dynamic')

if (filetext.find('com.apple.product-type.application') >= 0):
    print("stil the same product type!")

if (filetext.find('$(TARGET_NAME)Debug') >= 0):
    print("replacing debug product name...")
filetext = filetext.replace('$(TARGET_NAME)Debug', '$(TARGET_NAME)')

if (filetext.find('$(TARGET_NAME)Debug') >= 0):
    print("STILL THE SAME!!!")
filetext = filetext.replace('"' + sys.argv[1] + '"', '"lib' + sys.argv[1] + '"')

os.remove(path_to_xcode_project)

filewriter = open(path_to_xcode_project, 'w')
n = filewriter.write(filetext)
filewriter.close()



project = XcodeProject.load(path_to_xcode_project)


# Updating build script

runscript = \
'''COMPANY_NAME="COMPANYNAME"

APPSUPPORT_PATH="/Users/$USER/Library/Application Support/$COMPANY_NAME/$PRODUCT_BUNDLE_IDENTIFIER"
echo $APPSUPPORT_PATH;

mkdir -p "$APPSUPPORT_PATH/"

# Copy libfmod and change install directory for fmod to run

rsync -aved "$OF_PATH/libs/fmodex/lib/osx/libfmodex.dylib" "$APPSUPPORT_PATH/"

rsync -aved  "$TARGET_BUILD_DIR/$PRODUCT_NAME.dylib" "$APPSUPPORT_PATH/$PRODUCT_NAME.dylib"

install_name_tool -change @executable_path/libfmodex.dylib @loader_path/libfmodex.dylib "$APPSUPPORT_PATH/$PRODUCT_NAME.dylib" 

cp -R "RESOURCES_SOURCE" "$APPSUPPORT_PATH/"

echo "$GCC_PREPROCESSOR_DEFINITIONS";
'''

finalscript = runscript.replace("RESOURCES_SOURCE", os.path.join(os.getcwd(), "resources"))
finalscript = finalscript.replace("COMPANYNAME", company_name)

foundScript = False
id = 0
for target in project.objects.get_targets(None):
    for build_phase_id in target.buildPhases:
        build_phase = project.objects[build_phase_id]
        if not isinstance(build_phase, PBXShellScriptBuildPhase):
            continue

        print("script #", id)
        thisscript = build_phase.shellScript
        # TODO: also remove and replace the bundled oF script that starts with 'mkdir -p "$TARGET_BUILD_DIR'
        if thisscript.strip().startswith('COMPANY'):
            if not foundScript:
                build_phase.shellScript = finalscript
                foundScript = True
                print("replaced the script with the correct one")
            else:
                # remove this script
                project.remove_run_script(build_phase.shellScript)
                print("removed extra script")
        else:
            print(thisscript.strip()[:10], " does not begin with ", 'COMPANY'.strip())
        # if (build_phase.shellScript.startswith("COMPANY_NAME")) or (build_phase.shellScript.startswith('mkdir -p "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"')):
        #     print("found the script!")
        #     print(build_phase.shellScript)
        #     build_phase.shellScript = finalscript
        #     foundScript = True
        # else:
        #     print("this script doesn't start with companyname:", build_phase.shellScript)
        id += 1

if not foundScript:
    project.add_run_script(script=finalscript)

# adding preprocessor macros for murka

project.add_flags('GCC_PREPROCESSOR_DEFINITIONS', 'MURKA_OF')
project.add_flags('GCC_PREPROCESSOR_DEFINITIONS', 'MURKA_OFFSCREEN')

# removing extra files

project.remove_files_by_path("src/ofApp.cpp")
project.remove_files_by_path("src/ofApp.h")
project.remove_files_by_path("src/main.cpp")

# adding files

group = project.get_or_create_group('hotjuice_source')
project.add_folder(hotjuice_path + "/src", parent=group)

# setting bundle id

project.set_flags('PRODUCT_BUNDLE_IDENTIFIER',bundle_id)

project.save()

# project.remove_run_script()