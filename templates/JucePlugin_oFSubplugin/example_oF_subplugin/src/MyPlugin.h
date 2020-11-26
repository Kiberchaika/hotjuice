#include "ofMain.h"

#include "ofxHotJuice.h"

#include "ofxMurka.h"
#include "TestSurface.h"
 
#include "ofxPluginWithRender.h"

using namespace murka;
 
class MyPlugin : public ofxPluginWithRender {

	struct PaneInfo {
		int kind = 0; // 0 = off
		// 1 = horizontal
		// 2 = vertical
		 
		double position = 0.5;
		 
		// 4 indices of where to look for more internal panes in the map while we draw them
		int internalPane1Index = 0;
		int internalPane2Index = 0;
	};

	std::map<int, PaneInfo> panes{
		{0 , PaneInfo()},
		{1 , PaneInfo()},
	};

	Murka m;
	MurImage logo;
	ofImage img2;

	float a = 0;
	float b = 0;

	bool isCloned = false;
	std::string pathToResources;

	float lastUIScale = 1.0;
 
public:
	MyPlugin() {
#if defined (_WIN32)
		std::string pluginPath = "MyCompany/Hotjuice oF and JUCE example";
		pathToResources = hotjuice::Utils::getAppDataDirectory() + pluginPath + "/resources/";
#elif defined (__APPLE__)
		std::string pluginPath = "MyCompany/com.company.application";
		pathToResources = hotjuice::Utils::getAppDataDirectory() + pluginPath + "/resources/";
#endif

		cout << "creating MyPlugin" << endl;
        m.setResourcesPath(pathToResources);
		
		PLUGIN_ADD_FUNCTION(MyPlugin, test);
	}


	~MyPlugin() {
		cout << "deleting MyPlugin" << endl;
	}

	void test(void* in, void* out) {
		//std::cout << "test" << " a: " << a << std::endl;
	}

	void setupRenderer(void* in, void* out) override {
		ofxPluginWithRender::setupRenderer();
        
        lastUIScale = getDesktopScale();

		m.setWindow((ofAppBaseWindow*)(&window));
		m.setRenderer((ofBaseGLRenderer*)(window.renderer().get()));
	}
    
	void prepareToStartRendering(bool shouldReloadData) override {
		if (!shouldReloadData) {
			logo.load(pathToResources + "mach1logo.png");
		}
		else {
			logo.update();

			m.updateFontsTextures(&m);
			//   m.clearFontsCache();
		}
	}

	void prepareToStopRendering() override {
		logo.clearTexture();

		m.clearFontsTextures();
		// m.clearFontsCache();
	}

	void setup(void* in, void* out) override {
		if (!isCloned) {
			panes[0].position = 0.5;
			panes[1].position = 0.5;
		}
		// */
        
        _log = "SETUP MADE";
	}

	int cc = 0;
	void draw(void* in, void* out) override {
        if (getDesktopScale() != lastUIScale) {
            lastUIScale = getDesktopScale();
        }
        
		ofBaseRenderer& renderer = *(window.renderer());
		renderer.startRender();

		renderer.pushView();
		renderer.pushStyle();

		renderer.setColor(ofColor::forestGreen); // pink , forestGreen
		renderer.drawRectangle(cc, 0, 0, 200, 100);

		cc++;
		if (cc > 200) cc = 0;

		renderer.popStyle();
		renderer.popView();


		renderer.pushView();
		renderer.pushStyle();

		/*
		m.begin(); 
		drawWidget<TextField>(m, &str, {}, { 20, 330, 200, 35 });
		m.end(); 
		*/
		///*


		double time = ofGetElapsedTimef();

		m.begin();
		m.setFont("Roboto-Regular.ttf", 10);

		Pane::Parameters paneParameters;
		paneParameters.firstPanelDraw = [&](MurkaContext& c) {

			drawWidget<Pane>(c, &(panes[1].position),
				{ [&](MurkaContext& g) {

				g.renderer->setColor(0, 90, 200);
				g.renderer->drawRectangle(10, 10, g.getSize().x - 20, g.getSize().y - 20);
				drawWidget<TestSurface>(g, {}, {0, 0, g.getSize().x, g.getSize().y});
				},
				[&](MurkaContext& g) {
				g.renderer->setColor(90, 0, 110);

				g.renderer->pushStyle();
				g.renderer->disableFill();
				g.renderer->drawRectangle(30, 30, g.getSize().x - 60, g.getSize().y - 60);
				g.renderer->drawLine(0, 0, g.getSize().x, g.getSize().y);
				g.renderer->popStyle();

				drawWidget<TestSurface>(g, {}, {0, 0, g.getSize().x, g.getSize().y});
				}, 
				2 }, { 0, 0, c.getSize().x, c.getSize().y });
		};
		paneParameters.secondPanelDraw = [&](MurkaContext& c) {
			c.renderer->setColor(50, 90, 50);
			c.renderer->drawRectangle(10 + 10 * sin(time),
				10 + 10 * sin(time),
				c.getSize().x - 20 + 10 * sin(time),
				c.getSize().y - 20 + 10 * sin(time));

			drawWidget<TestSurface>(c, {}, { 0, 0, c.getSize().x, c.getSize().y });
		};
		paneParameters.kind = 1;

		drawWidget<Pane>(m, &(panes[0].position), paneParameters, { 0, 0, m.getWindowWidth(), m.getWindowWidth() });



		m.end();
		//*/


		renderer.popStyle();
		renderer.popView();


		renderer.setColor(ofColor::black);
		drawLog();
		drawUpdateStatus();

		renderer.finishRender();
	}

	void mouseDragged(int x, int y, int button) override {
		m.registerMouseDragged(x, y, button);
	}

	void mouseMoved(int x, int y) override {
		m.registerMouseMoved(x, y, 0);
	}

	void mousePressed(int x, int y, int button) override {
		m.registerMousePressed(x, y, button);
	}

	void mouseReleased(int x, int y, int button) override {
		m.registerMouseReleased(x, y, button);
	}

	bool keyPressed(int key) override {
		//_log += string("keyPressed : ") + to_string(iIn[0]) + string("\r\n");
		m.registerKeyPressed(key);
		return true;
	}

	bool keyReleased(int key) override {
		//_log += string("keyReleased : ") + to_string(iIn[0]) + string("\r\n");
		m.registerKeyReleased(key);
		return true;
	}

	void update(void* in, void* out) override {
		float(&fIn)[2] = *reinterpret_cast<float(*)[2]>(in);
		float(&fOut)[1] = *reinterpret_cast<float(*)[1]>(out);

		fOut[0] = fIn[0] * fIn[1] + 10000;
	}

	
	float phase = 0;

	void process(void* in, void* out) override {
		std::tuple<std::vector<float*>, int, double>& audioDataPackage = *(std::tuple<std::vector<float*>, int, double> *)in;

		std::vector<float*> buffers = std::get<0>(audioDataPackage);
		int channels = buffers.size();
		int bufferSize = std::get<1>(audioDataPackage);
		int sampleRate = std::get<2>(audioDataPackage);

		while (phase > TWO_PI) {
			phase -= TWO_PI;
		}

		float phaseOrig = phase;
		for (int i = 0; i < buffers.size(); i++) {
			phase = phaseOrig;
			for (int j = 0; j < bufferSize; j++) {
				phase += 0.025;
				buffers[i][j] = 0.2 * sin(phase);
			}
		}

		//_log += "buffer count: "  +  std::to_string(std::get<1>(data)) + "\r\n";
	}

	void clone(hotjuice::BasePlugin* basePlugin) override {
		MyPlugin* plugin = (MyPlugin*)basePlugin;

		a = plugin->a; 
		b = plugin->b;
		 
		isCloned = true;
		panes[0].position = plugin->panes[0].position;
		panes[1].position = plugin->panes[1].position;
	}
};

PLUGIN_LOADER(MyPlugin);


