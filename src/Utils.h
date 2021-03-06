#pragma once

#include <string>

#if defined(JUCE_APP_VERSION)
#include "../JuceLibraryCode/JuceHeader.h"
#else
#include "ofMain.h"
#endif

namespace hotjuice {
	namespace Utils {
		void clearSubdirectories(std::string path);
		void createDirectory(std::string pathDir);
		std::string getDirectoryPath(std::string path);

		bool checkFileExits(std::string path);
		bool copyFileFromTo(std::string from, std::string to);

		std::string getAppDataDirectory();

		std::string joinFilePath(std::string path1, std::string path2);
		std::string getFileAbsolutePath(std::string path);
		std::string getFileName(std::string path);
		std::string getFileExtension(std::string path);
		std::string getEnclosingDirectory(std::string path);
		std::string getFileBaseName(std::string path);
		unsigned long long getFileModificationTime(std::string path);

		unsigned long long getElapsedTimeMillis();

        void Sleep(int ms);
		void Execute(std::string command);
	};
};
