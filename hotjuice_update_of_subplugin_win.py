#!python

# associate py with local https://gist.github.com/Hrxn/e2180e3c34bbed2c2e7e

import os
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

for el in tree.xpath('//ns:ClCompile/ns:AdditionalIncludeDirectories', namespaces=namespaces):
    if el.text.find(pathHotjuice) < 0:
        el.text = el.text + ";" + pathHotjuice

for el in tree.xpath('//ns:PreprocessorDefinitions', namespaces=namespaces):
    if el.text.find("MURKA_OFFSCREEN") < 0:
        el.text = 'MURKA_OFFSCREEN;' + el.text
    if el.text.find("MURKA_OF") < 0:
        el.text = 'MURKA_OF;' + el.text

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

include_files('ClCompile', '.cpp')
include_files('ClInclude', '.h')

path_to_plugin = os.path.join(os.environ.get("APPDATA"), company_name, project_name)
for el in tree.xpath('//ns:PostBuildEvent', namespaces=namespaces):
    el.append(etree.XML('<Command>' +
    'mkdir ' + '"' + path_to_plugin + '" ' +
    ' &amp; ' + 'robocopy "' + os.path.join(pathWorkingDir, 'resources') + '" "' + path_to_plugin + '/resources/" /E '  +
    ' &amp; ' + 'robocopy "$(ProjectDir)bin/" "' + path_to_plugin + '/" "*.dll" "*.pdb" /is '  +
    ' &amp; ' + 'exit 0' +
    '</Command>'))

#tree.write('example_oF_subplugin.vcxproj')
tree.write(path_to_project)