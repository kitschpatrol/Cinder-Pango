#pragma once

#include "cinder/Cinder.h"
#include "cinder/Surface.h"
#include "cinder/gl/gl.h"

#include <pango/pangocairo.h>

namespace kp {
namespace pango {

class FontDescription;
class CairoContext;
class Layout {
public:
	Layout(CairoContext *pContext);
	~Layout();

	unsigned char *getPixels();
	ci::SurfaceRef getSurface();

	ci::gl::Texture2dRef getTexture(); // cinder changes
	void setJustify(bool bJustify);
	void setWidth(int nWidth);
	void setText(std::string sText);
	void setText(const char *pText, int nLen = -1);
	void setMarkup(std::string sText);
	void setMarkup(const char *pText, int nLen = -1);
	void show(); // not sure if this is the best location for this function
	void setFontDescription(FontDescription oFont);
	int getWidth();
	int getHeight();
	float getTextHeight();
	void setFont(std::string sFontDesc);
	void fill(float fR, float fG, float fB, float fA = 1.0f);
	void setTextColor(float fR, float fG, float fB, float fA = 1.0f);

	void setPangoAlign(int _pangoalign);
	void setAlignLeft();
	void setAlignRight();
	void setAlignCenter();

	ci::vec2 getPosAtIndex(int _index);
	int getIndexAtPos(ci::vec2 _pos);

	// RR-PM new 28.Nov 2012
	void setFontDescription(FontDescription oFont, cairo_antialias_t _antialias_type);

	// RR new 10.Juli 2012
	void setSpacing(int _spacing);
	void setSingleParagraphMode(bool single);
	void setTabs(std::vector<int> _tabpositions);
	void setIndent(int _indent);

	int getLineCount();

	ci::vec2 getPixelSize();
	PangoRectangle getPixelExtents();
	CairoContext *context;
	PangoLayout *pa_layout;

private:
};
}
} // namespace kp::pango