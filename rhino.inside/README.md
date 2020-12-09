# Rhino.Inside® Samples

The Rhino.Inside® technology allows Rhino and Grasshopper to be embedded within other products.  It may be possible to:

* Starting Rhino and Grasshopper as an add in another product.
* Call directly into the host's native APIs from a Grasshopper or Rhino plugin.
* Access Rhino's APIs through the host application.
* Grasshopper definitions can be opened and previewed in Rhino within the same process as the parent.
* Object can be natively created by Rhino or Grasshopper within the parent product.

![sample-shot](https://user-images.githubusercontent.com/1014562/101173482-e43ee280-3642-11eb-8e49-d3cfd222a516.png)

This repository contains all the sample developer code for loading Rhino inside other 64-bit Windows applications.

These samples are compatible with Rhino 7 which can found [here](https://www.rhino3d.com/download/Rhino/7.0).

## Samples by host application.
The samples in this repository are organized by folders that correspond to the Company\Product name of the host application where you can found more information.

This repository contains samples for:
- [dotnet](dotnet)
   - [C# console application](dotnet/SampleHelloWorld)
   - [VB console application](dotnet/SampleHelloWorldVB)
   - [C# console application for converting files to .obj](dotnet/SampleConvert)
   - [Winforms App with Rhino display](dotnet/SampleWinFormsApp)
   - [Eto App with Rhino display](dotnet/SampleEtoApp)
   - [Console application which solves a GH definition](dotnet/SampleRunGrasshopper)
- [JavaScript](javascript)
   - [Node.js](javascript/SampleRhinoInsideNodeApp) A basic example showing how to run Rhino inside of Node.js via edge.js.
   - [Electron](javascript/SampleRhinoInsideElectronApp) Sample of running Rhino inside Electron.
   - [Electron + GH](javascript/SampleRhinoInsideElectronAppGH) Sample of running Grasshopper inside Electron.
   - [CEF](javascript/SampleRhinoInsideCEF) Sample of running Grasshopper inside Chromium Embedded Framework.

Related projects:
- [Rhino.Inside.Revit](https://github.com/mcneel/rhino.inside-revit)
- [Rhino.Inside.CPython]()
- [Rhino.Inside BricsCAD Bricsys](https://github.com/Bricsys/rhino.inside-bricscad)
- [Rhino.Inside.Unity](https://github.com/mcneel/rhino.inside/tree/master/Unity)
- [Rhino.Inside.Unreal](https://github.com/mcneel/rhino.inside/tree/master/Epic%20Games/UE)
- [Rhino.Inside.AutoCAD](https://github.com/mcneel/rhino.inside/tree/master/Autodesk/AutoCAD)

**If you have any questions, [visit the discussion forum...](https://discourse.mcneel.com/c/rhino-inside/110)**
