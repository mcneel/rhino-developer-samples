using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsDeserializeEmbeddedResource.Commands
{
  public class SampleCsDeserializeEmbeddedResource : Command
  {
    private const string RESOURCE = "SampleCsDeserializeEmbeddedResource.Resources.SampleCsBrepBox.bin";

    public override string EnglishName => "SampleCsDeserializeEmbeddedResource";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GeometryBase geometry = SampleCsGeometryHelper.ReadFromEmbeddedResource(RESOURCE);
      if (null == geometry)
        return Result.Failure;

      Brep brep = geometry as Brep;
      if (null != brep)
      {
        doc.Objects.AddBrep(brep);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
