using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsDeseializeEmbeddedResource.Commands
{
  public class SampleCsDeseializeEmbeddedResourceCommand : Command
  {
    private const string RESOURCE = "SampleCsDeseializeEmbeddedResource.Resources.SampleCsBrepBox.bin";

    public override string EnglishName => "SampleCsDeseializeEmbeddedResource";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var geometry = SampleCsGeometryHelper.ReadFromEmbeddedResource(RESOURCE);
      if (null == geometry)
        return Result.Failure;

      var brep = geometry as Brep;
      if (null != brep)
      {
        doc.Objects.AddBrep(brep);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
