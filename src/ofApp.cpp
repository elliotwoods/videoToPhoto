#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape((1920 * 2 + 300) / 2, 1080 / 2);

	this->gui.init();
	
	{
		auto tier1 = ofxCvGui::Panels::Groups::makeStrip();
		
		tier1->setDirection(ofxCvGui::Panels::Groups::Strip::Direction::Vertical);
		tier1->setCellSizes({-1, 60});
		
		{
			auto tier2_1 = ofxCvGui::Panels::Groups::makeStrip();
			tier2_1->setDirection(ofxCvGui::Panels::Groups::Strip::Direction::Horizontal);
			tier2_1->setCellSizes({-1, -1, 300});
			
			{
				tier2_1->add(ofxCvGui::Panels::makeBaseDraws(this->video, "Video"));
			}

			{
				tier2_1->add(ofxCvGui::Panels::makeBaseDraws(this->photo, "Photo"));
			}
			
			
			{
				auto controls = ofxCvGui::Panels::makeWidgets();
				controls->addToggle("Play", [this]() {
					return !this->video.isPaused();
				},
					[this](bool playing) {
					if (playing && !this->video.isPlaying()) {
						this->video.play();
					}
					this->video.setPaused(!playing);
				})->setHotKey(' ');
				controls->addButton("Stop", [this]() {
					this->video.stop();
					this->video.setPosition(0);
				}, 's');
				controls->addSlider(this->frameCount)->addIntValidator();
				{
					auto blendingSelector = controls->addMultipleChoice("Blending");
					blendingSelector->addOption("Mean");
					blendingSelector->addOption("Median");
					blendingSelector->addOption("Min");
					blendingSelector->addOption("Max");
					blendingSelector->entangle(this->blendingMode);
				}
				controls->addButton("Generate", [this]() {
					this->generatePhoto();
				}, OF_KEY_RETURN)->setHeight(100.0f);
				tier2_1->add(controls);
			}
			tier1->add(tier2_1);
		}
		
		{
			auto timelineView = ofxCvGui::Panels::makeWidgets();
			{
				auto frameIndexWidget = timelineView->addSlider(this->frameIndex);
				frameIndexWidget->addIntValidator();
				timelineView->onUpdate += [this, frameIndexWidget](ofxCvGui::UpdateArguments &) {
					frameIndexWidget->setEnabled(this->frameIndex.getMax() > 0);
				};
				frameIndexWidget->onValueChange += [this](ofParameter<float> & frameIndex) {
					this->video.setFrame(frameIndex);
				};
			}
			tier1->add(timelineView);
		}
		
		this->gui.add(tier1);
	}
	
	//allocate blank photo
	{
		this->photo.allocate(32, 32, OF_IMAGE_COLOR_ALPHA);
		this->photo.getPixels().set(0);
		this->photo.update();
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	this->frameIndex = this->video.getCurrentFrame();
	this->video.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::loadMovie(string path) {
	if(this->video.load(path)) {
		this->frameIndex.setMax(this->video.getTotalNumFrames() - 1);
		this->frameIndex.set(0);
		this->video.play();

		auto width = this->video.getWidth();
		auto height = this->video.getHeight();

		while (max(width, height) > 1024) {
			width /= 2;
			height /= 2;
		}
		ofSetWindowShape(width * 2 + 300, height + 60);
	} else {
		this->frameIndex.setMax(0);
		this->frameIndex.set(0);
	}
}


//----------
void ofApp::generatePhoto() {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	if (dragInfo.files.size() == 1) {
		this->loadMovie(dragInfo.files.front());
	}
}
