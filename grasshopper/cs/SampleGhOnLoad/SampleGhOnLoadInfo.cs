using System;
using System.Drawing;
using Grasshopper.Kernel;

namespace SampleGhOnLoad
{
  public class SampleGhOnLoadInfo : GH_AssemblyInfo
  {
    public override string Name => "SampleGhOnLoad";
    public override Bitmap Icon => Properties.Resources.SampleGhOnLoad_24x24;
    public override string Description => "Sample Grasshopper component for Rhinoceros 6";
    public override Guid Id => new Guid("ad82f38f-1978-4c3d-8882-b326af367ca1");
    public override string AuthorName => "Robert McNeel & Associates";
    public override string AuthorContact => "devsupport@mcneel.com";
  }
}
