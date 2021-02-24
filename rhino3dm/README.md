# rhino3dm Samples

rhino3dm is a set of libraries based on the OpenNURBS geometry library with a "RhinoCommon" style. This provides the ability to access and manipulate geometry through .NET, Python or JavaScript applications independent of Rhino.

Functionality includes

- Create, interrogate, and store all geometry types supported in Rhino. This includes points, point clouds, NURBS curves and surfaces, polysurfaces (B-Reps), meshes, annotations, extrusions, and SubDs.
- Work with non-geometry classes supported in Rhino like layers, object attributes, transforms and viewports
- Read and write all of the above information to and from the .3dm file format
- Use as a client to make calls into the Rhino Compute cloud server for advanced manipulation of geometry objects
- Available on most platforms (Windows, macOS, Linux)

## Samples

- [C#](https://github.com/mcneel/rhino-developer-samples/tree/7/opennurbs/cs)
- [JavaScript](js)
  - [SampleSketch2d](js/SampleSketch2d) - [[Live](https://mcneel.github.io/rhino-developer-samples/rhino3dm/js/SampleSketch2d/)] Sketch NURBS curves on a canvas. Also shows how to download the resulting 3dm file.
  - [SampleObjectTypes](js/SampleObjectTypes) - [[Live](https://mcneel.github.io/rhino-developer-samples/rhino3dm/js/SampleObjectTypes/)] Sample shows the creation of various object types.
  - [SampleParse3dmObjects](js/SampleObjectTypes) - [[Live](https://mcneel.github.io/rhino-developer-samples/rhino3dm/js/SampleParse3dmObjects/)] Sample shows the creation of various object types and parsing with the Rhino3dmLoader for three.js.
  - [SampleUserStrings](js/SampleUserStrings) - [[Live](https://mcneel.github.io/rhino-developer-samples/rhino3dm/js/SampleUserStrings/)] Sample loads objects from a Rhino file and shows how to access the attribute and geometry user strings from the loaded 3dm directly and after it has been loaded by the Rhino3dmLoader for three.js.
  - [SampleViewer (Basic)](js/SampleViewer/01_basic) - [[Live](https://mcneel.github.io/rhino-developer-samples/rhino3dm/js/SampleViewer/01_basic/)] Loads a .3dm file and renders the geometry with [Three.js](https://threejs.org).
  [![image](https://user-images.githubusercontent.com/1014562/68938619-05feca80-079f-11ea-8d15-354c3e82a261.png)](js/SampleViewer/01_basic)
  - [Sample Viewer (Advanced)](js/SampleViewer/02_advanced) - [[Live](https://mcneel.github.io/rhino-developer-samples/rhino3dm/js/SampleViewer/02_advanced/)] Loads a .3dm file and renders the geometry and material with [Three.js](https://threejs.org).
  [![image](https://user-images.githubusercontent.com/1014562/68938708-30e91e80-079f-11ea-9ad8-0de304d87327.jpg)](js/SampleViewer/02_advanced)
  - [Sample3dmReader (node.js)](js/node/Sample3dmReader) - Shows how to read a file with node.js.
- [Python](py)
  - [SampleSimple](py/SampleSimple.py)
  - [SampleGetMeshes](py/SampleGetMeshes.py)
  - [SampleRequestBoundingBox](py/SampleRequestBoundingBox.py)
  - [SampleSphereLines](py/SampleSphereLines.py)
