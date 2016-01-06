//// Via http://www.roxlu.com/2014/046/rendering-text-with-pango--cairo-and-freetype
// No folor with FT2 renderer?

/*

 Text layouting and rendering with Pango
 ---------------------------------------

 This code snippet shows how to create a cairo surface and
 render some text into it using Pango. We store the generated
 pixels in a png file.

 n.b. this file was created for testing not production.

 */

#include <cairo.h>
#include <freetype2/ftbitmap.h>
#include <pango/pangocairo.h>
#include <pango/pangoft2.h>
#include <stdio.h>
#include <stdlib.h>

// static
// list fonts
// add font

// invalidating
// set anti-aliasing
// set pixel format
// Set font
// set justification
// set size
// set color
// set render mode (native or freetype)

// Render

int main() {
	const bool grayscale = false;
	const bool native = false;

	

	
	if (native) {
		setenv("PANGOCAIRO_BACKEND", "coretext", 0);
	} else {
		setenv("PANGOCAIRO_BACKEND", "fontconfig", 0); // this fixes some font issues on  mac
	}

	/* ------------------------------------------------------------ */
	/*                   I N I T I A L I Z E                        */
	/* ------------------------------------------------------------ */

	// measure
	PangoFontMap *font_map = NULL;
	font_map = pango_cairo_font_map_new();
	if (font_map == NULL) {
		printf("+ error: cannot create the pango font map.\n");
		exit(EXIT_FAILURE);
	}

	PangoContext *context = NULL;
	context = pango_font_map_create_context(font_map);
	if (NULL == context) {
		printf("+ error: cannot create pango font context.\n");
		exit(EXIT_FAILURE);
	}

	/* create layout object. */
	PangoLayout *layout = NULL;
	layout = pango_layout_new(context);

	if (NULL == layout) {
		printf("+ error: cannot create the pango layout.\n");
		exit(EXIT_FAILURE);
	}

	/* create the font description @todo the reference does not tell how/when to free this */
	PangoFontDescription *font_desc = NULL;
	font_desc = pango_font_description_from_string("Times 35");
	pango_layout_set_font_description(layout, font_desc);
	pango_font_map_load_font(font_map, context, font_desc);
	pango_font_description_free(font_desc);

	/* set the width around which pango will wrap */
	pango_layout_set_width(layout, 200 * PANGO_SCALE);
	pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);

	/* write using the markup feature */
	const gchar *text = ""
											"<span foreground=\"blue\" font_family=\"Gravur Condensed Pro\">"
											"<b>bold</b> <u>is</u> <i>nicé</i> "
											"</span>"
											"<tt> hello </tt>"
											"<span font_family=\"sans\" font_stretch=\"ultracondensed\" letter_spacing=\"500\" font_weight=\"light\"> SANS</span>"
											"<span foreground=\"#FFCC00\"> colored</span>"
											"";

	// Measure
	pango_layout_set_markup(layout, text, -1);

	int pixelWidth = 0;
	int pixelHeight = 0;
	pango_layout_get_pixel_size(layout, &pixelWidth, &pixelHeight);

	printf("Width: %i Height: %i\n", pixelWidth, pixelHeight);

	// draw into cairo
	_cairo_format cairoFormat = grayscale ? CAIRO_FORMAT_A8 : CAIRO_FORMAT_ARGB32;

	cairo_surface_t *surf = NULL;
	surf = cairo_image_surface_create(cairoFormat, pixelWidth, pixelHeight);

	if (CAIRO_STATUS_SUCCESS != cairo_surface_status(surf)) {
		printf("+ error: couldn't create the surface.\n");
		exit(EXIT_FAILURE);
	}

	/* create our cairo context object that tracks state. */
	cairo_t *cr = cairo_create(surf);
	if (CAIRO_STATUS_NO_MEMORY == cairo_status(cr)) {
		printf("+ error: out of memory, cannot create cairo_t*\n");
		exit(EXIT_FAILURE);
	}

	cairo_move_to(cr, 0, 0);
	pango_cairo_update_layout(cr, layout);
	pango_cairo_show_layout(cr, layout);

	/* ------------------------------------------------------------ */
	/*               O U T P U T  A N D  C L E A N U P              */
	/* ------------------------------------------------------------ */

	/* write to png */
	cairo_status_t status;
	status = cairo_surface_write_to_png(surf, "test_font.png");
	if (CAIRO_STATUS_SUCCESS != status) {
		printf("+ error: couldn't write to png\n");
		exit(EXIT_FAILURE);
	}

	// destroy bmp?
	cairo_surface_destroy(surf);
	cairo_destroy(cr);

	g_object_unref(layout);
	g_object_unref(font_map);
	g_object_unref(context);

	return 0;
}

int main2() {
	setenv("PANGOCAIRO_BACKEND", "fontconfig", 0); // this fixes some font issues on  mac

	PangoFontMap *fontMap = pango_cairo_font_map_get_default();
	PangoContext *pangoContext = pango_font_map_create_context(fontMap);
	PangoLayout *pangoLayout = pango_layout_new(pangoContext);

	float maxWidth = 200;

	pango_layout_set_width(pangoLayout, maxWidth * PANGO_SCALE);

	const gchar *text = ""
											"<span foreground=\"blue\" font_family=\"Gravur Condensed Pro\">"
											"<b>bold</b> <u>is</u> <i>nicé</i> "
											"</span>"
											"<tt> hello </tt>"
											"<span font_family=\"sans\" font_stretch=\"ultracondensed\" letter_spacing=\"500\" font_weight=\"light\"> SANS</span>"
											"<span foreground=\"#FFCC00\"> colored</span>"
											"";

	pango_layout_set_markup(pangoLayout, text, -1);

	int width = 0;
	int height = 0;
	pango_layout_get_pixel_size(pangoLayout, &width, &height);

	printf("Width: %i Height: %i\n", width, height);

	cairo_surface_t *cairoSurface = NULL;
	cairoSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

	if (CAIRO_STATUS_SUCCESS != cairo_surface_status(cairoSurface)) {
		printf("+ error: couldn't create the surface.\n");
		exit(EXIT_FAILURE);
	}

	/* create our cairo context object that tracks state. */
	cairo_t *cairoContext = NULL;
	cairoContext = cairo_create(cairoSurface);
	if (CAIRO_STATUS_NO_MEMORY == cairo_status(cairoContext)) {
		printf("+ error: out of memory, cannot create cairo_t*\n");
		exit(EXIT_FAILURE);
	}

	cairo_move_to(cairoContext, 0, 0);
	pango_cairo_update_layout(cairoContext, pangoLayout);
	pango_cairo_show_layout(cairoContext, pangoLayout);

	/* write to png */
	cairo_status_t status;
	status = cairo_surface_write_to_png(cairoSurface, "test_font.png");
	if (CAIRO_STATUS_SUCCESS != status) {
		printf("+ error: couldn't write to png\n");
		exit(EXIT_FAILURE);
	}

	// destroy bmp?
	cairo_surface_destroy(cairoSurface);
	cairo_destroy(cairoContext);

	g_object_unref(pangoLayout);
	g_object_unref(fontMap);
	g_object_unref(pangoContext);

	return 0;
}

int main3() {

	setenv("PANGOCAIRO_BACKEND", "fontconfig", 0); // this fixes some font issues on  mac
	// setenv("PANGOCAIRO_BACKEND", "coretext", 0);

	const bool native = true; // deprecated, keep this true
	const bool grayscale = false;

	cairo_surface_t *surf = NULL;
	cairo_t *cr = NULL;
	cairo_status_t status;
	PangoContext *context = NULL;
	PangoLayout *layout = NULL;
	PangoFontDescription *font_desc = NULL;
	PangoFontMap *font_map = NULL;
	FT_Bitmap bmp = {0};

	//	int stride = 0;
	int width = 640;
	int height = 480;

	/* ------------------------------------------------------------ */
	/*                   I N I T I A L I Z E                        */
	/* ------------------------------------------------------------ */

	/* FT buffer */
	//	FT_Bitmap_New(&bmp);
	//	bmp.rows = height;
	//	bmp.width = width;
	//
	//	bmp.buffer = (unsigned char *)malloc(bmp.rows * bmp.width);
	//	if (NULL == bmp.buffer) {
	//		printf("+ error: cannot allocate the buffer for the output bitmap.\n");
	//		exit(EXIT_FAILURE);
	//	}
	//
	//	/* create our "canvas" */
	//	bmp.pitch = (width + 3) & -4;
	//	bmp.pixel_mode = FT_PIXEL_MODE_BGRA; /*< Grayscale*/
	//	stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
	//	surf = cairo_image_surface_create_for_data(bmp.buffer, CAIRO_FORMAT_ARGB32, width, height, stride);

	_cairo_format cairoFormat = grayscale ? CAIRO_FORMAT_A8 : CAIRO_FORMAT_ARGB32;

	if (native) {
		surf = cairo_image_surface_create(cairoFormat, width, height);
	} else {
		// TODO

		// FT_Bitmap_Convert?

		FT_Pixel_Mode_ ftFormat = grayscale ? FT_PIXEL_MODE_GRAY : FT_PIXEL_MODE_BGRA;

		int bpp = grayscale ? 1 : 4;
		int pitch = width * bpp + bpp & ~bpp;

		FT_Bitmap_New(&bmp);
		bmp.width = width;
		bmp.rows = height;
		bmp.buffer = (unsigned char *)malloc(pitch * height);
		bmp.pixel_mode = ftFormat;
		if (grayscale) {
			bmp.num_grays = 256;
		}
		bmp.pitch = pitch;

		int stride = cairo_format_stride_for_width(cairoFormat, width);
		surf = cairo_image_surface_create_for_data(bmp.buffer, cairoFormat, width, height, stride);
	}

	if (CAIRO_STATUS_SUCCESS != cairo_surface_status(surf)) {
		printf("+ error: couldn't create the surface.\n");
		exit(EXIT_FAILURE);
	}

	/* create our cairo context object that tracks state. */
	cr = cairo_create(surf);
	if (CAIRO_STATUS_NO_MEMORY == cairo_status(cr)) {
		printf("+ error: out of memory, cannot create cairo_t*\n");
		exit(EXIT_FAILURE);
	}

	/* ------------------------------------------------------------ */
	/*               D R A W   I N T O  C A N V A S                 */
	/* ------------------------------------------------------------ */

	if (native) {
		font_map = pango_cairo_font_map_new();
	} else {
		font_map = pango_ft2_font_map_new();
	}

	if (NULL == font_map) {
		printf("+ error: cannot create the pango font map.\n");
		exit(EXIT_FAILURE);
	}

	context = pango_font_map_create_context(font_map);
	if (NULL == context) {
		printf("+ error: cannot create pango font context.\n");
		exit(EXIT_FAILURE);
	}

	/* create layout object. */
	layout = pango_layout_new(context);

	if (NULL == layout) {
		printf("+ error: cannot create the pango layout.\n");
		exit(EXIT_FAILURE);
	}

	/* create the font description @todo the reference does not tell how/when to free this */
	font_desc = pango_font_description_from_string("Times 35");
	pango_layout_set_font_description(layout, font_desc);
	pango_font_map_load_font(font_map, context, font_desc);
	pango_font_description_free(font_desc);

	/* set the width around which pango will wrap */
	pango_layout_set_width(layout, width * PANGO_SCALE);
	pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);

	/* write using the markup feature */
	const gchar *text = ""
											"<span foreground=\"blue\" font_family=\"Gravur Condensed Pro\">"
											"<b>bold</b> <u>is</u> <i>nicé</i> "
											"</span>"
											"<tt> hello </tt>"
											"<span font_family=\"sans\" font_stretch=\"ultracondensed\" letter_spacing=\"500\" font_weight=\"light\"> SANS</span>"
											"<span foreground=\"#FFCC00\"> colored</span>"
											"";

	pango_layout_set_markup(layout, text, -1);

	/* render */

	if (!native) {
		pango_ft2_render_layout(&bmp, layout, 0, 0);
	}

	pango_cairo_update_layout(cr, layout);

	if (native) {
		pango_cairo_show_layout(cr, layout);
	}

	/* ------------------------------------------------------------ */
	/*               O U T P U T  A N D  C L E A N U P              */
	/* ------------------------------------------------------------ */

	/* write to png */
	status = cairo_surface_write_to_png(surf, "test_font.png");
	if (CAIRO_STATUS_SUCCESS != status) {
		printf("+ error: couldn't write to png\n");
		exit(EXIT_FAILURE);
	}

	// destroy bmp?
	cairo_surface_destroy(surf);
	cairo_destroy(cr);

	g_object_unref(layout);
	g_object_unref(font_map);
	g_object_unref(context);

	return 0;
}

//#include <glib.h>
//#include <math.h>
//#include <pango/pangocairo.h>
//
// static void draw_text(cairo_t *cr) {
//#define RADIUS 150
//#define N_WORDS 10
//#define FONT "Gravur Condensed Pro 30"
//
//	PangoLayout *layout;
//	PangoFontDescription *desc;
//	int i;
//
//	/* Center coordinates on the middle of the region we are drawing
//	 */
//	cairo_translate(cr, RADIUS, RADIUS);
//
//	/* Create a PangoLayout, set the font and text */
//
//	pango_layout_set_text(layout, "Text", -1);
//	desc = pango_font_description_from_string(FONT);
//	pango_layout_set_font_description(layout, desc);
//
//	pango_font_description_free(desc);
//
//	//	pango_font_description_free(font_desc);
//
//	/* Draw the layout N_WORDS times in a circle */
//	for (i = 0; i < N_WORDS; i++) {
//		int width, height;
//		double angle = (360. * i) / N_WORDS;
//		double red;
//
//		cairo_save(cr);
//
//		/* Gradient from red at angle == 60 to blue at angle == 240 */
//		red = (1 + cos((angle - 60) * G_PI / 180.)) / 2;
//		cairo_set_source_rgb(cr, red, 0, 1.0 - red);
//
//		cairo_rotate(cr, angle * G_PI / 180.);
//
//		/* Inform Pango to re-layout the text with the new transformation */
//		pango_cairo_update_layout(cr, layout);
//
//		pango_layout_get_size(layout, &width, &height);
//		cairo_move_to(cr, -((double)width / PANGO_SCALE) / 2, -RADIUS);
//		pango_cairo_show_layout(cr, layout);
//
//		cairo_restore(cr);
//	}
//
//	/* free the layout object */
//	g_object_unref(layout);
//}
//
// int main(int argc, char **argv) {
//
//	FcConfig *fontConfig = FcConfigCreate();
//
//	//	const FcChar8 *file = (const FcChar8 *)"/Users/mika/Code/tech-museum-bio.wetlab/WorkbenchTabletOF/bin/data/fonts/GravurCondensedPro-Regular.ttf";
//	const FcChar8 *file = (const FcChar8 *)"/Users/mika/Documents/Fonts/Gravur Condensed Pro/GravurCondensedPro-Regular.ttf";
//	//	const FcChar8 *file = (const FcChar8 *)"/Users/mika/Documents/Fonts/Trade Gothic LT Std/TradeGothicLTStd-Bd2.otf";
//	//
//	FcBool fontAddStatus = FcConfigAppFontAddFile(fontConfig, file);
//
//	if (fontAddStatus) {
//		g_print("added font successfully");
//	}
//
//	FcConfigSetCurrent(fontConfig);
//
//	// Print fonts
//
//	FcFontSet *fonts = FcConfigGetFonts(FcConfigGetCurrent(), FcSetApplication);
//
//	printf("Font count: %i", fonts->nfont);
//
//	FcPatternPrint(fonts->fonts[0]);
//
//	PangoContext *context;
//	cairo_t *cr;
//	char *filename;
//	cairo_status_t status;
//	cairo_surface_t *surface;
//
//	if (argc != 2) {
//		g_printerr("Usage: cairosimple OUTPUT_FILENAME\n");
//		return 1;
//	}
//
//	filename = argv[1];
//
//	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 2 * RADIUS, 2 * RADIUS);
//	cr = cairo_create(surface);
//
//	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
//	cairo_paint(cr);
//	draw_text(cr);
//	cairo_destroy(cr);
//
//	status = cairo_surface_write_to_png(surface, filename);
//	cairo_surface_destroy(surface);
//
//	if (status != CAIRO_STATUS_SUCCESS) {
//		g_printerr("Could not save png to '%s' status %i\n", filename, status);
//		return 1;
//	}
//
//	return 0;
//}
