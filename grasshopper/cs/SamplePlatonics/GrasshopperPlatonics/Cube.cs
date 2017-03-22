using System;

namespace GrasshopperPlatonics
{
  /// <summary>
  /// The one and only Cube component
  /// </summary>
  public class Cube : PlatonicComponentBase
  {
    /// <summary>
    /// Constructor
    /// </summary>
    public Cube()
      : base("Cube", "U6", "Platonic solid with 6 faces", "Platonics")
    {
    }

    /// <summary>
    /// Returns a consistent ID for this object type. Every object must supply a
    /// unique and unchanging ID that is used to identify objects of the same type.
    /// </summary>
    public override Guid ComponentGuid
    {
      get { return new Guid("682E2CBC-7006-4682-9251-04628DC50200"); }
    }

    /// <summary>
    /// Returns the platonic object
    /// </summary>
    public override PlatonicsCommon.PlatonicBase Geometry()
    {
      return new PlatonicsCommon.Cube();
    }
    
    /// <summary>
    /// Returns the component's icon
    /// </summary>
    protected override System.Drawing.Bitmap Icon 
    {
      get { return GrasshopperPlatonics.Properties.Resources.Cube; } 
    }
  }
}
