using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using SampleCsDeseializeEmbeddedResource;

namespace SampleCsDeserializeEmbeddedResource.Commands
{
  public class SampleCsDeserializeEmbeddedResource : Command
  {
    private const string RESOURCE = "SampleCsDeserializeEmbeddedResource.Resources.SampleCsBrepBox.bin";

    public override string EnglishName => "SampleCsDeserializeEmbeddedResource";

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
