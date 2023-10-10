using Rhino.FileIO;
using Rhino.Geometry;

File3dm meshSphereDoc = File3dm.Read("MeshSphere_R8.3dm");
File3dm meshBoxDoc = File3dm.Read("MeshBox_R8.3dm");

// --- Brep --- //
Mesh meshSphere = new();
foreach( var ro in meshSphereDoc.Objects )
    meshSphere = (Mesh)ro.Geometry;

var sphere = new Rhino.Geometry.Sphere(Rhino.Geometry.Point3d.Origin, 10);
var brep = Rhino.Geometry.Brep.CreateFromSphere(sphere);
brep.Faces[0].SetMesh(Rhino.Geometry.MeshType.Render, meshSphere);

// --- Extrusion --- //
Mesh meshBox = new();
foreach( var ro in meshBoxDoc.Objects )
    meshBox = (Mesh)ro.Geometry;
    

var bbox = new Rhino.Geometry.BoundingBox(-10,-10, 0, 10, 10, 20);
var box = new Rhino.Geometry.Box(bbox);
var extrusion = Rhino.Geometry.Extrusion.CreateBoxExtrusion(box, true);
extrusion.SetMesh(meshBox, MeshType.Render);

File3dm outFile = new File3dm();

outFile.Objects.AddBrep(brep, null);
outFile.Objects.AddExtrusion(extrusion, null);
var fwo = new File3dmWriteOptions
{
    Version = 8
};
outFile.Write("SetMeshes.3dm", fwo);

