namespace RhinoPlatonics
{
  /// <summary>
  /// The one and only Cube command
  /// </summary>
  [System.Runtime.InteropServices.Guid("e1026698-4a62-4635-b6cf-9612da256293")]
  public class Cube : PlatonicCommandBase
  {
    protected override PlatonicsCommon.PlatonicBase NewPlatonic()
    {
      return new PlatonicsCommon.Cube();
    }
  }
}
