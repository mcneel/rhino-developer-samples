# Rhino.Inside Node.js
Sample showing how to start Rhino from node.js

## Requirements
- [Rhino 7 for Windows](https://www.rhino3d.com/download/rhino-for-windows/7/latest)
- [node.js and npm](https://nodejs.org/en/) (tested on node version 12.18.0 and npm 6.14.4)

## Dependencies
- [edge.js](https://www.npmjs.com/package/edge-js) - [version 14.3.2] Used to communicate between .net and node.js.
- [rhino3dm.js](https://www.npmjs.com/package/rhino3dm) - [version 0.13.0] Used to decode data from .net
- .net [version 4.8]
- [Rhino.Inside](https://www.nuget.org/packages/Rhino.Inside/) - [version 7.0.0] - Used to resolve Rhino related assemblies.
- [RhinoMethods Library](../SampleRhinoInsideJavascriptMethodsLib) - .net library that opens up Rhino.

## Usage
1. This assumes you've installed node.js and Rhino 7, and have built the RhinoMethods library.
2. Clone this repository. Open a terminal to the cloned repository directory.
3. Install dependencies: `npm i`.
4. Start app: `npm run start`.
5. You will see the following in the terminal:
```
rhino3dm loaded.
Rhino has started.
Mesh has 92 vertices and 100 faces.
```