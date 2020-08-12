
# hotjuice
A framework for making projects in Juce with rich OpenGL UI and hot reloading of plugin code via dynamic linking

For JUCE project:
 - Add `hotjuice/src` folder to header search path, add contained files to prjoect

For oF subplugin:

Show contents of *.xcodeproj, find project.pbxproj file and open it with xcode, then change: 

 - Change `productType` string to  `com.apple.product-type.library.dynamic`
 - Change `name` of project (above productType) to  `libNAME`
 - Change `PRODUCT_NAME` for debug to  `$(TARGET_NAME)` instead of `$(TARGET_NAME)Debug`

Set for xcode:

 - Add `hotjuice/src` folder to header search path, add contained files to prjoect
 - Run "Clean" command if you switched target (Release/ Debug)
 - Change `bundle id` of your app, for example `com.company.application`
 - Set `Optimization Level` to `None` for Release
 - Set "Preprocessor Macros"

		MURKA_OF
		MURKA_OFFSCREEN

- Set "Run script" in “Build Phases” tab after "link" setting with and change `COMPANY_NAME` with your name:

		COMPANY_NAME="MyCompany"

		mkdir -p "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"

		# Copy default icon file into App/Resources
		rsync -aved "$ICON_FILE" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"

		APPSUPPORT_PATH="/Users/$USER/Library/Application Support/$COMPANY_NAME/$PRODUCT_BUNDLE_IDENTIFIER"
		echo $APPSUPPORT_PATH;

		mkdir -p "$APPSUPPORT_PATH/"

		# Copy libfmod and change install directory for fmod to run

		rsync -aved "$OF_PATH/libs/fmodex/lib/osx/libfmodex.dylib" "$APPSUPPORT_PATH/"

		rsync -aved  "$TARGET_BUILD_DIR/$PRODUCT_NAME.dylib" "$APPSUPPORT_PATH/$PRODUCT_NAME.dylib"

		install_name_tool -change @executable_path/libfmodex.dylib @loader_path/libfmodex.dylib "$APPSUPPORT_PATH/$PRODUCT_NAME.dylib" 

		echo "$GCC_PREPROCESSOR_DEFINITIONS";

