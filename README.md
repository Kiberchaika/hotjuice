
# hotjuice
A framework for making projects in Juce with rich OpenGL UI and hot reloading of plugin code via dynamic linking

Supported oF 0.10.+ and 0.11.+

## Getting started with a project that has oF subplugin and JUCE main plugin

First, copy the template from this repositorty. Then, edit project_settings.py with the appropriate settings with your project.

For JUCE project:
- Add `hotjuice/src` folder to header search path, add contained files to project for them to compile.
- Be sure to check where it loads the dylib, it's written in PluginProcessor.cpp

For the OF plugin, use our generators for processing created in the OF project:
- Windows :
1. Install python 2.7
2. Call from `cmd` (run cmd as administrator):
> assoc .py=PythonScript
> ftype PythonScript=C:\bin\python.exe "%1" %*
3. Add path to `hotjuice` folder to PATH environment variable 
4. Go to folder with your subplugin project and `resources` folder and call `hotjuice_update_of_subplugin_win.py YOUR_SUBPLUGIN_PROJECT_NAME`

- OSX:
1. you have to pip install mod-pbxproj to the python that's at /usr/bin/python
2. add hotjuice to $PATH via ~/.bash_profile or otherwise
3. call chmod +x on hotjuice_update_of_subplugin_osx.py so you may call the script
4. generate oF project via projectGenerator, open it in XCode, change something (like build version number) and save (regenerates project, todo: try to use https://github.com/CocoaPods/Xcodeproj to regenerate it from the script instead)
4. Go to folder with your subplugin project folder and `resources` folder and call `hotjuice_update_of_subplugin_osx.py YOUR_SUBPLUGIN_PROJECT_NAME`
