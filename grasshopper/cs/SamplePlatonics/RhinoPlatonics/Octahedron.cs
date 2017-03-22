namespace RhinoPlatonics
{
  /// <summary>
  /// The one and only Octahedron command
  /// </summary>
  [System.Runtime.InteropServices.Guid("ff202903-9052-41a7-9b10-8e7ed6ef4991")]
  public class Octahedron : PlatonicCommandBase
  {
    protected override PlatonicsCommon.PlatonicBase NewPlatonic()
    {
      return new PlatonicsCommon.Octahedron();
    }
  }
}
