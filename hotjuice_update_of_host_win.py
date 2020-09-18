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

def include_file(filePath):
    ext = os.path.splitext(filePath)[1]
    section = None
    if ext == '.cpp' or ext == '.c':
        section = 'ClCompile'
    elif ext == '.hpp' or ext == '.h':
        section = 'ClInclude'

    if section:
        isFound = False
        el = tree.xpath('//ns:ItemGroup/ns:' + section + '/..', namespaces=namespaces)[0]
        for el_ in tree.xpath('//ns:ItemGroup/ns:' + section, namespaces=namespaces):
            if el_.attrib['Include'] == filePath:
                isFound = True

        if not isFound:
            el.append(etree.XML('<' + section + ' Include="' + filePath + '"/>'))

def include_files_from_dir(path):
    for file in os.listdir(path):
        include_file(os.path.join(path, file))

# add include files
include_files_from_dir(pathHotjuice)

tree.write(path_to_project)