
# hotjuice
A framework for making projects in Juce with rich OpenGL UI and hot reloading of plugin code via dynamic linking

For JUCE project:
 - Add header search path to `hotjuice` folder

For oF subplugin:

Show contents of *.xcodeproj, find project.pbxproj file and open it with xcode, then change: 

 - Change `productType` string to  `com.apple.product-type.library.dynamic`
 - Change name of project (above productType) to  `libNAME`
 - Change `PRODUCT_NAME` for debug to  `$(TARGET_NAME)` instead of `$(TARGET_NAME)Debug`

Set for xcode:

 - Add header search path to `hotjuice` folder
 - Run "Clean" command if you switched target (Release/ Debug)
 - Change bundle id of your app, for example `com.company.application`
 - Set `Optimization Level` to `None` for Release
 - Set "Preprocessor Macros"

		MURKA_OF
		MURKA_OFFSCREEN

- Set "Run script" in “Build Phases” tab after "link" setting with:

		mkdir -p "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"
		# Copy default icon file into App/Resources
		rsync -aved "$ICON_FILE" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"

		mkdir -p "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/"

		APPSUPPORT_PATH="/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/"

		echo $APPSUPPORT_PATH;

		# Copy libfmod and change install directory for fmod to run

		rsync -aved "$OF_PATH/libs/fmodex/lib/osx/libfmodex.dylib" "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/";

		rsync -aved  "$TARGET_BUILD_DIR/$PRODUCT_NAME.dylib" "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/$PRODUCT_NAME.dylib"

		install_name_tool -change @executable_path/libfmodex.dylib    @loader_path/libfmodex.dylib "/Users/$USER/Library/Application Support/$PRODUCT_BUNDLE_IDENTIFIER/$PRODUCT_NAME.dylib" 

		echo "$GCC_PREPROCESSOR_DEFINITIONS";

