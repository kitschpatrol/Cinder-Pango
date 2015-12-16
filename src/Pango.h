#pragma once

/**
 * @author	Diederick Huijbers <diederick@apollomedia.nl>
 * @version	0.0.1 2010.07.03
 *
 * <example>
 *
 *  layout = simple->getPango()->createLayout(width,height);
 *	layout->fill(0.0f, 0.0f, 0.0f, 0.0f);
 *	layout->setTextColor(1.0f, 1.0f,10.0f, 1.0f);
 *	layout->setText(oTweet.text);
 *	layout->show();
 *	ofImage img = layout->getImage();
 *
 * </example>
 */

#include "CairoContext.h"
#include "CairoSurface.h"
#include "FontDescription.h"
#include "Layout.h"
#include <pango/pangocairo.h>

namespace kp {
namespace pango {

class Pango {
public:
	Pango();
	CairoContext *createContextWithSurface(float fWidth, float fHeight, cairo_format_t nType = CAIRO_FORMAT_ARGB32);
	Layout *createLayout(float fWidth, float fHeight);

	static std::vector<std::string> getFontList();
	static void logFontList();
	static void loadFont(const ci::fs::path &path);

private:
};
}
} // namespace kp::pango