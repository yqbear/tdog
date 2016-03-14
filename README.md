# tdog

ABOUT TDOG

TDOG is a lightweight, portable, and open source C++ xUnit Testing
Framework. It features:

 - Macros for easy test case creation
 - C++ namespaced test suites (test groups)
 - Test fixtures
 - Test, suite & global level setup/teardown
 - Repeated tests
 - Multi-report generation:
   - Text
   - HTML
   - JUnit/Jenkins compatible XML
 - Comprehensive range of asserts
 - Test specific and global time limits
 - Test time warnings
 - Exceptions (i.e. "no throw" and "throw" tests)
 - Not dependendent on RTTI
 - Lightweight & highly portable
 - No dependencies on other libraries or tools
 - Comprehensive documentation
 - Unencumbered open source license

TDOG is maintained for g++ for Linux, MSVC and MinGW on Windows.
It can be found at: http://bigangrydog.com

See the "index.html" under "doc" for documentation.


BUILDING

To build the static library on Linux, from the "tdog" directory, call:

make

Then build the self-test application, with:

make CONF=selftest.conf

Alternatively, to build everything in one step, use:

make rebuildall CONF=selftest.conf

Under MinGW, use "mingw32-make" instead.

This will build a "release" version of the software. To build with debug
information, append "TARGET=debug" to the above. You can also cross-compile
for both 32 and 64 bits by appending "ARCH=32" or "ARCH=64". See the
"makefile.readme" for more information about the make configuration.

Project files also exist for MSVC 2013, under the "mvc" directory. These
target MD and MT, 32 and 64 bit, and "release" and "debug".

TDOG uses the standard library and templates. Limited C++11 features will
be used automatically, if available, but is not necessary. See the
TDOG_NO_RTTI conditional option.

Once built, you can run the binary under the "bin" directory to perform
a self-test of the framework. Test reports will be generated to the
"bin/reports" directory. Run "selftest" directly from "bin", so that
this is the working directory.


RELEASE NOTES

Version 3.0 - First public release of TDOG.


PLATFORMS

I've built and tested TDOG under the following platforms:

CentOS 6 -    g++ 4.4.7 (32 bit)
CentOS 7 -    g++ 4.8.5 (64 bit)
Fedora 23 -   g++ 5.3.1 (64 bit)
Windows 10 -  TDM64 g++ 5.1.0 (32 and 64)
Windows 10 -  MSVC 2013 (32 and 64)
Windows XP -  Ancient Borland BCB 5* (32 bit)

Andy T

*Does not support contiguous suites under BCB.
