namespace RhinoPlatonics
{
  /// <summary>
  /// The one and only Icosahedron command
  /// </summary>
  [System.Runtime.InteropServices.Guid("a089fcf1-8e4b-4be1-8f47-2a8ba7ca700e")]
  public class Icosahedron : PlatonicCommandBase
  {
    protected override PlatonicsCommon.PlatonicBase NewPlatonic()
    {
      return new PlatonicsCommon.Icosahedron();
    }
  }
}
