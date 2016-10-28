using Rhino;
using Rhino.Geometry;

namespace examples_cs
{
  [System.Runtime.InteropServices.Guid("0E82E6DA-5335-453A-AC94-2499BBBCBE55")]
  public class RTreeClosestPoint : Rhino.Commands.Command
  {
    public override string EnglishName { get { return "cs_RtreeClosestPoint"; } }

    void SearchCallback(object sender, RTreeEventArgs e)
    {
      SearchData data = e.Tag as SearchData;
      if (data == null)
        return;
      data.HitCount = data.HitCount + 1;
      Point3f vertex = data.Mesh.Vertices[e.Id];
      double distance = data.Point.DistanceTo(vertex);
      if (data.Index == -1 || data.Distance > distance)
      {
        // shrink the sphere to help improve the test
        e.SearchSphere = new Sphere(data.Point, distance);
        data.Index = e.Id;
        data.Distance = distance;
      }
    }

    class SearchData
    {
      public SearchData(Mesh mesh, Point3d point)
      {
        Point = point;
        Mesh = mesh;
        HitCount = 0;
        Index = -1;
        Distance = 0;
      }

      public int HitCount { get; set; }
      public Point3d Point { get; private set; }
      public Mesh Mesh { get; private set; }
      public int Index { get; set; }
      public double Distance { get; set; }
    }

    protected override Rhino.Commands.Result RunCommand(RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      Rhino.DocObjects.ObjRef objref;
      var rc = Rhino.Input.RhinoGet.GetOneObject("select mesh", false, Rhino.DocObjects.ObjectType.Mesh, out objref);
      if (rc != Rhino.Commands.Result.Success)
        return rc;

      Mesh mesh = objref.Mesh();
      objref.Object().Select(false);
      doc.Views.Redraw();

      using (RTree tree = new RTree())
      {
        for (int i = 0; i < mesh.Vertices.Count; i++)
        {
          // we can make a C++ function that just builds an rtree from the
          // vertices in one quick shot, but for now...
          tree.Insert(mesh.Vertices[i], i);
        }

        while (true)
        {
          Point3d point;
          rc = Rhino.Input.RhinoGet.GetPoint("test point", false, out point);
          if (rc != Rhino.Commands.Result.Success)
            break;

          SearchData data = new SearchData(mesh, point);
          // Use the first vertex in the mesh to define a start sphere
          double distance = point.DistanceTo(mesh.Vertices[0]);
          Sphere sphere = new Sphere(point, distance * 1.1);
          if (tree.Search(sphere, SearchCallback, data))
          {
            doc.Objects.AddPoint(mesh.Vertices[data.Index]);
            doc.Views.Redraw();
            RhinoApp.WriteLine("Found point in {0} tests", data.HitCount);
          }
        }
      }
      return Rhino.Commands.Result.Success;
    }
  }
}

