#pragma once

#include "ofMain.h"
#include "ofxHotJuice.h"

class ofApp : public ofBaseApp {
	hotjuice::PluginManager* pluginManager;
	hotjuice::Plugin* plugin;

	bool needToReinitRender;
	bool isReloading;

public:
    void setup();
    void update();
    void draw();
	void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	void audioOut(ofSoundBuffer & buffer);

	ofSoundStream soundStream;
	std::tuple<std::vector<float*>, int, double> audioData;

};
