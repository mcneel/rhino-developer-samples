# Rhino.Compute Samples

Write code to access the Rhino and Grasshopper SDKs through a stateless REST API running on Windows Servers. Use Compute to enhance any online solution you are developing to create and manipulate two and three-dimensional curves, surfaces, and solids. Install and customize Compute to run on any cloud services framework.

These samples show how to calculate various geometric operations with Rhino.Compute.

- [C#](cs)
  - [SampleMakeAMesh](cs/SampleMakeAMesh) - Creates a NURBS sphere and calls Rhino.Compute to mesh it.
  - [SampleCircleIntersection](cs/SampleCircleIntersection) - Solves the intersection between two circles and saves the resul as an SVG.
  - [SampleBrepBooleanOperation](cs/SampleBrepBooleanOperation) - Solves various boolean operations and saves them as OBJ files. 
  - [SampleNurbsTesselation](cs/SampleNurbsTesselation) - Creates a NurbsSurface, converts it to a Mesh (using different meshing parameters) and exports the results as OBJ files.
  [![](https://user-images.githubusercontent.com/121068/52407745-00b27800-2ac9-11e9-8040-630a69c789f4.png)]()
- [JavaScript](js)
  - [SampleBrepMesher](js/SampleBrepMesher) - Calls compute to mesh a BREP.
  - [SampleBrepIsocurves](js/SampleBrepIsocurves) - Loads a 3dm file containing the Rhino logo as a BREP, fetches the render mesh and wireframe and loads them into a three.js scene.
  [![BREP extract isocurves sample](https://user-images.githubusercontent.com/1014562/68939044-f2a02f00-079f-11ea-8e39-d582fa67e409.png)](js/SampleBrepIsocurves)
  - [SampleBrepBoolean](js/SampleBrepBoolean) - Loads a 3dm file containing the Rhino logo as a BREP and another set of breps then calculates the boolean difference between the Rhino logo and these.
  [![BREP Boolean difference sample](https://user-images.githubusercontent.com/1014562/68939173-3b57e800-07a0-11ea-9e3a-46a9e4a82f40.png)](js/SampleBrepBoolean)
  - [SampleClashDetection](js/SampleClashDetection) - Performs clash detection between a bunch of randomly positioned spheres and one main sphere.
  [![Clash detection sample](https://user-images.githubusercontent.com/1014562/68939429-e7013800-07a0-11ea-91cb-7b58a6e97c3b.png)](js/SampleClashDetection)
  - [SampleGHCurveInput](js/SampleGHCurveInput) - Builds a curve with rhino3dm.js and uses it as the input to a GH definition.
  [![Sample Grasshopper Curve Input](https://user-images.githubusercontent.com/1014562/101641267-d5d13c00-3a31-11eb-87f8-d92b4a8b293c.png)]()
  - [SampleGHDelaunayMesh](js/SampleGHDelaunayMesh) - Generates random points and meshes them with the Delaunay component in Grasshopper.
  [![Sample Grasshopper Delaunay Mesh](https://user-images.githubusercontent.com/1014562/68939360-bc16e400-07a0-11ea-80c1-f88aa7c5c0ec.png)](js/SampleGHDelaunayMesh)
  - [SampleGHExtrusions](js/SampleGHExtrusions) - Passes the input from three sliders to a Grasshopper defintion which offsets faces from a mesh and smooths it with SubD components.
  [![Sample Grasshopper Extrusions](https://user-images.githubusercontent.com/1014562/68939312-912c9000-07a0-11ea-9261-ed2e025bfa45.png)](SampleGHExtrusions)
- [Python](py)
  - [SampleTkinter](py/SampleTkinter) - This sample uses rhino3dm to create a brep from a sphere and then sends it to Compute to generate a mesh. The resulting mesh is visualised with Python's built-in Tkinter GUI framework.
  [![Python tkiner sample](https://user-images.githubusercontent.com/1014562/101641680-555f0b00-3a32-11eb-9dfd-c703adebe54c.gif)]()