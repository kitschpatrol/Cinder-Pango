#pragma once

#include <pango/pangocairo.h>
#include <string>

namespace kp {
namespace pango {

class CairoSurface {
public:
	CairoSurface(float fWidth, float fHeight, cairo_format_t nType = CAIRO_FORMAT_ARGB32);
	~CairoSurface();
	int getWidth();
	int getHeight();
	unsigned char *getPixelsARGB(); /// no conversion, you get what you asked for when creating the surface
																	//	unsigned char *getPixelsRGBA(); /// this converts ARGB to RGBA so its expensive
	void ref(std::string s = "");
	cairo_surface_t *cr_surface;

private:
	unsigned char *new_data;
	int pixels;
};
}
} // namespace kp::pango