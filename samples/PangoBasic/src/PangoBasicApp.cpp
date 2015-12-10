#include "Pango.h"
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
	void draw() override;
	void createPangoContext();

	kp::pango::Pango *pango;
	kp::pango::CairoContext *context;
	kp::pango::Layout *layout;
	gl::Texture2dRef mPangoTexture;
	float bgColor;
};

void PangoBasicApp::setup() {

	pango = nullptr;
	context = nullptr;
	layout = nullptr;
	createPangoContext();
}

void PangoBasicApp::mouseDown(MouseEvent event) {
}

void PangoBasicApp::update() {
	// calc dynamic bg color
	bgColor = (0.5 + 0.5 * sin(0.5 * getElapsedSeconds()));
	float iBgColor = 1.0f - bgColor;

	// clear pango context, we will redraw on it
	context->clear();

	// add content to pango
	layout->setMarkup(
			"<b>Bold Text</b> "
			"<span foreground=\"green\" font=\"24.0\">Green text</span> "
			"<span foreground=\"red\" font=\"Times 48.0\">Red text</span> "
			"<span foreground=\"blue\" font=\"Verdana 72.0\">Blue text</span> "
			"<i>Italic Text</i> "
			"hovedgruppen fra <i>forskjellige</i> destinasjoner. Tilknytningsbillett er gyldig inntil 24 timer f√∏r avreise hovedgruppe.\n\nUnicef said 3m "
			"people had been affected and more than <span font=\"33.0\">1,400</span> had been killed. <b>The government</b> said some 27,000 people remained trapped "
			"and awaiting help." +
			std::to_string(getElapsedFrames()));

	// update canvas with new content
	context->color4f(iBgColor, iBgColor, iBgColor, 1.0f);
	layout->show();

	// upload pix data to GPU texture
	//	unsigned char *pix = context->getSurface()->getRawPixels();
	//	texture.loadData(pix, context->getSurface()->getWidth(), context->getSurface()->getHeight(),
	//									 GL_BGRA // note GL_BGRA! GL driver will do the data conversion
	//									 );

	mPangoTexture = layout->getTexture();
}

void PangoBasicApp::draw() {
	gl::clear(Color(bgColor, bgColor, bgColor));

	gl::draw(mPangoTexture);
}

void PangoBasicApp::createPangoContext() {
	if (pango)
		delete pango;
	pango = new kp::pango::Pango();
	// note the request of a 32 bit context, allows RGBA.
	// Not necessary for B&W text!
	context = pango->createContextWithSurface(getWindowWidth(), getWindowHeight(), CAIRO_FORMAT_ARGB32);

	if (layout) {
		delete layout;
	}
	layout = context->createPangoLayout();
	layout->setWidth(getWindowWidth());
	layout->setJustify(false);

	// set a font
	kp::pango::FontDescription fd;
	fd.createFromString("Lucida Grande 22");

	// note font antialias mode supplied here!
	layout->setFontDescription(fd, CAIRO_ANTIALIAS_SUBPIXEL);
}

CINDER_APP(PangoBasicApp, RendererGl)
