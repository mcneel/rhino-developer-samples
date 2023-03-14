using Grasshopper;
using Grasshopper.Kernel;
using System;
using System.Drawing;

namespace GrasshopperWatcherExample
{
  public class GrasshopperWatcherExampleInfo : GH_AssemblyInfo
  {
    public override string Name => "GrasshopperWatcherExample";

    //Return a 24x24 pixel bitmap to represent this GHA library.
    public override Bitmap Icon => null;

    //Return a short string describing the purpose of this GHA library.
    public override string Description => "";

    public override Guid Id => new Guid("0c77fef1-bd0e-4332-baf9-f9b5b4122c4c");

    //Return a string identifying you or your company.
    public override string AuthorName => "";

    //Return a string representing your preferred contact details.
    public override string AuthorContact => "";
  }
}