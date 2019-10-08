# SampleSharedXform

SampleSharedXform is a Rhino 6 solution that demonstrates how to share application data between [RhinoCommon](https://developer.rhino3d.com/guides/rhinocommon/) plug-ins and [Rhino C++](https://developer.rhino3d.com/guides/grasshopper/) plug-ins.

The solution contains the following projects:

* **SampleSharedXformLib** — Rhino-dependent C++ assembly (DLL). The assembly maintains a dictionary that maps Rhino object ids to an array of 4x4 transformation matrices. The data is runtime-only; it does not serialize in Rhino documents.
* **SampleSharedXformOwner** — A RhinoCommon plug-in that access the "C" exported functions from SampleSharedXformLib using a technique called [Platform Invoke](https://docs.microsoft.com/en-us/dotnet/standard/native-interop/pinvoke), or P/Invoke. This plug-in has read-write access to the dictionary maintained by SampleSharedXformLib.
* **SampleSharedXformClient1** — A Rhino C++ plug-in project that access the functionality, provided by SampleSharedXformLib, using [Load-Time Dynamic Linking](https://docs.microsoft.com/en-us/windows/win32/dlls/using-load-time-dynamic-linking). This plug-in has read-only access to the dictionary maintained by SampleSharedXformLib.
* **SampleSharedXformClient2** — A Rhino C++ plug-in project that access the functionality, provided bySampleSharedXformLib, using [Run-Time Dynamic Linking](https://docs.microsoft.com/en-us/windows/win32/dlls/using-run-time-dynamic-linking). This plug-in has read-only access to the dictionary maintained by SampleSharedXformLib.

## Building Sample

To build the sample, you are going to need:

* Rhino 6 (http://www.rhino3d.com)
* Rhino 6 C/C++ SDK (https://developer.rhino3d.com/)
* Microsoft Visual C++ 2017
* Microsoft Visual C# 2017

## License

This source code is licensed under the [MIT License](https://github.com/dalefugier/RhinoNature/blob/master/LICENSE).

