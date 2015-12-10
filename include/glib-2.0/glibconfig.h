#ifdef _WIN32
// define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
// define something for Windows (64-bit only)
#include <glibconfig-win64.h>
#endif
#elif __APPLE__
// define something for mac only
#include <glibconfig-osx.h>
#elif __linux__
// linux
#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif