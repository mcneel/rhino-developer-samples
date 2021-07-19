using System;
using System.Drawing;
using Grasshopper.Kernel;

namespace SampleGhTaskCapable
{
  public class SampleGhTaskCapableInfo : GH_AssemblyInfo
  {
    public override string Name => "SampleGhTaskCapable";
    public override Bitmap Icon => Properties.Resources.SampleGhTaskCapableComponent_24x24;
    public override string Description => "Sample Grasshopper task-capable component for Rhinoceros";
    public override Guid Id => new Guid("e0332029-ae49-484c-8ceb-be9b29636df1");
    public override string AuthorName => "Robert McNeel & Associates";
    public override string AuthorContact => "devsupport@mcneel.com";
  }
}
