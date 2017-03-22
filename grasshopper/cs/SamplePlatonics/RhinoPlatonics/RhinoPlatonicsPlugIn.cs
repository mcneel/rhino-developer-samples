namespace RhinoPlatonics
{
  public class RhinoPlatonicsPlugIn : Rhino.PlugIns.PlugIn
  {
    public RhinoPlatonicsPlugIn()
    {
      Instance = this;
    }

    public static RhinoPlatonicsPlugIn Instance
    {
      get;
      private set;
    }
  }
}