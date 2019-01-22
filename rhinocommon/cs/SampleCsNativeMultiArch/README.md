# SampleCsNativeMultiArch

RhinoCommon native library sample. Demonstrates how to load an x86 native
library when running in Rhino 5 32-bit and vice-versa for Rhino 5 64-bit.

The native library in this case is _libcurl_ (MIT) and the included pre-built
binaries were extracted from nuget packages built from [pjquirk/libcurl.NET][1]
(also MIT).

This example focuses on the loading of this native library rather than how to
use cURL in Rhino! To test that the library has loaded correctly, the version
number is printed to the Rhino command line.

[1]: https://github.com/pjquirk/libcurl.NET
