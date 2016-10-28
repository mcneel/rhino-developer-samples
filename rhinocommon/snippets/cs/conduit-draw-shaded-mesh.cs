public class DrawShadedMeshConduit : Rhino.Display.DisplayConduit
{
  public Rhino.Geometry.Mesh Mesh { get; set; }

  protected override void CalculateBoundingBox(Rhino.Display.CalculateBoundingBoxEventArgs e)
  {
    if (null != Mesh)
    {
      Rhino.Geometry.BoundingBox bbox = Mesh.GetBoundingBox(false);
      // Unites a bounding box with the current display bounding box in
      // order to ensure dynamic objects in "box" are drawn.
      e.IncludeBoundingBox(bbox);
    }
  }

  protected override void PostDrawObjects(Rhino.Display.DrawEventArgs e)
  {
    if (null != Mesh)
    {
      Rhino.Display.DisplayMaterial material = new Rhino.Display.DisplayMaterial();
      material.Diffuse = System.Drawing.Color.Blue;
      e.Display.DrawMeshShaded(Mesh, material);
    }
  }
}

partial class Examples
{
  public static Rhino.Commands.Result ConduitDrawShadedMesh(Rhino.RhinoDoc doc)
  {
    Rhino.Geometry.Mesh mesh = MeshBox(100, 500, 10);
    if (null == mesh)
      return Rhino.Commands.Result.Failure;

    DrawShadedMeshConduit conduit = new DrawShadedMeshConduit();
    conduit.Mesh = mesh;
    conduit.Enabled = true;
    doc.Views.Redraw();

    string outputString = string.Empty;
    Rhino.Input.RhinoGet.GetString("Press <Enter> to continue", true, ref outputString);

    conduit.Enabled = false;
    doc.Views.Redraw();

    return Rhino.Commands.Result.Success;
  }

  public static Rhino.Geometry.Mesh MeshBox(double x, double y, double z)
  {
    Rhino.Geometry.Mesh mesh = new Rhino.Geometry.Mesh();
    mesh.Vertices.Add(0, 0, 0);
    mesh.Vertices.Add(x, 0, 0);
    mesh.Vertices.Add(x, y, 0);
    mesh.Vertices.Add(0, y, 0);
    mesh.Vertices.Add(0, 0, z);
    mesh.Vertices.Add(x, 0, z);
    mesh.Vertices.Add(x, y, z);
    mesh.Vertices.Add(0, y, z);
    mesh.Faces.AddFace(3, 2, 1, 0);
    mesh.Faces.AddFace(4, 5, 6, 7);
    mesh.Faces.AddFace(0, 1, 5, 4);
    mesh.Faces.AddFace(1, 2, 6, 5);
    mesh.Faces.AddFace(2, 3, 7, 6);
    mesh.Faces.AddFace(3, 0, 4, 7);
    mesh.Normals.ComputeNormals();
    mesh.Compact();
    if (mesh.IsValid)
      return mesh;
    return null;
  }
}
