namespace RhinoPlatonics
{
  /// <summary>
  /// The one and only Dodecahedron command
  /// </summary>
  [System.Runtime.InteropServices.Guid("4075050e-9621-489f-84a7-5ee0069a2e8d")]
  public class Dodecahedron : PlatonicCommandBase
  {
    protected override PlatonicsCommon.PlatonicBase NewPlatonic()
    {
      return new PlatonicsCommon.Dodecahedron();
    }
  }
}
