#include "cinder/Rand.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "CinderPango.h"

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
	kp::pango::CinderPango::setTextRenderer(kp::pango::TextRenderer::FREETYPE);
	mPango = kp::pango::CinderPango::create();
	mPango->setMinSize(100, 100);
	mPango->setMaxSize(getWindowWidth(), getWindowHeight());
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
		case KeyEvent::KEY_UP:
			mPango->setSpacing(mPango->getSpacing() + 1.0);
			break;
		case KeyEvent::KEY_DOWN:
			mPango->setSpacing(mPango->getSpacing() - 1.0);
			break;
		case KeyEvent::KEY_i:
			mPango->setDefaultTextItalicsEnabled(!mPango->getDefaultTextItalicsEnabled());
			break;
		case KeyEvent::KEY_d:
			mPango = nullptr;
			break;
		case KeyEvent::KEY_c:
			mPango = kp::pango::CinderPango::create();
			mPango->setMaxSize(getWindowWidth(), getWindowHeight());
			break;
		case KeyEvent::KEY_b:
			if (mPango->getBackgroundColor() == ci::ColorA::zero()) {
				mPango->setBackgroundColor(ci::ColorA(ci::CM_HSV, ci::Rand::randFloat(), 0.5f, 1.0f, 1.0f));
			} else {
				// Go clear
				mPango->setBackgroundColor(ci::ColorA::zero());
			}
			break;
		case KeyEvent::KEY_r:
			if (kp::pango::CinderPango::getTextRenderer() == kp::pango::TextRenderer::FREETYPE) {
				kp::pango::CinderPango::setTextRenderer(kp::pango::TextRenderer::PLATFORM_NATIVE);
			} else {
				kp::pango::CinderPango::setTextRenderer(kp::pango::TextRenderer::FREETYPE);
			}

			// rebuild
			mPango = kp::pango::CinderPango::create();
			mPango->setMaxSize(getWindowWidth(), getWindowHeight());
			break;
		default:
			break;
	}
}

void PangoBasicApp::update() {
	if (mPango != nullptr) {

		mPango->setText(
				"<b>Bold Text</b> "
				"<span foreground=\"green\" font=\"24.0\">Green téxt</span> "
				"<span foreground=\"red\" font=\"Times 48.0\">Red text</span> "
				"<span foreground=\"blue\" font=\"Gravur Condensed Pro 72.0\">AVAVAVA Blue text</span> "
				"<i>Italic Text</i> "
				"hovedgruppen fra <i>forskjellige</i> destinasjoner. Tilknytningsbillett er gyldig inntil 24 timer f√∏r avreise hovedgruppe.\n\nUnicef said 3m "
				"people had been affected and more than <span font=\"33.0\">1,400</span> had been killed. <b>The government</b> said some 27,000 people remained "
				"trapped "
				"and awaiting help. " +
				std::to_string(getElapsedFrames()));

		// Only renders if it needs to
		mPango->render();
	}
}

void PangoBasicApp::draw() {
	float bgColor = (0.5 + 0.5 * sin(0.5 * getElapsedSeconds()));
	gl::clear(Color(bgColor, bgColor, bgColor));
	gl::enableAlphaBlendingPremult();

	if (mPango != nullptr) {
		gl::draw(mPango->getTexture());
	}
}

CINDER_APP(PangoBasicApp, RendererGl)
