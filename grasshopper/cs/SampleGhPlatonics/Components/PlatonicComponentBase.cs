using Rhino.Geometry;
using SampleGhPlatonics.Geometry;

namespace SampleGhPlatonics.Components
{
  /// <summary>
  /// The base class for all components
  /// </summary>
  public abstract class PlatonicComponentBase : Grasshopper.Kernel.GH_Component
  {
    /// <summary>
    /// Constructor
    /// </summary>
    protected PlatonicComponentBase(string name, string nickname, string description, string subCategory)
      : base(name, nickname, description, "Sample", subCategory)
    {
    }

    /// <summary>
    /// Returns the platonic object
    /// </summary>
    public abstract PlatonicGeometryBase Geometry();

    /// <summary>
    /// Declare all your input parameters here
    /// </summary>
    protected override void RegisterInputParams(GH_InputParamManager input)
    {
      input.AddPointParameter("Point", "P", "Center point", Grasshopper.Kernel.GH_ParamAccess.item, Rhino.Geometry.Point3d.Origin);
      input.AddNumberParameter("Length", "L", "Edge length", Grasshopper.Kernel.GH_ParamAccess.item, 1.0);
    }

    /// <summary>
    /// Declare all your output parameters here
    /// </summary>
    protected override void RegisterOutputParams(GH_OutputParamManager output)
    {
      output.AddBrepParameter("Faces", "F", "Brep faces", Grasshopper.Kernel.GH_ParamAccess.list);
      output.AddCurveParameter("Edge", "E", "Edge curves", Grasshopper.Kernel.GH_ParamAccess.list);
      output.AddPointParameter("Vertices", "V", "Face vertices", Grasshopper.Kernel.GH_ParamAccess.list);
    }

    /// <summary>
    /// This function will be called (successively) from within the
    /// ComputeData method of this component
    /// </summary>
    protected override void SolveInstance(Grasshopper.Kernel.IGH_DataAccess data)
    {
      var center = Rhino.Geometry.Point3d.Origin;
      var length = 1.0;

      if (!data.GetData(0, ref center)) return;
      if (!data.GetData(1, ref length)) return;
      if (!center.IsValid || length <= 0.0) return;

      var dir = center - Point3d.Origin;
      var plane = Plane.WorldXY;
      var xf_translate = Transform.Translation(dir);
      var xf_scale = Transform.Scale(plane, length, length, length);
      var xf = xf_translate * xf_scale;
      if (!xf.IsValid)
        return;

      var geom = Geometry();
      if (null == geom) return;

      var faces = geom.Faces();
      if (null != faces && 0 < faces.Length)
      {
        foreach (var face in faces)
          face.Transform(xf);
        data.SetDataList(0, faces);
      }

      var edges = geom.Edges();
      if (null != edges && 0 < edges.Length)
      {
        foreach (var edge in edges)
          edge.Transform(xf);
        data.SetDataList(1, edges);
      }

      var vertices = geom.Vertices();
      if (null != vertices && 0 < vertices.Length)
      {
        for (var i = 0; i < vertices.Length; i++)
          vertices[i].Transform(xf);
        data.SetDataList(2, vertices);
      }
    }
  }
}
