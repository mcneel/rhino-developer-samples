using System;
using SampleGhPlatonics.Geometry;

namespace SampleGhPlatonics.Components
{
  /// <summary>
  /// The one and only Icosahedron component
  /// </summary>
  public class Icosahedron : PlatonicComponentBase
  {
    /// <summary>
    /// Constructor
    /// </summary>
    public Icosahedron()
      : base("Icosahedron", "U22", "Platonic solid with 20 faces")
    {
    }

    /// <summary>
    /// Returns a consistent ID for this object type. Every object must supply a
    /// unique and unchanging ID that is used to identify objects of the same type.
    /// </summary>
    public override Guid ComponentGuid => new Guid("864A9E15-F8AD-4F99-91EC-EF66BEB0AF46");

    /// <summary>
    /// Returns the platonic object
    /// </summary>
    public override PlatonicGeometryBase Geometry() => new Geometry.Icosahedron();

    /// <summary>
    /// Returns the component's icon
    /// </summary>
    protected override System.Drawing.Bitmap Icon => Properties.Resources.Icosahedron_24x24;
  }
}
