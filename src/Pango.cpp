#include "CairoContext.h"
#include "CairoSurface.h"
#include "Pango.h"

using namespace kp::pango;

Pango::Pango() {
}

CairoContext *Pango::createContextWithSurface(float fWidth, float fHeight, cairo_format_t nType) {
	CairoSurface *surface = new CairoSurface(fWidth, fHeight, nType);
	CairoContext *context = new CairoContext(surface);
	return context;
}

/**
 * A layout is used to align text onto a surface. For example if you want to
 * right align text, you can use a layout together with a surface. This method
 * sets up exactly what you need for this.
 *
 */
Layout *Pango::createLayout(float fWidth, float fHeight) {
	CairoContext *context = createContextWithSurface(fWidth, fHeight);
	Layout *layout = context->createPangoLayout();
	layout->setWidth(fWidth);
	return layout;
}

void Pango::listFonts() {
	// http: // www.lemoda.net/pango/list-fonts/
	int i;
	PangoFontFamily **families;
	int n_families;
	PangoFontMap *fontmap;

	fontmap = pango_cairo_font_map_get_default();
	pango_font_map_list_families(fontmap, &families, &n_families);
	printf("There are %d families\n", n_families);
	for (i = 0; i < n_families; i++) {
		PangoFontFamily *family = families[i];
		const char *family_name;

		family_name = pango_font_family_get_name(family);
		printf("Family %d: %s\n", i, family_name);
	}
	g_free(families);
}