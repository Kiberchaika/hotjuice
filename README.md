
# hotjuice
A framework for making projects in Juce with rich OpenGL UI and hot reloading of plugin code via dynamic linking

Supported oF 0.10.+ and 0.11.+

For JUCE project:
 - Add `hotjuice/src` folder to header search path, add contained files to project

For the OF plugin, use our generators for processing created in the OF project:
- For windows :
1. Install python 2.7
2. Call from `cmd` (run cmd as administrator):
> assoc .py=PythonScript
> ftype PythonScript=C:\bin\python.exe "%1" %*
3. Added path to `hotjuice` folder to PATH environment variable 
4. Go to folder with your project and `resources` folder and call `project_setup_win.py YOUR_PROJECT_NAME`

- For OSX:
1. 
