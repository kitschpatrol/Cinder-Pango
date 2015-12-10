#Cinder-Pango

**A thin wrapper for using the [Pango](http://pangu.io) text layout and rendering library with [Cinder](https://libcinder.org). Supports inline style markup and complex layouts that would be tedious to achieve with Cinder's built-in TextBox and TextLayout classes.**


## Getting Started
Take a look at the PangoBasic example.

This block has an obscene number of dependencies. Using Tinderbox or similar to integrate it with your project is recommended.


## Origin

This is a near-verbatim port of [ofxPango](https://github.com/kitschpatrol/ofxpango), and as such the API style is not what you might expect from a Cinderblock. (For now.)

Thanks to Oriol Ferrer Mesià, Diederick Huijbers, Matthias Rohrbach, and others.


## Compatibility

Tested against the [Cinder master branch](https://github.com/cinder/Cinder/commit/02089928b3982f866a77a9e6e2168075f9f9e6f6) (v9.1).

Tinderbox-generated project files are known to work on:

- Mac OS X 10.11 x64 with Xcode 7.2
- Windows 10 x64 with Visual Studio 2015 Community.

This library was not built with an eye towards backwards compatibility. It's probably relatively trivially achievable by rebuilding the dependencies, but this isn't currently a priority.

## Known Issues

### Xcode

If you get a bunch of `ld: warning: object file (../../../lib/macosx/libcairo.a(cairo.o)) was built for newer OSX version (10.11) than being linked (10.8)` warnings, then increase the minimum deploy target to 10.11.

### Visual Studio

Tinderbox enables the /gm flag in Visual Studio projects by default, which will conflicts with the /mp flag. Disable /gm to take care of this.

There's an issue with the GTK DLLs afflicting Win32 bit builds. Win32 is currently not supported.

## Dependences

**All dependencies are included in the block.**

On Mac, dependencies were build through [Homebrew](http://brew.sh). Two formulae had to be modified to build static libs instead of dylibs. The modified formulae are included in this repo. Here's how to build them if necesary:

	// from the /docs/homebrew folder in this repo
	brew rm cairo
	brew rm pango
	brew rm harfbuzz
	brew install cairo --universal
	brew install pango.rb --universal
	brew install harfbuzz.rb --with-cairo --universal



On Windows, dependencies were compiled elsewhere.

Windows 64 bit libs procured via [gtk+-bundle_3.6.4-20130513_win64.zip](http://www.tarnyko.net/dl/gtk.htm) and [gtk2 installer](http://tschoonj.github.io/blog/2014/09/29/gtk2-64-bit-windows-runtime-environment-installer-now-on-github). (The latter had some issues... missing lib files.) See also another alternative [gtk-win64 build](http://lvserver.ugent.be/gtk-win64/).

Windowns 32 bit libs from here and there, currently not working.

##TODO

- Use the Cinder Cairo block instead of including Cairo.
- 32 bit Windows support. (Some issue with the GTK DLLs.)
- Nicer, more Cinder-esque API.

