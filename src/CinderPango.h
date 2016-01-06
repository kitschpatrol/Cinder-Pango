// CinderPango.h
// PangoBasic
//
// Created by Eric Mika on 1/6/16.
//

#pragma once

#include "cinder/Cinder.h"
#include <fontconfig/fontconfig.h>
#include <pango/pangocairo.h>

namespace kp {
namespace pango {

// TODO wrap these up?
const bool grayscale = false;
const bool native = false;

enum class TextAlignment : int {
	LEFT,
	CENTER,
	RIGHT,
};

enum class TextRenderer {
	FREETYPE,
	PLATFORM_NATIVE,
};

typedef std::shared_ptr<class CinderPango> CinderPangoRef;

class CinderPango : public std::enable_shared_from_this<CinderPango> {
public:
	static CinderPangoRef create();
	~CinderPango();

	// Globals
	static std::vector<std::string> getFontList();
	static void logFontList();
	static void loadFont(const ci::fs::path &path);
	static TextRenderer getTextRenderer();
	static void setTextRenderer(TextRenderer renderer);

	// Rendering

	const std::string getText();
	void setText(std::string text); // markup welcome

	// Text is rendered into this texture
	const ci::gl::TextureRef getTexture();

	// Text smaller than the min size will be clipped
	ci::ivec2 getMinSize();
	void setMinSize(int minWidth, int minHeight);
	void setMinSize(ci::ivec2 minSize);

	// Text can grow up to this size before a line breaks or clipping begins
	ci::ivec2 getMaxSize();
	void setMaxSize(int maxWidth, int maxHeight);
	void setMaxSize(ci::ivec2 maxSize);

	// Setting default font styles is more efficient than passing markup via the text string
	ci::ColorA getDefaultTextColor();
	void setDefaultTextColor(ci::ColorA color);

	float getDefaultTextSize();
	void setDefaultTextSize(float size);

	std::string getDefaultTextFont();
	void setDefaultTextFont(std::string font);

	TextAlignment getTextAlignment();
	void setTextAlignment(TextAlignment alignment);

	// Renders text into the texture.
	// Returns true if the texture was actually pdated, false if nothing had to change
	// It's reasonable (and more efficient) to just run this in an update loop rather than calling it
	// explicitly after every change to the text state. It will coalesce all invalidations since the
	// last frame and only rebuild what needs to be rebuilt to render the diff.
	// Set force to true to render even if the system thinks state wasn't invalidated.
	bool render(bool force = false);

protected:
	CinderPango();

private:
	ci::gl::TextureRef mTexture;
	std::string mText;
	ci::ivec2 mMinSize;
	ci::ivec2 mMaxSize;
	std::string mDefaultTextFont;
	ci::ColorA mDefaultTextColor;
	float mDefaultTextSize;
	TextAlignment mTextAlignment;

	// Internal flags for state invalidation
	// Used by render method
	bool mNeedsFontUpdate;
	bool mNeedsMeasuring;
	bool mNeedsTextRender;

	// Pango references
	PangoFontMap *fontMap;
	PangoContext *pangoContext;
	PangoLayout *pangoLayout;
	PangoFontDescription *fontDescription;
	cairo_surface_t *cairoSurface;
	cairo_t *cairoContext;
};
}
} // namespace kp::pango
