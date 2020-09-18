
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
1. Install pbxproj fork `pip install git+https://github.com/yeswecan/mod-pbxproj.git`
2. `chmod +x hotjuice_update_of_subplugin_osx.py`
3. Add this directory to PATH env (*CHANGE /path/to/hotjuice*)
   - `echo 'export PATH=/path/to/hotjuice:$PATH' >>~/.zshrc` or
   - `echo 'export PATH=/path/to/hotjuice:$PATH' >>~/.bash_profile`
4. Use (OpenFrameworks ProjectGenerator)[https://openframeworks.cc/download/] to generate and create xcode project
5. Make any change to xcode project settings (for example change the build version number)
6. Change directory to parent directory of subplugin project and `resources` and run script (changing name of `YOUR_SUBPLUGIN_PROJECT_NAME`): 
   - `hotjuice_update_of_subplugin_osx.py YOUR_SUBPLUGIN_PROJECT_NAME`
7. Use only debug build

## TODO
- Use https://github.com/CocoaPods/Xcodeproj to regenerate it from the script instead
