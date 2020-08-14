#!python

# associate py with local https://gist.github.com/Hrxn/e2180e3c34bbed2c2e7e

import os
from xml.sax import saxutils
from lxml import etree

pathWorkingDir = os.getcwd()
pathFileDir = os.path.dirname(os.path.realpath(__file__))
pathHotjuice =  os.path.join(pathFileDir, 'src')

import sys
sys.path.append(pathWorkingDir)
from project_settings import *

path_to_project = os.path.join(pathWorkingDir, sys.argv[1], sys.argv[1] + '.vcxproj')
tree = etree.parse(path_to_project)

namespaces = {'ns':'http://schemas.microsoft.com/developer/msbuild/2003'}

# added include dir
for el in tree.xpath('//ns:ClCompile/ns:AdditionalIncludeDirectories', namespaces=namespaces):
    if el.text.find(pathHotjuice) < 0:
        el.text = el.text + ";" + pathHotjuice

# added predefined
for el in tree.xpath('//ns:PreprocessorDefinitions', namespaces=namespaces):
    if el.text.find("MURKA_OFFSCREEN;") < 0:
        el.text = 'MURKA_OFFSCREEN;' + el.text
    if el.text.find("MURKA_OF;") < 0:
        el.text = 'MURKA_OF;' + el.text

# change prj to dll
for el in tree.xpath('//ns:ConfigurationType', namespaces=namespaces):
    el.text = 'DynamicLibrary'

def include_files(section, extension):
    el = tree.xpath('//ns:ItemGroup/ns:' + section + '/..', namespaces=namespaces)[0]
    for file in os.listdir(pathHotjuice):
        if file.endswith(extension):

            path = os.path.join(pathHotjuice, file)
            isFound = False
            for el_ in tree.xpath('//ns:ItemGroup/ns:' + section, namespaces=namespaces):
                if el_.attrib['Include'] == path:
                    isFound = True

            if not isFound:
                el.append(etree.XML('<' + section + ' Include="' + path + '"/>'))

# add include files
include_files('ClCompile', '.cpp')
include_files('ClInclude', '.h')

path_to_plugin = os.path.join(os.environ.get("APPDATA"), company_name, project_name)

# add post build
for el in tree.xpath('//ns:PostBuildEvent', namespaces=namespaces):
    el.find('..').append(etree.XML('<CustomBuildStep><Command>' +
    saxutils.escape('mkdir ' + '"' + path_to_plugin + '" ') +
    saxutils.escape(' & ' + 'robocopy "' + os.path.join(pathWorkingDir, 'resources') + '" "' + path_to_plugin + '/resources/" /E ') +
    saxutils.escape(' & ' + 'robocopy "$(ProjectDir)bin/" "' + path_to_plugin + '/" "*.dll" "*.pdb" /is ') +
    saxutils.escape(' & ' + 'exit 0') +
    '</Command><Outputs>_log</Outputs></CustomBuildStep>'))

for el in tree.xpath('//ns:OutDir', namespaces=namespaces):
    el.find('..').append(etree.XML('<CustomBuildAfterTargets>Build</CustomBuildAfterTargets>'))


#tree.write('example_oF_subplugin.vcxproj')
tree.write(path_to_project)