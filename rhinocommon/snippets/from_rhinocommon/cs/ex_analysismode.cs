using System;
using Rhino;
using Rhino.DocObjects;
using Rhino.Geometry;


[System.Runtime.InteropServices.Guid("62dd8eec-5cce-42c7-9d80-8b01fc169b81")]
public class AnalysisModeOnCommand : Rhino.Commands.Command
{
  public override string EnglishName { get { return "cs_analysismode_on"; } }

  protected override Rhino.Commands.Result RunCommand(RhinoDoc doc, Rhino.Commands.RunMode mode)
  {
    // make sure our custom visual analysis mode is registered
    var zmode = Rhino.Display.VisualAnalysisMode.Register(typeof(ZAnalysisMode));

    const ObjectType filter = Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.PolysrfFilter | Rhino.DocObjects.ObjectType.Mesh;
    Rhino.DocObjects.ObjRef[] objs;
    var rc = Rhino.Input.RhinoGet.GetMultipleObjects("Select objects for Z analysis", false, filter, out objs);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    int count = 0;
    for (int i = 0; i < objs.Length; i++)
    {
      var obj = objs[i].Object();

      // see if this object is alreay in Z analysis mode
      if (obj.InVisualAnalysisMode(zmode))
        continue;

      if (obj.EnableVisualAnalysisMode(zmode, true))
        count++;
    }
    doc.Views.Redraw();
    RhinoApp.WriteLine("{0} objects were put into Z-Analysis mode.", count);
    return Rhino.Commands.Result.Success;
  }
}

[System.Runtime.InteropServices.Guid("0A8CE87D-A8CB-4A41-9DE2-5B3957436AEE")]
public class AnalysisModeOffCommand : Rhino.Commands.Command
{
  public override string EnglishName { get { return "cs_analysismode_off"; } }

  protected override Rhino.Commands.Result RunCommand(RhinoDoc doc, Rhino.Commands.RunMode mode)
  {
    var zmode = Rhino.Display.VisualAnalysisMode.Find(typeof(ZAnalysisMode));
    // If zmode is null, we've never registered the mode so we know it hasn't been used
    if (zmode != null)
    {
      foreach (Rhino.DocObjects.RhinoObject obj in doc.Objects)
      {
        obj.EnableVisualAnalysisMode(zmode, false);
      }
      doc.Views.Redraw();
    }
    RhinoApp.WriteLine("Z-Analysis is off.");
    return Rhino.Commands.Result.Success;
  }
}


/// <summary>
/// This simple example provides a false color based on the world z-coordinate.
/// For details, see the implementation of the FalseColor() function.
/// </summary>
public class ZAnalysisMode : Rhino.Display.VisualAnalysisMode
{
  Interval m_z_range = new Interval(-10,10);
  Interval m_hue_range = new Interval(0,4*Math.PI / 3);
  private const bool m_show_isocurves = true;

  public override string Name { get { return "Z-Analysis"; } }
  public override Rhino.Display.VisualAnalysisMode.AnalysisStyle Style { get { return AnalysisStyle.FalseColor; } }

  public override bool ObjectSupportsAnalysisMode(Rhino.DocObjects.RhinoObject obj)
  {
    if (obj is Rhino.DocObjects.MeshObject || obj is Rhino.DocObjects.BrepObject)
      return true;
    return false;
  }

  protected override void UpdateVertexColors(Rhino.DocObjects.RhinoObject obj, Mesh[] meshes)
  {
    // A "mapping tag" is used to determine if the colors need to be set
    Rhino.Render.MappingTag mt = GetMappingTag(obj.RuntimeSerialNumber);

    for (int mi = 0; mi < meshes.Length; mi++)
    {
      var mesh = meshes[mi];
      if( mesh.VertexColors.Tag.Id != this.Id )
      {
        // The mesh's mapping tag is different from ours. Either the mesh has
        // no false colors, has false colors set by another analysis mode, has
        // false colors set using different m_z_range[]/m_hue_range[] values, or
        // the mesh has been moved.  In any case, we need to set the false
        // colors to the ones we want.
        System.Drawing.Color[] colors = new System.Drawing.Color[mesh.Vertices.Count];
        for (int i = 0; i < mesh.Vertices.Count; i++)
        {
          double z = mesh.Vertices[i].Z;
          colors[i] = FalseColor(z);
        }
        mesh.VertexColors.SetColors(colors);
        // set the mesh's color tag 
        mesh.VertexColors.Tag = mt;
      }
    }
  }

  public override bool ShowIsoCurves
  {
    get
    {
      // Most shaded analysis modes that work on breps have the option of
      // showing or hiding isocurves.  Run the built-in Rhino ZebraAnalysis
      // to see how Rhino handles the user interface.  If controlling
      // iso-curve visability is a feature you want to support, then provide
      // user interface to set this member variable.
      return m_show_isocurves; 
    }
  }

  /// <summary>
  /// Returns a mapping tag that is used to detect when a mesh's colors need to
  /// be set.
  /// </summary>
  /// <returns></returns>
  Rhino.Render.MappingTag GetMappingTag(uint serialNumber)
  {
    Rhino.Render.MappingTag mt = new Rhino.Render.MappingTag();
    mt.Id = this.Id;

    // Since the false colors that are shown will change if the mesh is
    // transformed, we have to initialize the transformation.
    mt.MeshTransform = Transform.Identity;

    // This is a 32 bit CRC or the information used to set the false colors.
    // For this example, the m_z_range and m_hue_range intervals control the
    // colors, so we calculate their crc.
    uint crc = RhinoMath.CRC32(serialNumber, m_z_range.T0);
    crc = RhinoMath.CRC32(crc, m_z_range.T1);
    crc = RhinoMath.CRC32(crc, m_hue_range.T0);
    crc = RhinoMath.CRC32(crc, m_hue_range.T1);
    mt.MappingCRC = crc;
    return mt;
  }

  System.Drawing.Color FalseColor(double z)
  {
    // Simple example of one way to change a number into a color.
    double s = m_z_range.NormalizedParameterAt(z);
    s = Rhino.RhinoMath.Clamp(s, 0, 1);
    return System.Drawing.Color.FromArgb((int)(s * 255), 0, 0);
  }

}