#!/usr/bin/python

import os
import sys
from pbxproj import XcodeProject
from pbxproj import PBXShellScriptBuildPhase

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

project = XcodeProject.load(path_to_xcode_project)

project.objects

# Updating product type

filereader = open(path_to_xcode_project, 'r')
filetext = filereader.read()
filereader.close()
filetext = filetext.replace('com.apple.product-type.application', 'com.apple.product-type.library.dynamic')
filetext = filetext.replace('$(TARGET_NAME)Debug', '$(TARGET_NAME)')
filetext = filetext.replace('"' + sys.argv[1] + '"', '"lib' + sys.argv[1] + '"')

filewriter = open(path_to_xcode_project, 'w')
filewriter.write(filetext)

# Updating build script

runscript = \
'''
COMPANY_NAME="Mach1Spatial"

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

foundScript = False
id = 0
for target in project.objects.get_targets(None):
    for build_phase_id in target.buildPhases:
        build_phase = project.objects[build_phase_id]
        if not isinstance(build_phase, PBXShellScriptBuildPhase):
            continue

        print("script #", id)
        if (build_phase.shellScript.startswith("COMPANY_NAME")) or (build_phase.shellScript.startswith('mkdir -p "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"')):
            print("found the script!")
            print(build_phase.shellScript)
            build_phase.shellScript = finalscript
            foundScript = True
        id += 1

if not foundScript:
    project.add_run_script(script=finalscript)

# adding files

group = project.get_or_create_group('hotjuice_source')
project.add_folder(hotjuice_path + "/src", parent=group)

# setting bundle id

project.set_flags('PRODUCT_BUNDLE_IDENTIFIER','com.productbundle.identifier')

project.save()

# project.remove_run_script()