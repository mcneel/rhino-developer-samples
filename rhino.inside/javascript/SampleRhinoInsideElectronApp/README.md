# Rhino.Inside Electron
Sample showing how to start Rhino from Electron

![image](https://user-images.githubusercontent.com/1014562/101172558-9b3a5e80-3641-11eb-84cc-1e9816053f19.png)

## Requirements
- [Rhino 7 for Windows](https://www.rhino3d.com/download/rhino-for-windows/7/latest)
- [node.js and npm](https://nodejs.org/en/) (tested on node version 12.18.0 and npm 6.14.4)

## Dependencies
- [electron-edge.js](https://www.npmjs.com/package/electron-edge-js) - [version 12.16.3] Used to communicate between .net and node.js.
- [rhino3dm.js](https://www.npmjs.com/package/rhino3dm) - [version 0.13.0] Used to decode data from .net
- [three.js](https://threejs.org) - [version 123] Used to display the geometry coming from Rhino in the Electron app.
- [Electron](https://www.electronjs.org/) - [version 5.0.13] Used to host the application, UI, and javascript.
- .net [version 4.8]
- [Rhino.Inside](https://www.nuget.org/packages/Rhino.Inside/) - [version 7.0.0] - Used to resolve Rhino related assemblies.
- [RhinoMethods Library](../SampleRhinoInsideJavascriptMethodsLib) - .net library that opens up Rhino.

## Usage
1. This assumes you've installed node.js and Rhino 7, and have built the RhinoMethods library.
2. Clone this repository. Open a terminal to the cloned repository directory.
3. Install dependencies: `npm i`.
4. Start app: `npm run start`.
5. An electron app will appear and eventually a wireframe render of a sphere.