using System;
using SampleGhPlatonics.Geometry;

namespace SampleGhPlatonics.Components
{
  /// <summary>
  /// The one and only Octahedron component
  /// </summary>
  public class Octahedron : PlatonicComponentBase
  {
    /// <summary>
    /// Constructor
    /// </summary>
    public Octahedron()
      : base("Octahedron", "U5", "Platonic solid with 8 faces", "Platonics")
    {
    }

    /// <summary>
    /// Returns a consistent ID for this object type. Every object must supply a
    /// unique and unchanging ID that is used to identify objects of the same type.
    /// </summary>
    public override Guid ComponentGuid => new Guid("EDD414B2-AB24-49BB-8A56-B4AAAB4B20FF");

    /// <summary>
    /// Returns the platonic object
    /// </summary>
    public override PlatonicGeometryBase Geometry() => new Geometry.Octahedron();

    /// <summary>
    /// Returns the component's icon
    /// </summary>
    protected override System.Drawing.Bitmap Icon => Properties.Resources.Octahedron_24x24;
  }
}
