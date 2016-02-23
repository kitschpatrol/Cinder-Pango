// CinderPango.h
// PangoBasic
//
// Created by Eric Mika on 1/6/16.
//

#pragma once

#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

#ifdef CINDER_MSW
#include "cinder/cairo/Cairo.h"
#endif

#include <fontconfig/fontconfig.h>
#include <pango/pangocairo.h>

#include <vector>

namespace kp {
namespace pango {

// TODO wrap these up?
const bool grayscale = false;
const bool native = false;

enum class TextAlignment : int {
	LEFT,
	CENTER,
	RIGHT,
	JUSTIFY,
};

enum class TextRenderer {
	FREETYPE,
	PLATFORM_NATIVE,
};

enum class TextWeight : int {
	THIN = 100,
	ULTRALIGHT = 200,
	LIGHT = 300,
	SEMILIGHT = 350,
	BOOK = 380,
	NORMAL = 400,
	MEDIUM = 500,
	SEMIBOLD = 600,
	BOLD = 700,
	ULTRABOLD = 800,
	HEAVY = 900,
	ULTRAHEAVY = 1000
};

enum class TextAntialias : int {
	DEFAULT,
	NONE,
	GRAY,
	SUBPIXEL,
};

typedef std::shared_ptr<class CinderPango> CinderPangoRef;

class CinderPango : public std::enable_shared_from_this<CinderPango> {
public:
	static CinderPangoRef create();
	virtual ~CinderPango();

	// Globals
	static std::vector<std::string> getFontList();
	static void logFontList();
	static void loadFont(const ci::fs::path &path);
	static TextRenderer getTextRenderer();
	static void setTextRenderer(TextRenderer renderer);

	// Rendering

	const std::string getText();

	// setText can take inline markup to override the default text settings
	// See here for full list of supported tags:
	// https://developer.gnome.org/pango/stable/PangoMarkupFormat.html
	void setText(std::string text);

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

	void setDefaultTextStyle(std::string font = "Sans", float size = 12.0, ci::ColorA color = ci::Color::black(), TextWeight weight = TextWeight::NORMAL,
													 TextAlignment alignment = TextAlignment::LEFT); // convenience

	ci::ColorA getDefaultTextColor();
	void setDefaultTextColor(ci::ColorA color);

	ci::ColorA getBackgroundColor();
	void setBackgroundColor(ci::ColorA color);

	float getDefaultTextSize();
	void setDefaultTextSize(float size);

	std::string getDefaultTextFont();
	void setDefaultTextFont(std::string font);

	TextWeight getDefaultTextWeight();
	void setDefaultTextWeight(TextWeight weight);

	TextAntialias getTextAntialias();
	void setTextAntialias(TextAntialias mode);

	TextAlignment getTextAlignment();
	void setTextAlignment(TextAlignment alignment);

	bool getDefaultTextSmallCapsEnabled();
	void setDefaultTextSmallCapsEnabled(bool value);

	bool getDefaultTextItalicsEnabled();
	void setDefaultTextItalicsEnabled(bool value);

	float getSpacing();
	void setSpacing(float spacing);

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
	std::string mProcessedText; // stores text after newline filtering
	bool mProbablyHasMarkup;
	ci::ivec2 mMinSize;
	ci::ivec2 mMaxSize;

	// TODO wrap these up...
	std::string mDefaultTextFont;
	bool mDefaultTextItalicsEnabled;
	bool mDefaultTextSmallCapsEnabled;
	ci::ColorA mDefaultTextColor;
	ci::ColorA mBackgroundColor;
	float mDefaultTextSize;
	TextAlignment mTextAlignment;
	TextWeight mDefaultTextWeight;
	TextAntialias mTextAntialias;
	float mSpacing;

	// Internal flags for state invalidation
	// Used by render method
	bool mNeedsFontUpdate;
	bool mNeedsMeasuring;
	bool mNeedsTextRender;
	bool mNeedsFontOptionUpdate;
	bool mNeedsMarkupDetection;

	// Pango references
	PangoFontMap *fontMap;
	PangoContext *pangoContext;
	PangoLayout *pangoLayout;
	PangoFontDescription *fontDescription;
	cairo_surface_t *cairoSurface;
	cairo_t *cairoContext;
	cairo_font_options_t *cairoFontOptions;

#ifdef CAIRO_HAS_WIN32_SURFACE
	cairo_surface_t *cairoImageSurface;
#endif
};
}
} // namespace kp::pango
