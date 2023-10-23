Rhino.FileIO.File3dm file3dm = new Rhino.FileIO.File3dm();

var material = new Rhino.DocObjects.Material();
material.ToPhysicallyBased();
material.PhysicallyBased.BaseColor = new Rhino.Display.Color4f(1, 0, 0, 0);
material.PhysicallyBased.Metallic = 0.7;
material.PhysicallyBased.Roughness = 0.5;
file3dm.AllMaterials.Add(material);

var oa = new Rhino.DocObjects.ObjectAttributes
{
    MaterialIndex = 0,
    MaterialSource = Rhino.DocObjects.ObjectMaterialSource.MaterialFromObject
};

var sphere = new Rhino.Geometry.Sphere(Rhino.Geometry.Point3d.Origin, 10);
file3dm.Objects.AddSphere(sphere, oa);

file3dm.Write("fileWithMaterial.3dm", null);





