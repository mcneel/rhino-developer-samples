namespace SampleCsDeserializeEmbeddedResource
{
  public class SampleCsDeserializeEmbeddedResourcePlugIn : Rhino.PlugIns.PlugIn
  {
    public SampleCsDeserializeEmbeddedResourcePlugIn()
    {
      Instance = this;
    }

    public static SampleCsDeserializeEmbeddedResourcePlugIn Instance
    {
      get; private set;
    }
  }
}