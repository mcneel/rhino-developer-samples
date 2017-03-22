using System;

namespace GrasshopperPlatonics
{
  /// <summary>
  /// The one and only Dodecahedron component
  /// </summary>
  public class Dodecahedron : PlatonicComponentBase
  {
    /// <summary>
    /// Constructor
    /// </summary>
    public Dodecahedron()
      : base("Dodecahedron", "U23", "Platonic solid with 12 faces", "Platonics")
    {
    }

    /// <summary>
    /// Returns a consistent ID for this object type. Every object must supply a
    /// unique and unchanging ID that is used to identify objects of the same type.
    /// </summary>
    public override Guid ComponentGuid
    {
      get { return new Guid("81D5DFF0-5FF4-461C-ADD7-52E883F4DFEE"); }
    }

    /// <summary>
    /// Returns the platonic object
    /// </summary>
    public override PlatonicsCommon.PlatonicBase Geometry()
    {
      return new PlatonicsCommon.Dodecahedron();
    }

    /// <summary>
    /// Returns the component's icon
    /// </summary>
    protected override System.Drawing.Bitmap Icon
    {
      get { return GrasshopperPlatonics.Properties.Resources.Dodecahedron; }
    }
  }
}
