<!-- title({ prefix: "Cinder-" }) -->

# Cinder-Pango

<!-- /title -->

<!-- badges -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/license/mit)

<!-- /badges -->

<!-- short-description -->

**CinderBlock for using the Pango text layout and rendering library with Cinder. Supports inline style markup and complex layouts that would be tedious to achieve with Cinder's built-in TextBox and TextLayout classes.**

<!-- /short-description -->

## Overview

A [CinderBlock](https://libcinder.org/docs/guides/cinder-blocks/index.html) that wraps the [Pango](https://www.pango.org) text layout and rendering library for [Cinder](https://libcinder.org/). Text can include Pango's [inline style markup](https://docs.gtk.org/Pango/pango_markup.html) to mix fonts, sizes, colors, and styles in a single layout. Pango lays out the text, Cairo draws it, and the result is returned as a `ci::gl::TextureRef`.

## Getting started

### Dependencies

Requires Cinder on macOS or Windows.

**All dependencies are included in the block.** Headers for Pango, Cairo, GLib, HarfBuzz, Fontconfig, FreeType, libpng, and pixman are in [`include`](include). Prebuilt static libraries for macOS (x86_64) are in [`lib/macosx`](lib/macosx), and import libraries and DLLs for Windows are in [`lib/msw`](lib/msw).

This block has an obscene number of dependencies. Using TinderBox or similar to integrate it with your project is recommended.

On Windows, `CinderPango.h` also includes `cinder/cairo/Cairo.h`, so add Cinder's own [Cairo block](https://github.com/cinder/Cinder/tree/master/blocks/Cairo) to the project as well. Its headers enable win32 surfaces (see [Implementation notes](#implementation-notes)).

Tested against the [Cinder master branch](https://github.com/cinder/Cinder/commit/02089928b3982f866a77a9e6e2168075f9f9e6f6) (0.9.1dev).

TinderBox-generated project files are known to work on:

- Mac OS X 10.11 x64 with Xcode 7.2
- Windows 10 x64 with Visual Studio 2015 Community

This library was not built with an eye towards backwards compatibility. It's probably relatively trivially achievable by rebuilding the dependencies, but this isn't currently a priority.

### Installation

From your Cinder `blocks` directory:

```sh
git clone https://github.com/kitschpatrol/Cinder-Pango.git
```

Then add **Pango** to a project with [TinderBox](https://libcinder.org/docs/guides/tinderbox/index.html).

In Xcode, if you get a bunch of `ld: warning: object file (../../../lib/macosx/libcairo.a(cairo.o)) was built for newer OSX version (10.11) than being linked (10.8)` warnings, then increase the minimum deployment target to 10.11.

TinderBox enables the `/Gm` flag in Visual Studio projects by default, which conflicts with the `/MP` flag. Disable `/Gm` to take care of this.

## Usage

Take a look at the PangoBasic example. A minimal app creates a `CinderPango` instance, gives it some text, and draws its texture:

```cpp
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"

#include "CinderPango.h"

class MyApp : public ci::app::App {
public:
	void setup() override {
		kp::pango::CinderPango::setTextRenderer(kp::pango::TextRenderer::FREETYPE);
		mPango = kp::pango::CinderPango::create();
		mPango->setMaxSize(getWindowWidth(), getWindowHeight());
		mPango->setDefaultTextStyle("Sans", 24.0f, ci::Color::white());
		mPango->setText("<b>Bold</b> and <span foreground=\"red\">red</span> text");
	}

	void update() override {
		mPango->render(); // Only renders if something changed
	}

	void draw() override {
		ci::gl::clear();
		ci::gl::enableAlphaBlendingPremult();
		ci::gl::draw(mPango->getTexture());
	}

private:
	kp::pango::CinderPangoRef mPango;
};

CINDER_APP(MyApp, ci::app::RendererGl)
```

`render()` coalesces all changes since the last call and only rebuilds what needs to be rebuilt, so it's reasonable (and more efficient) to call it every frame in `update()` rather than after every change. It returns `true` when the texture was updated.

Alpha channels in textures returned from Cinder-Pango are premultiplied. Call `gl::enableAlphaBlendingPremult();` before drawing the texture. If you're seeing strange artifacts around text on different colored backgrounds, this is probably why.

Setting default font styles is more efficient than passing markup via the text string. Markup in the text overrides the defaults.

The [CinderPango.h](src/CinderPango.h) API is in the `kp::pango` namespace. Except for `setDefaultTextStyle()`, each setter has a matching getter:

| Method                                                                              | Description                                                                                                         |
| ----------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------- |
| `create()`                                                                          | Create an instance.                                                                                                 |
| `setText(text)` / `getText()`                                                       | The text to render, with optional Pango markup. `<br>` and `<br />` tags are converted to newlines.                 |
| `render(force)`                                                                     | Render into the texture if anything changed, or always when `force` is `true`.                                      |
| `getTexture()`                                                                      | The texture the text is rendered into.                                                                              |
| `setMinSize(size)` / `setMaxSize(size)`                                             | The texture's size range. Text wraps at the maximum width and is clipped past the maximum size (320 × 240 default). |
| `setDefaultTextStyle(font, size, color, weight, alignment)`                         | Set the default font, size, color, weight, and alignment at once.                                                   |
| `setDefaultTextFont(font)` / `setDefaultTextSize(size)`                             | Default font family and size.                                                                                       |
| `setDefaultTextColor(color)` / `setDefaultTextWeight(weight)`                       | Default color and `TextWeight` (`THIN` through `ULTRAHEAVY`).                                                       |
| `setDefaultTextItalicsEnabled(enabled)` / `setDefaultTextSmallCapsEnabled(enabled)` | Default italics and small caps.                                                                                     |
| `setTextAlignment(alignment)`                                                       | `TextAlignment::LEFT`, `CENTER`, `RIGHT`, or `JUSTIFY`.                                                             |
| `setSpacing(spacing)`                                                               | Extra space between lines, in pixels.                                                                               |
| `setTextAntialias(mode)`                                                            | `TextAntialias::DEFAULT`, `NONE`, `GRAY`, or `SUBPIXEL`.                                                            |
| `setBackgroundColor(color)`                                                         | Fill color behind the text. Transparent by default.                                                                 |
| `CinderPango::setTextRenderer(renderer)` / `getTextRenderer()`                      | Choose `TextRenderer::FREETYPE` or `PLATFORM_NATIVE` through the `PANGOCAIRO_BACKEND` environment variable.         |
| `CinderPango::loadFont(path)`                                                       | Load a font file so it can be used by name.                                                                         |
| `CinderPango::getFontList(verbose)` / `logFontList(verbose)`                        | List the available font families. With `verbose`, also log each family's faces.                                     |

Each instance reads the text renderer setting when it's created, so call `setTextRenderer()` before `create()`.

## Examples

### PangoBasic

The [PangoBasic sample](samples/PangoBasic/src/PangoBasicApp.cpp) renders a paragraph of mixed markup (bold, italic, colored spans, and font and size changes) that updates every frame. Open [PangoBasic.xcodeproj](samples/PangoBasic/xcode/PangoBasic.xcodeproj) in Xcode or [PangoBasic.sln](samples/PangoBasic/vc2015/PangoBasic.sln) in Visual Studio 2015. There's also a [CMake project](samples/PangoBasic/linux/CMakeLists.txt) for Linux that uses the system's Pango, Cairo, and HarfBuzz.

| Input         | Action                                                        |
| ------------- | ------------------------------------------------------------- |
| Click         | Set the maximum size to the mouse position                    |
| `A`           | Toggle antialiasing                                           |
| `Up` / `Down` | Increase / decrease line spacing                              |
| `I`           | Toggle italics                                                |
| `B`           | Toggle a random background color                              |
| `R`           | Switch between the FreeType and native renderers and recreate |
| `D` / `C`     | Destroy / create the `CinderPango` instance                   |

Clicking and the `A`, `Up` / `Down`, `I`, and `B` keys don't check for a destroyed instance, so press `C` (or `R`) after `D` before using them.

### PangoBasicNoCinder

A sanity-check project that uses Pango and Cairo directly, without Cinder. It [renders some markup](samples/PangoBasicNoCinder/src/main.cpp) into `test_font.png` and prints the text's size. There are Xcode and Visual Studio 2015 projects.

## Updating bundled libraries

On Mac, dependencies were built through [Homebrew](https://brew.sh). Two formulae had to be modified to build static libs instead of dylibs. The modified formulae are included in [`docs/homebrew`](docs/homebrew). Here's how to build them if necessary:

```sh
# From the docs/homebrew folder in this repo
brew rm cairo
brew rm pango
brew rm harfbuzz
brew install cairo --universal
brew install pango.rb --universal
brew install harfbuzz.rb --with-cairo --universal
```

These steps date from December 2015 (the formulae are for Pango 1.38.1 and HarfBuzz 1.1.2) and may need adjusting for current Homebrew. The macOS libraries were last rebuilt in February 2016.

On Windows, dependencies were compiled elsewhere.

Windows 64-bit libs procured via [gtk+-bundle_3.6.4-20130513_win64.zip](https://web.archive.org/web/20160208100239/http://www.tarnyko.net/dl/gtk.htm) and [gtk2 installer](https://tschoonj.github.io/blog/2014/09/29/gtk2-64-bit-windows-runtime-environment-installer-now-on-github/). (The latter had some issues... missing lib files.) See also another alternative [gtk-win64 build](https://web.archive.org/web/20160114031029/http://lvserver.ugent.be/gtk-win64/).

Windows 32-bit libs from here and there, currently not working. See the [notes in `lib/msw/Win32`](lib/msw/Win32/readme.md).

## Background

### Implementation notes

Each `CinderPango` instance keeps its own Pango font map, context, and layout. Setters only flag what changed, and `render()` then updates the font description, re-measures the layout, resizes the Cairo surface, and redraws the text as needed. Text that doesn't contain both `<` and `>` is passed to Pango as plain text, skipping the markup parser.

The Cairo surface is drawn flipped vertically and uploaded to the texture as BGRA.

On Windows, CinderPango draws into a win32 surface when `CAIRO_HAS_WIN32_SURFACE` is defined. The win32 surface support is compiled into Cinder's core Cairo block, but isn't present in the DLLs included in this library. Currently, if you import Cinder's Cairo block before importing CinderPango, then CinderPango will automatically use win32 surfaces instead of what (I assume) is the FreeType backend to blit the glyphs.

### Other projects

- Cinder's built-in [TextBox](https://libcinder.org/docs/classcinder_1_1_text_box.html) and TextLayout classes cover simpler text without the extra dependencies.
- [ofxPango](https://github.com/kitschpatrol/ofxPango) is the openFrameworks addon that inspired this block.

### Known issues

#### macOS

- Some fonts may not render correctly on macOS; they'll just render as a default serif or sans instead of what you requested. I haven't tracked down exactly how / why this is, but as a workaround you can tell PangoCairo to render glyphs with FreeType instead of the native CoreText library by setting the `PANGOCAIRO_BACKEND` environment variable. There's a method in the API to do this globally:

  ```cpp
  kp::pango::CinderPango::setTextRenderer(kp::pango::TextRenderer::FREETYPE); // this works around some font issues on mac
  ```

- The `TextRenderer::PLATFORM_NATIVE` (CoreText) backend renderer leaks memory through `_pango_core_text_shape`, `_pango_cairo_core_text_font_new` and other methods. The `TextRenderer::FREETYPE` backend renderer is comparatively free of leaks.

- The bundled macOS libraries are built for x86_64 (only `libintl.a` also has an i386 slice), so 32-bit and Apple silicon (arm64) builds aren't supported.

#### Windows

- The environment variable set by `setTextRenderer()` doesn't seem to do anything on Windows. Instead, I think rendering is determined by the type of Cairo surface you draw into (see [Implementation notes](#implementation-notes)). The renderer property isn't consolidated across platforms.
- Font loading from a local file doesn't work on Windows. `loadFont()` passes the path's native string to Fontconfig, which is a wide string on Windows. You have to install any fonts you'd like to use at the system level.
- There are some issues with text rendering quality on Windows.
- On Windows, antialiasing and some other properties have no effect. Platform-invalid parameters like these aren't documented or warned about.
- There's an issue with the GTK DLLs afflicting 32-bit Windows (Win32) builds. Win32 is currently not supported.
- With win32 surfaces, the destructor destroys the image surface borrowed from `cairo_win32_surface_get_image()` instead of the win32 surface it created.

#### General

- Only part of the Pango API is wrapped. Hyphenation, hinting, etc. aren't exposed, and hinting is always set to full.
- Certain attributes normally relegated to markup, like letter spacing and underline, aren't wrapped, so they can only be set through markup.
- Textures always have an alpha channel.
- Font sizes aren't unified across platforms like Cinder's font system. See `pango_cairo_context_set_resolution`.
- High DPI displays aren't handled.
- Glyphs are drawn into a new surface via `cairo_image_surface_create` and then copied into the texture, rather than blitted directly by passing a reference to the pixel data to `cairo_image_surface_create_for_data`.
- Cairo is bundled instead of using Cinder's Cairo block, although on Windows the Cairo block is still needed for its headers (see [Dependencies](#dependencies)).
- The Pango context is never released (releasing it seemed to crash on Windows), so each instance leaks some memory.

## Maintainers

[kitschpatrol](https://github.com/kitschpatrol)

## Acknowledgments

This block was inspired by the [openFrameworks](https://openframeworks.cc) addon [ofxPango](https://github.com/kitschpatrol/ofxPango), though the API has changed significantly to hide more of Pango and expose a more Cinder-esque interface.

Thanks to [Oriol Ferrer Mesià](https://uri.cat), [Diederick Huijbers](https://web.archive.org/web/20160119234901/http://www.roxlu.com/), [Matthias Rohrbach](https://www.matthiasrohrbach.ch), and others.

Thanks to [Petros Kataras](https://github.com/PetrosKataras) for the Linux project files for the PangoBasic sample.

<!-- contributing -->

## Contributing

[Issues](https://github.com/kitschpatrol/Cinder-Pango/issues) are welcome and appreciated.

Please open an issue to discuss changes before submitting a pull request. Unsolicited PRs (especially AI-generated ones) are unlikely to be merged.

This repository uses [MDAT](https://github.com/kitschpatrol/mdat) for readme placeholder expansion.

<!-- /contributing -->

<!-- license -->

## License

[MIT](license.txt) © Eric Mika

<!-- /license -->

The MIT license covers the wrapper code in [`src`](src) and the sample apps.

Two of the CMake find modules in [`samples/PangoBasic/linux/cmake`](samples/PangoBasic/linux/cmake) are from other authors and carry their own license headers: `FindCairo.cmake` (BSD) and `FindHarfBuzz.cmake` (MIT), and the PangoBasicNoCinder sample is adapted from a [roxlu.com example](https://web.archive.org/web/20161228051237/http://roxlu.com/2014/046/rendering-text-with-pango--cairo-and-freetype).

The headers in [`include`](include) and the libraries in [`lib`](lib) are third-party software under their own licenses.

Pango and GLib are under the LGPL (version 2 or later), and Cairo is under your choice of the LGPL 2.1 or the MPL 1.1. HarfBuzz, Fontconfig, and pixman use MIT-style licenses, FreeType uses the FreeType License, and libpng uses the libpng license. Both platforms include GNU gettext's libintl (LGPL).

The macOS libraries also include libgettextpo (GPL), and the Windows DLLs also include libffi, libiconv, liblzma, libwinpthread, libxml2, and zlib, each under its own license.
