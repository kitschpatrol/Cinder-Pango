#include "cinder/Cinder.h"
#include "cinder/Log.h"
#include "CairoContext.h"
#include "Layout.h"
#include "CairoSurface.h"

using namespace kp::pango;

CairoContext::CairoContext(CairoSurface *pSurface) : surface(pSurface), cr_context(NULL) {
	cr_context = cairo_create(pSurface->cr_surface);
	if (cairo_status(cr_context) != CAIRO_STATUS_SUCCESS) {
		CI_LOG_E("Error creating the pango cairo context");
	}

	// Flip vertically... wasn't necessary in OF
	cairo_scale(cr_context, 1.0f, -1.0f);
	cairo_translate(cr_context, 0.0f, -this->getSurface()->getHeight());
}

CairoContext::~CairoContext() {
	destroy();
	delete surface;
}

Layout *CairoContext::createPangoLayout() {
	Layout *playout = new Layout(this);
	return playout;
}

void CairoContext::clear() {
	cairo_save(cr_context);
	cairo_set_source_rgba(cr_context, 0, 0, 0, 0);
	cairo_set_operator(cr_context, CAIRO_OPERATOR_SOURCE);
	cairo_paint(cr_context);
	cairo_restore(cr_context);
}

void CairoContext::color4f(float fR, float fG, float fB, float fA) {
	cairo_set_source_rgba(cr_context, fR, fG, fB, fA);
}

void CairoContext::color3f(float fR, float fG, float fB) {
	cairo_set_source_rgb(cr_context, fR, fG, fB);
}

void CairoContext::paint() {
	cairo_paint(cr_context);
}

void CairoContext::destroy() {
	cairo_destroy(cr_context);
}

CairoSurface *CairoContext::getSurface() {
	return surface;
}

void CairoContext::setIdentityMatrix() {
	cairo_identity_matrix(cr_context);
}

void CairoContext::translate(double nX, double nY) {
	cairo_translate(cr_context, nX, nY);
}