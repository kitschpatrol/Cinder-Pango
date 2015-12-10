#pragma once

#include <pango/pangocairo.h>
#include <string>

namespace kp {
namespace pango {

class FontDescription {
public:
	FontDescription();
	void createFromString(std::string sFont);
	void createFromString(const char *pFontSpec);

	void setStyle(PangoStyle nStyle = PANGO_STYLE_NORMAL);
	void setVariant(PangoVariant nVariant = PANGO_VARIANT_NORMAL);
	void setWeight(PangoWeight nWeight = PANGO_WEIGHT_NORMAL);
	void setStretch(PangoStretch nStretch = PANGO_STRETCH_NORMAL);
	PangoFontDescription *pa_description;

private:
};
}
} // namespace kp::pango