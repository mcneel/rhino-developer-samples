namespace RhinoPlatonics
{
  /// <summary>
  /// The one and only Tetrahedron command
  /// </summary>
  [System.Runtime.InteropServices.Guid("5c73d524-cc18-4207-8751-0adb1643d59c")]
  public class Tetrahedron : PlatonicCommandBase
  {
    protected override PlatonicsCommon.PlatonicBase NewPlatonic()
    {
      return new PlatonicsCommon.Tetrahedron();
    }
  }
}
