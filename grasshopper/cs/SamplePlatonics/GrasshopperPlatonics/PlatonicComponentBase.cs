using Rhino.Geometry;

namespace GrasshopperPlatonics
{
  /// <summary>
  /// The base class for all components
  /// </summary>
  public abstract class PlatonicComponentBase : Grasshopper.Kernel.GH_Component
  {
    int m_center_index = -1;
    int m_length_index = -1;
    int m_faces_index = -1;
    int m_edges_index = -1;
    int m_vertices_index = -1;

    /// <summary>
    /// Constructor
    /// </summary>
    protected PlatonicComponentBase(string name, string nickname, string description, string subCategory)
      : base(name, nickname, description, "Platonics", subCategory)
    {
    }

    /// <summary>
    /// Returns the platonic object
    /// </summary>
    public abstract PlatonicsCommon.PlatonicBase Geometry();

    /// <summary>
    /// Declare all your input parameters here
    /// </summary>
    protected override void RegisterInputParams(Grasshopper.Kernel.GH_Component.GH_InputParamManager input)
    {
      m_center_index = input.AddPointParameter("Point", "P", "Center point", Grasshopper.Kernel.GH_ParamAccess.item, Rhino.Geometry.Point3d.Origin);
      m_length_index = input.AddNumberParameter("Length", "L", "Edge length", Grasshopper.Kernel.GH_ParamAccess.item, 1.0);
    }

    /// <summary>
    /// Declare all your output parameters here
    /// </summary>
    protected override void RegisterOutputParams(Grasshopper.Kernel.GH_Component.GH_OutputParamManager output)
    {
      m_faces_index = output.AddBrepParameter("Faces", "F", "Brep faces", Grasshopper.Kernel.GH_ParamAccess.list);
      m_edges_index = output.AddCurveParameter("Edge", "E", "Edge curves", Grasshopper.Kernel.GH_ParamAccess.list);
      m_vertices_index = output.AddPointParameter("Vertices", "V", "Face vertices", Grasshopper.Kernel.GH_ParamAccess.list);
    }

    /// <summary>
    /// This function will be called (successively) from within the
    /// ComputeData method of this component
    /// </summary>
    protected override void SolveInstance(Grasshopper.Kernel.IGH_DataAccess data)
    {
      Rhino.Geometry.Point3d center = Rhino.Geometry.Point3d.Origin;
      double length = 1.0;

      if (!data.GetData(m_center_index, ref center)) return;
      if (!data.GetData(m_length_index, ref length)) return;
      if (!center.IsValid || length <= 0.0) return;

      Rhino.Geometry.Vector3d dir = center - Rhino.Geometry.Point3d.Origin;
      Rhino.Geometry.Plane plane = Rhino.Geometry.Plane.WorldXY;
      Rhino.Geometry.Transform xf_translate = Rhino.Geometry.Transform.Translation(dir);
      Rhino.Geometry.Transform xf_scale = Rhino.Geometry.Transform.Scale(plane, length, length, length);
      Rhino.Geometry.Transform xf = xf_translate * xf_scale;
      if (!xf.IsValid)
        return;

      PlatonicsCommon.PlatonicBase geom = Geometry();
      if (null == geom) return;

      Rhino.Geometry.Brep[] faces = geom.Faces();
      if (null != faces && 0 < faces.Length)
      {
        foreach (Brep t in faces)
          t.Transform(xf);
        data.SetDataList(m_faces_index, faces);
      }

      Rhino.Geometry.Curve[] edges = geom.Edges();
      if (null != edges && 0 < edges.Length)
      {
        foreach (Curve t in edges)
          t.Transform(xf);
        data.SetDataList(m_edges_index, edges);
      }

      Rhino.Geometry.Point3d[] vertices = geom.Vertices();
      if (null != vertices && 0 < vertices.Length)
      {
        for (int i = 0; i < vertices.Length; i++)
          vertices[i].Transform(xf);
        data.SetDataList(m_vertices_index, vertices);
      }
    }
  }
}
