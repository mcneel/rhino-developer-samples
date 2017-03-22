using System;

namespace GrasshopperPlatonics
{
  /// <summary>
  /// The one and only Tetrahedron component
  /// </summary>
  public class Tetrahedron : PlatonicComponentBase
  {
    /// <summary>
    /// Constructor
    /// </summary>
    public Tetrahedron()
      : base("Tetrahedron", "U1", "Platonic solid with 4 faces", "Platonics")
    {
    }

    /// <summary>
    /// Returns a consistent ID for this object type. Every object must supply a
    /// unique and unchanging ID that is used to identify objects of the same type.
    /// </summary>
    public override Guid ComponentGuid
    {
      get { return new Guid("3ACD41AE-5288-4B80-9E47-8E4976AEDF2C"); }
    }

    /// <summary>
    /// Returns the platonic object
    /// </summary>
    public override PlatonicsCommon.PlatonicBase Geometry()
    {
      return new PlatonicsCommon.Tetrahedron();
    }

    /// <summary>
    /// Returns the component's icon
    /// </summary>
    protected override System.Drawing.Bitmap Icon 
    {
      get { return GrasshopperPlatonics.Properties.Resources.Tetrahedron; } 
    }
  }
}
