using Rhino;
using Rhino.PlugIns;
using Rhino.Runtime;
using Rhino.UI;
using System.Collections.Generic;

namespace SampleCsEto
{
  public class SampleCommandLineViewPlugIn : Rhino.PlugIns.PlugIn
  {
    public SampleCommandLineViewPlugIn()
    {
      Instance = this;
    }

    public static SampleCommandLineViewPlugIn Instance
    {
      get;
      private set;
    }
  }
}