using Rhino.FileIO;

var model = new Rhino.FileIO.File3dm();

var circle = new Rhino.Geometry.Circle(10);

var bbox = new Rhino.Geometry.BoundingBox(-10,-10,0, 10,10,20);
var box = new Rhino.Geometry.Box(bbox);
var extrusion = Rhino.Geometry.Extrusion.CreateBoxExtrusion(box);

var geometry = new List<Rhino.Geometry.GeometryBase>();
var attributes = new List<Rhino.DocObjects.ObjectAttributes>();

geometry.Add(circle.ToNurbsCurve());
geometry.Add(extrusion);

attributes.Add(new Rhino.DocObjects.ObjectAttributes());
attributes.Add(new Rhino.DocObjects.ObjectAttributes());

var pt = Rhino.Geometry.Point3d.Origin;

var index = model.AllInstanceDefinitions.Add("test", "an instance definition from dotnet", "https://www.rhino3d.com","tag", pt, geometry, attributes);

Console.WriteLine("Index of the new idef: {0}", index);

var idef = model.AllInstanceDefinitions.ToArray()[index];

var xform = new Rhino.Geometry.Transform(100);
var iref = new Rhino.Geometry.InstanceReferenceGeometry(idef.Id, xform);
var guid = model.Objects.AddInstanceObject(iref, null);

Console.WriteLine("Guid of the new iRef: {0}", guid);

model.Write("idef_cs.3dm", null);


