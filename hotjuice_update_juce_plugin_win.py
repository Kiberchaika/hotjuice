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

for subdir, dirs, files in os.walk(os.path.join(pathWorkingDir, sys.argv[1])):
    for name in files:
        path_to_project = subdir + os.sep + name

        if path_to_project.endswith(("_SharedCode.vcxproj")):

            tree = etree.parse(path_to_project)

            namespaces = {'ns': 'http://schemas.microsoft.com/developer/msbuild/2003'}

            # added include dir
            for el in tree.xpath('//ns:ClCompile/ns:AdditionalIncludeDirectories', namespaces=namespaces):
                if el.text.find(pathHotjuice) < 0:
                    el.text = el.text + ";" + pathHotjuice


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

            # tree.write('example_oF_subplugin.vcxproj')
            tree.write(path_to_project)
