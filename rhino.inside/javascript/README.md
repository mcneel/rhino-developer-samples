# Rhino.Inside Javascript
Rhino Inside® technology allows Rhino and Grasshopper to be embedded within other products.
### Running Rhino and Grasshopper inside various JavaScript Runtimes / Frameworks (Node.js, Electron, Chromium Embedded Framework)

## Samples

1. [Rhino.Inside Node.js](SampleRhinoInsideNodeApp) - This sample runs Rhino inside Node.js. The JavaScript code calls into a .NET dll which launches Rhino, creates some geometry, and returns information related to that geometry. This information is then passed back to JavaScript and is printed to the console. This sample uses the RhinoMethods Library.
2. [Rhino.Inside Electron](SampleRhinoInsideElectronApp) - This sample runs Rhino.Inside Electron. The Electron app calls methods from JavaScript to .NET to launch Rhino, create some geometry. This geometry is then passed back to Electron. The geometry is deserialized with rhino3dm.js and rendered with Three.js. This sample uses the RhinoMethods Library.
3. [Rhino.Inside Electron + GH](SampleRhinoInsideElectronAppGH) - This sample starts Rhino and Grasshopper inside Electron. Grasshopper preview meshes are passed to the Electron app and visualized with Three.js. This sample uses the RhinoTaskManager Library.
4. [Rhino.Inside CEF](SampleRhinoInsideCEF) - This sample starts Rhino and Grasshopper inside a .net winforms application hosting a Chromium Embedded Framework browser. Grasshopper Preview meshes are passed to CEF and visualized with Three.js. This sample uses the RhinoTaskManager Library.

## Libraries

The JavaScript apps depend on .net library projects:

1. [RhinoMethods](SampleRhinoInsideJavascriptMethods) - This library exposes two simple methods, one to start Rhino and another which makes a simple sphere. This library is used in the [Rhino.Inside Node.js](SampleRhinoInsideNodeApp) and [Rhino.Inside Electron](SampleRhinoInsideElectronApp) samples.
2. [RhinoTaskManager](SampleRhinoInsideJavascriptTaskManager) - This library creates a task scheduler which manages tasks to be run by Rhino. This library is used in the [Rhino.Inside Electron + GH](SampleRhinoInsideElectronAppGH) and [Rhino.Inside CefSharp]() samples.

## Usage
1. Open the `SampleRhinoInsideJavascriptLibs.sln` and build the two libraries that are used by the rest of the samples.
2. Open any of the samples in VSCode and run `npm i` and `npm run start` except for the [Rhino.Inside CEF](SampleRhinoInsideCEF) which should be opened in Visual Studio, build and run or debugged.