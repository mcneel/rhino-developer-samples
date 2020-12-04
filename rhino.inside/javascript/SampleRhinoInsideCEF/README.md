# Rhino.Inside CEF
Sample showing how to start Rhino and Grasshopper from a winforms app with a CEF control.

![image](https://user-images.githubusercontent.com/1014562/101174472-36ccce80-3644-11eb-9c1d-42d4fe0fccfb.png)


## Requirements
- [Rhino 7 for Windows](https://www.rhino3d.com/download/rhino-for-windows/7/latest)

## Dependencies
- [CEFSharp](https://github.com/cefsharp/CefSharp) - [version 86.0.241] Used to host the UI, and javascript.
- [rhino3dm.js](https://www.npmjs.com/package/rhino3dm) - [version 0.13.0] Used to decode data from .net
- [three.js](https://threejs.org) - [version 123] Used to display the geometry coming from Rhino..
- .net [version 4.8]
- [Rhino.Inside](https://www.nuget.org/packages/Rhino.Inside/) - [version 7.0.0] - Used to resolve Rhino related assemblies.
- [RhinoTaskManager Library](../SampleRhinoInsideJavascriptTaskManagerLib) - .net library that opens up Rhino.

## Usage
1. This assumes you've installed node.js and Rhino 7.
2. Clone this repository and open the `SampleRhinoInsideCEF.sln` in Visual Studio 2019.
3. Start the application.
4. A winforms window will appear, then the GH Editor. Place some components onto the GH canvas to see them rendered on the js canvas hosted in CEF. This might take some time to start depending on the number of plugins installed in Rhino and GH.