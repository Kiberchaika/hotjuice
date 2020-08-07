# hotjuice
A framework for making projects in Juce with rich OpenGL UI and hot reloading of plugin code via dynamic linking

For oF subplugin:

You need to:
  - Clean subplugin product if you switched target (Release/ Debug)
 - Set optimization to None for Release
- 
 need to define (set "Preprocessor Macros" for xcode)
 MURKA_OF
 MURKA_OFFSCREEN

- Set "Run script" in “Build Phases” tab after "link" setting with:
 