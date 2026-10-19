libharfbuzz-0.dll is from https://github.com/racket/libs
libwinpthread-1.dll is from a sketchy dll download site : /

pixman-1.lib is an import library generated from libpixman-1-0.dll with llvm-dlltool (`llvm-dlltool -m i386 -D libpixman-1-0.dll -d pixman-1.def -l pixman-1.lib`, with the .def listing the DLL's exports).
