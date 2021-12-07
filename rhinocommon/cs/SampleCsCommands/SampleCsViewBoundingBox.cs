using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input;

namespace SampleCsCommands
{
  public class SampleCsViewBoundingBox : Command
  {
    private SampleCsViewBoundingBoxConduit m_conduit;

    public override string EnglishName => "SampleCsViewBoundingBox";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      if (null != m_conduit)
      {
        m_conduit.Enabled = false;
        doc.Views.Redraw();
        m_conduit = null;
        return Result.Success;
      }

      var rc = RhinoGet.GetBox(out Box box);
      if (rc != Result.Success)
        return rc;

      m_conduit = new SampleCsViewBoundingBoxConduit
      {
        BoundingBox = box.BoundingBox,
        Enabled = true
      };

      doc.Views.Redraw();

      return Result.Success;
    }
  }

  public class SampleCsViewBoundingBoxConduit : DisplayConduit
  {
    public BoundingBox BoundingBox { get; set; } = BoundingBox.Unset;

    protected override void PreDrawObject(DrawObjectEventArgs args)
    {
      if (null != args && null != args.RhinoObject && BoundingBox.IsValid)
      {
        var bbox = args.RhinoObject.Geometry.GetBoundingBox(true);
        if (!BoundingBox.Contains(bbox))
          args.DrawObject = false;
      }
      base.PreDrawObject(args);
    }
  }
}