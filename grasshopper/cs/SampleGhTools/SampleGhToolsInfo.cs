using Grasshopper.Kernel;
using System;
using System.Drawing;

namespace SampleGhTools
{
  public class SampleGhToolsInfo : GH_AssemblyInfo
  {
    public override string Name => "SampleGhTools";
    public override Bitmap Icon => Properties.Resources.SampleGhTools_24x24;
    public override string Description => "Sample Grasshopper tools for Rhinoceros";
    public override Guid Id => new Guid("e0332029-ae49-484c-8ceb-be9b29636df1");
    public override string AuthorName => "Robert McNeel & Associates";
    public override string AuthorContact => "devsupport@mcneel.com";
  }
}
