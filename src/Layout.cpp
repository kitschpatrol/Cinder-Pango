#include "CairoContext.h"
#include "CairoSurface.h"
#include "FontDescription.h"
#include "Layout.h"

using namespace kp::pango;

Layout::Layout(CairoContext *pContext) : context(pContext) {
	pa_layout = pango_cairo_create_layout(pContext->cr_context);
}

Layout::~Layout() {
	delete context;
	g_object_unref(pa_layout);
}

void Layout::setJustify(bool bJustify) {
	pango_layout_set_justify(pa_layout, bJustify);
}

void Layout::setWidth(int nWidth) {
	pango_layout_set_width(pa_layout, nWidth * PANGO_SCALE);
}

void Layout::setText(std::string sText) {
	setText(sText.c_str());
}

void Layout::setText(const char *pText, int nLen) {
	pango_layout_set_text(pa_layout, pText, nLen);
}

void Layout::setMarkup(std::string sText) {
	setMarkup(sText.c_str());
}

void Layout::setMarkup(const char *pText, int nLen) {
	pango_layout_set_markup(pa_layout, pText, nLen);
}

void Layout::show() {
	pango_cairo_show_layout(context->cr_context, pa_layout);
}

float Layout::getTextHeight() {
	PangoRectangle rect = getPixelExtents();
	return rect.height;
}

void Layout::setFontDescription(FontDescription oFont) {
	PangoContext *pc = pango_layout_get_context(pa_layout);
	cairo_font_options_t *options = cairo_font_options_create();
	cairo_font_options_set_antialias(options, CAIRO_ANTIALIAS_DEFAULT);
	cairo_font_options_set_hint_style(options, CAIRO_HINT_STYLE_FULL);
	cairo_font_options_set_hint_metrics(options, CAIRO_HINT_METRICS_ON);

	pango_cairo_context_set_font_options(pc, options);
	cairo_font_options_destroy(options);

	pango_layout_set_font_description(pa_layout, oFont.pa_description);
}

void Layout::setFontDescription(FontDescription oFont, cairo_antialias_t _antialias_type) {
	PangoContext *pc = pango_layout_get_context(pa_layout);
	cairo_font_options_t *options = cairo_font_options_create();
	cairo_font_options_set_antialias(options, _antialias_type);
	cairo_font_options_set_hint_style(options, CAIRO_HINT_STYLE_FULL);
	cairo_font_options_set_hint_metrics(options, CAIRO_HINT_METRICS_ON);

	pango_cairo_context_set_font_options(pc, options);
	cairo_font_options_destroy(options);

	pango_layout_set_font_description(pa_layout, oFont.pa_description);
}

unsigned char *Layout::getPixels() {
	return context->getSurface()->getPixelsARGB();
}

// cinder changes
ci::SurfaceRef Layout::getSurface() {
	unsigned char *pix = getPixels();
	ci::SurfaceRef surface = ci::Surface::create(pix, getWidth(), getHeight(), getWidth() * 4, ci::SurfaceChannelOrder::BGRA);
	//	delete[] pix;
	return surface;
}

ci::gl::Texture2dRef Layout::getTexture() {
	unsigned char *pix = getPixels();
	ci::gl::Texture2dRef texture = ci::gl::Texture2d::create(pix, GL_BGRA, getWidth(), getHeight());
	//	delete[] pix;
	return texture;
}

int Layout::getWidth() {
	return context->getSurface()->getWidth();
}

int Layout::getHeight() {
	return context->getSurface()->getHeight();
}

void Layout::setFont(std::string sFontDesc) {
	FontDescription fd;
	fd.createFromString(sFontDesc);
	setFontDescription(fd);
}

void Layout::fill(float fR, float fG, float fB, float fA) {
	context->color4f(fR, fG, fB, fA);
	context->paint();
}

void Layout::setTextColor(float fR, float fG, float fB, float fA) {
	context->color4f(fR, fG, fB, fA);
}

void Layout::setPangoAlign(int _pangoalign) {
	pango_layout_set_alignment(pa_layout, (PangoAlignment)_pangoalign);
}

void Layout::setAlignLeft() {
	pango_layout_set_alignment(pa_layout, PANGO_ALIGN_LEFT);
}

void Layout::setAlignCenter() {
	pango_layout_set_alignment(pa_layout, PANGO_ALIGN_CENTER);
}

void Layout::setAlignRight() {
	pango_layout_set_alignment(pa_layout, PANGO_ALIGN_RIGHT);
}

PangoRectangle Layout::getPixelExtents() {
	PangoRectangle rect;
	pango_layout_get_pixel_extents(pa_layout, &rect, NULL);
	return rect;
}

void Layout::setSpacing(int _spacing) {
	pango_layout_set_spacing(pa_layout, _spacing * PANGO_SCALE);
}

void Layout::setTabs(std::vector<int> _tabpositions) {

	PangoTabArray *pta = pango_tab_array_new(_tabpositions.size(), true);
	for (int i = 0; i < _tabpositions.size(); i++) {
		pango_tab_array_set_tab(pta, i, PANGO_TAB_LEFT, _tabpositions[i]);
	}
	pango_layout_set_tabs(pa_layout, pta);
	pango_tab_array_free(pta);
}

void Layout::setIndent(int _indent) {
	pango_layout_set_indent(pa_layout, _indent * PANGO_SCALE);
}

ci::vec2 Layout::getPixelSize() {
	ci::vec2 b;
	int w, h;
	pango_layout_get_pixel_size(pa_layout, &w, &h);
	b.x = w;
	b.y = h;
	return b;
}

void Layout::setSingleParagraphMode(bool single) {
	pango_layout_set_single_paragraph_mode(pa_layout, single);
}

int Layout::getLineCount() {
	return pango_layout_get_line_count(pa_layout);
}

ci::vec2 Layout::getPosAtIndex(int _index) {
	PangoRectangle pos;
	pango_layout_index_to_pos(pa_layout, _index, &pos);

	ci::vec2 p;
	p.x = pos.x / PANGO_SCALE;
	p.y = pos.y / PANGO_SCALE;

	return p;
}

int Layout::getIndexAtPos(ci::vec2 _pos) {
	int index;
	int trailing;
	pango_layout_xy_to_index(pa_layout, _pos.x, _pos.y, &index, &trailing);

	return index;
}