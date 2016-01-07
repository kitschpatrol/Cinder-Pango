#include "CinderPango.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PangoBasicApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void keyDown(KeyEvent event) override;
	void draw() override;

	kp::pango::CinderPangoRef mPango;
};

void PangoBasicApp::setup() {
	mPango = kp::pango::CinderPango::create();
	mPango->setMaxSize(getWindowWidth(), getWindowHeight());
	// mPango->setTextRenderer(kp::pango::TextRenderer::FREETYPE);
}

void PangoBasicApp::mouseDown(MouseEvent event) {
	mPango->setMaxSize(event.getPos());
}

void PangoBasicApp::keyDown(KeyEvent event) {

	switch (event.getCode()) {
		case KeyEvent::KEY_a:
			// Toggle Anti-Aliasing
			if (mPango->getTextAntialias() == kp::pango::TextAntialias::NONE) {
				mPango->setTextAntialias(kp::pango::TextAntialias::DEFAULT);
			} else {
				mPango->setTextAntialias(kp::pango::TextAntialias::NONE);
			}
			break;

		default:
			break;
	}
}

void PangoBasicApp::update() {
	mPango->setText(
			"<b>Bold Text</b> "
			"<span foreground=\"green\" font=\"24.0\">Green téxt</span> "
			"<span foreground=\"red\" font=\"Times 48.0\">Red text</span> "
			"<span foreground=\"blue\" font=\"Verdana 72.0\">Blue text</span> "
			"<i>Italic Text</i> "
			"hovedgruppen fra <i>forskjellige</i> destinasjoner. Tilknytningsbillett er gyldig inntil 24 timer f√∏r avreise hovedgruppe.\n\nUnicef said 3m "
			"people had been affected and more than <span font=\"33.0\">1,400</span> had been killed. <b>The government</b> said some 27,000 people remained "
			"trapped "
			"and awaiting help. "

			);
	//	std::to_string(getElapsedFrames()));

	// Only renders if it needs to
	mPango->render();
}

void PangoBasicApp::draw() {
	float bgColor = (0.5 + 0.5 * sin(0.5 * getElapsedSeconds()));
	gl::clear(Color(bgColor, bgColor, bgColor));

	gl::draw(mPango->getTexture());
}

CINDER_APP(PangoBasicApp, RendererGl)
