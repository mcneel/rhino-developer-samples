namespace SampleCsDeseializeEmbeddedResource
{
  public class SampleCsDeseializeEmbeddedResourcePlugIn : Rhino.PlugIns.PlugIn
  {
    public SampleCsDeseializeEmbeddedResourcePlugIn()
    {
      Instance = this;
    }

    public static SampleCsDeseializeEmbeddedResourcePlugIn Instance
    {
      get; private set;
    }
  }
}