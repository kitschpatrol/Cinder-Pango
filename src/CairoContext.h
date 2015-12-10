#pragma once

#include <pango/pangocairo.h>

/**
 * Representation of the Cairo Context. A context is used
 * together with a surface. The context keeps tracks of several
 * settings used to draw onto the surface.
 *
 * @author	Diederick Huijbers <diederick@apollomedia.nl>
 * @version	2010.07.04 - 0.0.1
 */

namespace kp {
namespace pango {

class Layout;
class CairoSurface;
class CairoContext {
public:
	CairoContext(CairoSurface *pSurface);
	~CairoContext();
	void color4f(float fR, float fG, float fB, float fA);
	void color3f(float fR, float fG, float fB);
	void paint();
	void destroy();
	void clear();

	void setIdentityMatrix();
	void translate(double nX, double nY);

	Layout *createPangoLayout();
	CairoSurface *getSurface();

	cairo_t *cr_context;
	CairoSurface *surface;

private:
};
}
} // namespace kp::pango