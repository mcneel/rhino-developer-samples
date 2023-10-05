Rhino.FileIO.File3dm file3dm = Rhino.FileIO.File3dm.Read("../../models/Rhino_Logo_Mesh7.3dm");

var meshes = new List<Rhino.Geometry.Mesh>();

foreach ( Rhino.FileIO.File3dmObject ro in file3dm.Objects )
{
    // check if geometry type is a mesh. 
    // in theory, we can also extract render meshes from breps and extrusions if they have them, and get a mesh from a subd control net
    if( ro.Geometry.ObjectType == Rhino.DocObjects.ObjectType.Mesh ) 
    {
        meshes.Add(ro.Geometry as Rhino.Geometry.Mesh);
    }

}

Export.ObjExport.ExportMeshesToObj("Rhino_Logo_Mesh.obj", meshes);

