using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace examples_cs
{
  [System.Runtime.InteropServices.Guid("476cc15c-8979-4325-8819-298024efbb56")]
  public class CustomMeshCommand : Command
  {
    public override string EnglishName
    {
      get { return "CustomMeshCommand_CS"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var base_mesh = Mesh.CreateFromSphere(new Sphere(Point3d.Origin, 10.0), 20, 20);
      TestCustomMesh mesh = new TestCustomMesh(base_mesh);
      doc.Objects.AddRhinoObject(mesh);
      doc.Views.Redraw();
      return Result.Success;
    }
  }

  public class TestCustomMesh : Rhino.DocObjects.Custom.CustomMeshObject
  {
    public TestCustomMesh()
    {
    }

    public TestCustomMesh(Mesh mesh)
      : base(mesh)
    {
    }
    public override string ShortDescription(bool plural)
    {
      if (plural)
        return "TestCustomMeshes";
      return "TestCustomMesh";
    }
  }
}
