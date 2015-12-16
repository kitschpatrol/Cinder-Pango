#include "CairoSurface.h"
#include "cinder/Log.h"

using namespace kp::pango;

CairoSurface::CairoSurface(float fWidth, float fHeight, cairo_format_t nType) {
	// TODO cairo_image_surface_create_for_data instead, passing in surface or texture data directly?
	cr_surface = cairo_image_surface_create(nType, fWidth, fHeight);
	pixels = getWidth() * getHeight(); // we assume RGBA for now!

	// needed?
	new_data = new unsigned char[pixels * 4];
}

CairoSurface::~CairoSurface() {
	cairo_surface_destroy(cr_surface);
	cairo_surface_finish(cr_surface);
	delete[] new_data;
}

void CairoSurface::ref(std::string s) {
	CI_LOG_V("+ CairoSurface (" << s << ")"
															<< "ref count: " << cairo_surface_get_reference_count(cr_surface) << " on this: " << this);
}

int CairoSurface::getWidth() {
	return cairo_image_surface_get_width(cr_surface);
}

int CairoSurface::getHeight() {
	return cairo_image_surface_get_height(cr_surface);
}

unsigned char *CairoSurface::getPixelsARGB() {
	return cairo_image_surface_get_data(cr_surface);
}

unsigned char *CairoSurface::getPixelsRGBA() {
	uint32_t *data = (uint32_t *)cairo_image_surface_get_data(cr_surface);

	for (int i = 0; i < getWidth(); ++i) {
		for (int j = 0; j < getHeight(); ++j) {
			int pos = (j * getWidth()) + i;
			int dest = (j * getWidth() * 4) + i * 4;
			uint32_t p = data[pos];
			int r, g, b, a;
			a = (unsigned char)((p & 0xFF000000) >> 24);
			r = (unsigned char)((p & 0x00FF0000) >> 16);
			g = (unsigned char)((p & 0x0000FF00) >> 8);
			b = (unsigned char)((p & 0x000000FF));
			new_data[dest] = r;
			new_data[dest + 1] = g;
			new_data[dest + 2] = b;
			new_data[dest + 3] = a;
		}
	}
	return new_data;
}
