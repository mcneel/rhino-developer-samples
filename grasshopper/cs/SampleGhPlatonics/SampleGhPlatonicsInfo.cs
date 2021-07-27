using System;
using System.Drawing;
using Grasshopper.Kernel;

namespace SampleGhPlatonics
{
  public class SampleGhPlatonicsInfo : GH_AssemblyInfo
  {
    public override string Name => "SampleGhPlatonics";
    public override Bitmap Icon => Properties.Resources.SampleGhPlatonics_24x24;
    public override string Description => "Sample Grasshopper component for Rhinoceros";
    public override Guid Id => new Guid("421c3995-2772-45b1-9a48-73f24e79aa6f");
    public override string AuthorName => "Robert McNeel & Associates";
    public override string AuthorContact => "devsupport@mcneel.com";
  }
}
