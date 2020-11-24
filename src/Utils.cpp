#include "Utils.h"

#if defined(_WIN32)
#include <windows.h>
#include <shellapi.h>
#elif defined(__APPLE__)
#endif

#include <chrono>
#include <thread>

void hotjuice::Utils::clearSubdirectories(std::string path) {
#if defined(JUCE_APP_VERSION)
	juce::Array<juce::File> files = juce::File(path).findChildFiles(File::TypesOfFileToFind::findFilesAndDirectories, true);
	for (size_t i = 0; i < files.size(); i++) {
		if (files[i].isDirectory()) {
			files[i].deleteRecursively();
		}
	}
#else
	ofDirectory dir = ofDirectory(path);
	std::vector<ofFile> files = dir.getFiles();
	for (size_t i = 0; i < files.size(); i++) {
		if (ofDirectory(files[i]).isDirectory()) {
			ofDirectory::removeDirectory(files[i].getAbsolutePath(), true, false);
		}
	}
#endif
}

#if defined (_WIN32)
#elif defined (__APPLE__)
std::string _getAppDataDirectory();
#endif

std::string hotjuice::Utils::getAppDataDirectory()
{
    std::string str = "";
#if defined (_WIN32)
	str = std::string(getenv("APPDATA")) + "/";
#elif defined (__APPLE__)
    str = _getAppDataDirectory();
#endif
    return str;
}

void hotjuice::Utils::createDirectory(std::string pathDir) {
#if defined(JUCE_APP_VERSION)
	juce::File file(pathDir);
	if (!file.exists()) file.createDirectory();
#else
	ofDirectory dir(pathDir);
	if (!dir.exists()) dir.create(true);
#endif
}

std::string hotjuice::Utils::getDirectoryPath(std::string path) {
#if defined(JUCE_APP_VERSION)
	return juce::File::getCurrentWorkingDirectory().getChildFile(path).getFullPathName().toStdString();
#else
	return ofToDataPath(path, true);
#endif
}

bool hotjuice::Utils::checkFileExits(std::string path) {
#if defined(JUCE_APP_VERSION)
	return juce::File(path).exists();
#else
	return ofFile(path).exists();
#endif
}

bool hotjuice::Utils::copyFileFromTo(std::string from, std::string to) {
#if defined(JUCE_APP_VERSION)
	return juce::File(from).copyFileTo(juce::File(to));
#else
	return ofFile::copyFromTo(from, to, false, true);
#endif
}

std::string hotjuice::Utils::joinFilePath(std::string path1, std::string path2) {
#if defined(JUCE_APP_VERSION)
	return juce::File(path1).getChildFile(path2).getFullPathName().toStdString();
#else
	return ofFilePath::join(path1, path2);
#endif
}

std::string hotjuice::Utils::getFileAbsolutePath(std::string path) {
#if defined(JUCE_APP_VERSION)
	return juce::File(path).getFullPathName().toStdString();
#else
	return ofFilePath::getAbsolutePath(path, true);
#endif
}

std::string hotjuice::Utils::getFileName(std::string path) {
#if defined(JUCE_APP_VERSION)
    return juce::File(path).getFileName().toStdString();
#else
    return ofFilePath::getFileName(path);
#endif
}

std::string hotjuice::Utils::getFileExtension(std::string path) {
#if defined(JUCE_APP_VERSION)
    return juce::File(path).getFileExtension().toStdString();
#else
    return ofFilePath::getFileExt(path);
#endif
}

std::string hotjuice::Utils::getEnclosingDirectory(std::string path) {
#if defined(JUCE_APP_VERSION)
	return juce::File(path).getParentDirectory().getFullPathName().toStdString();
#else
	return ofFilePath::getEnclosingDirectory(path);
#endif
}

std::string hotjuice::Utils::getFileBaseName(std::string path) {
#if defined(JUCE_APP_VERSION)
	return juce::File(path).getFileNameWithoutExtension().toStdString();
#else
	return ofFilePath::getBaseName(path);
#endif
}

unsigned long long hotjuice::Utils::getFileModificationTime(std::string path) {
#if defined(JUCE_APP_VERSION)
	return juce::File(path).getLastModificationTime().toMilliseconds();
#else
	return std::filesystem::last_write_time(ofFile(path));
#endif
} 

unsigned long long hotjuice::Utils::getElapsedTimeMillis() {
#if defined(JUCE_APP_VERSION)
	return juce::Time::currentTimeMillis();
#else
	return ofGetElapsedTimeMillis();
#endif
}

void hotjuice::Utils::Execute(std::string command) {
#if defined(_WIN32)
	ShellExecuteA(NULL, NULL, command.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#elif defined(__APPLE__)
    system(command.c_str());
#endif
}

void hotjuice::Utils::Sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

