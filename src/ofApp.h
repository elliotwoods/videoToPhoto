#pragma once

#include "ofMain.h"

#include "ofxCvGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	
		void loadMovie(string path);
		void generatePhoto();

		void dragEvent(ofDragInfo dragInfo);


		void keyPressed(int key) { }
		void keyReleased(int key) { }
		void mouseMoved(int x, int y) { }
		void mouseDragged(int x, int y, int button) { }
		void mousePressed(int x, int y, int button) { }
		void mouseReleased(int x, int y, int button) { }
		void mouseEntered(int x, int y) { }
		void mouseExited(int x, int y) { }
		void windowResized(int w, int h) { }
		void gotMessage(ofMessage msg) { }

	ofxCvGui::Builder gui;
	shared_ptr<ofxCvGui::Panels::Widgets> thumbnailView;
	
	ofShortImage photo;
	ofVideoPlayer video;
	
	ofParameter<float> frameIndex { "Frame index", 0, 0, 0};
	ofParameter<float> frameCount{ "Frame count", 1, 1, 5 };

	enum BlendingMode {
		Mean,
		Median,
		Min,
		Max
	};
	ofParameter<BlendingMode> blendingMode{ "Blending", Mean };
};
