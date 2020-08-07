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



 mkdir -p "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"
# Copy default icon file into App/Resources
rsync -aved "$ICON_FILE" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"

mkdir -p "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/"

APPSUPPORT_PATH="/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/"

echo "THIS IS THE TESTING POINT!!!!!!!!!!";
echo $APPSUPPORT_PATH;

# Copy libfmod and change install directory for fmod to run
#cp "$OF_PATH/libs/fmodex/lib/osx/libfmodex.dylib" "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/";

rsync -aved "$OF_PATH/libs/fmodex/lib/osx/libfmodex.dylib" "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/";

#install_name_tool -id "@loader_path/libfmodex.dylib" "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/libfmodex.dylib" 

rsync -aved  "$TARGET_BUILD_DIR/$PRODUCT_NAME.dylib" "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/$PRODUCT_NAME.dylib"


#install_name_tool -id "~/Library/Application\ Support/com.mach1.creatorapp/$PRODUCT_NAME.dylib" "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/$PRODUCT_NAME.dylib" 

install_name_tool -change @executable_path/libfmodex.dylib    @loader_path/libfmodex.dylib "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/$PRODUCT_NAME.dylib" 



echo "$GCC_PREPROCESSOR_DEFINITIONS";