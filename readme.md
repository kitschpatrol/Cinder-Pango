#Cinder-Pango

**A wrapper for using the [Pango](http://www.pango.org) text layout and rendering library with [Cinder](https://libcinder.org). Supports [inline style markup](https://developer.gnome.org/pango/stable/PangoMarkupFormat.html) and complex layouts that would be tedious to achieve with Cinder's built-in TextBox and TextLayout classes.**


## Getting Started
Take a look at the PangoBasic example.

This block has an obscene number of dependencies. Using Tinderbox or similar to integrate it with your project is recommended.


## Origin

This block was inspired by the [openFrameworks](http://openframeworks.cc) addon [ofxPango](https://github.com/kitschpatrol/ofxpango), though the API has changed significantly to hide more of Pango and expose a more Cinder-esque interface.

Thanks to [Oriol Ferrer Mesià](http://uri.cat/), [Diederick Huijbers](http://www.roxlu.com), [Matthias Rohrbach](http://www.robandrose.ch), and others.

## Tips

Alpha channels in textures returned from Cinder-Pango are premultiplied. Call `gl::enableAlphaBlendingPremult();` before drawing the texture. If you're seeing strange artifacts around text on different colored backgrounds, this is probably why.

## Compatibility

Tested against the [Cinder master branch](https://github.com/cinder/Cinder/commit/02089928b3982f866a77a9e6e2168075f9f9e6f6) (v9.1).

Tinderbox-generated project files are known to work on:

- Mac OS X 10.11 x64 with Xcode 7.2
- Windows 10 x64 with Visual Studio 2015 Community.

This library was not built with an eye towards backwards compatibility. It's probably relatively trivially achievable by rebuilding the dependencies, but this isn't currently a priority.

## Known Issues

### Mac

Some fonts may not render correctly on Mac OS X, they'll just render as a default serif or sans instead of what you requested. I haven't tracked down exactly how / why this is, but as a work-around you can tell PangoCairo to render glyphs with Freetype instead of the native CoreText library by setting the `PANGOCAIRO_BACKEND` environment variable. There's a method in the API to do this globally:

	CinderPango::setTextRenderer(TextRenderer::FREETYPE); // this works around some font issues on  mac


The TextRenderer::PLATFORM_NATIVE (coretext) backend renderer leaks memory through _pango_core_text_shape, _pango_cairo_core_text_font_new and other methods. The TextRenderer::FREETYPE backend renderer is comparatively free of leaks.


### Xcode

If you get a bunch of `ld: warning: object file (../../../lib/macosx/libcairo.a(cairo.o)) was built for newer OSX version (10.11) than being linked (10.8)` warnings, then increase the minimum deploy target to 10.11.

### Visual Studio

Tinderbox enables the /gm flag in Visual Studio projects by default, which will conflicts with the /mp flag. Disable /gm to take care of this.

There's an issue with the GTK DLLs afflicting Win32 bit builds. Win32 is currently not supported.

### Windows

The environmental variable set by setTextRenderer doesn't seem to do anything on Windows. Instead, I think rendering is determined by the type of cairo surface you draw into. The win32 surface support is compiled into Cinder's core Cairo block, but isn't present in the DLLs included in this library. Currently, if you import Cinder's cairo block before importing CinderPango, then CinderPango will automatically use win32 surfaces instead of what (I assume) is the freetype backend to blit the glyphs.

Font loading from a local file doesn't work (yet) on Windows. You have to install any fonts you'd like to use at the system level.

There are some issues with text rendering quality on Windows that I'm trying to work through.

On Windows, antialiasing and some other properties have no effect.


## Dependencies

**All dependencies are included in the block.**

On Mac, dependencies were built through [Homebrew](http://brew.sh). Two formulae had to be modified to build static libs instead of dylibs. The modified formulae are included in this repo. Here's how to build them if necessary:

	// from the /docs/homebrew folder in this repo
	brew rm cairo
	brew rm pango
	brew rm harfbuzz
	brew install cairo --universal
	brew install pango.rb --universal
	brew install harfbuzz.rb --with-cairo --universal

On Windows, dependencies were compiled elsewhere.

Windows 64 bit libs procured via [gtk+-bundle_3.6.4-20130513_win64.zip](http://www.tarnyko.net/dl/gtk.htm) and [gtk2 installer](http://tschoonj.github.io/blog/2014/09/29/gtk2-64-bit-windows-runtime-environment-installer-now-on-github). (The latter had some issues... missing lib files.) See also another alternative [gtk-win64 build](http://lvserver.ugent.be/gtk-win64/).

Windows 32 bit libs from here and there, currently not working.

##TODO
- Consolidate renderer property across platforms.
- Windows local font loading.
- Document and warn about platform-invalid parameters. (E.g. anti-alaising settings on Windows.)
- Wrap more of the API. (Hyphenation, hinting, etc.)
- Wrap certain attributes normally relegated to markup... letterspacing, underline, etc?
- Make alpha optional.
- Cross-platform font size unification like Cinder's font system? See pango_cairo_context_set_resolution.
- High DPI stuff.
- Try blitting glyphs directly into a surface or texture by passing a reference to to the pixel data to `cairo_image_surface_create_for_data` instead of creating a new one via `cairo_image_surface_create`.
- Figure out why some fonts don't load correctly on Mac + CoreText.
- Use the Cinder Cairo block instead of including Cairo.
- 32 bit Windows support. (Some issue with the GTK DLLs.)
- Fix libraries for 32 bit Mac.
