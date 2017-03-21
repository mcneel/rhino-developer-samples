using Rhino;
using Rhino.Commands;
using Rhino.DocObjects.Custom;

namespace SampleCsRectangleGrips
{
  [System.Runtime.InteropServices.Guid("9c9af062-7d49-4081-9786-41f8247ac812")]
  public class SampleCsRectangleGripsCommand : Command
  {
    private SampleCsRectangleGripsEnabler m_grip_enabler;

    /// <summary>
    /// The command name as it appears on the Rhino command line
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsRectangleGrips"; }
    }

    /// <summary>
    /// Call by Rhino when the user wants to run this command
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      if (null == m_grip_enabler)
      {
        // Register once and only once...
        m_grip_enabler = new SampleCsRectangleGripsEnabler();
        CustomObjectGrips.RegisterGripsEnabler(m_grip_enabler.TurnOnGrips, typeof(SampleCsRectangleGrips));
      }

      var go = new SampleCsGetRectangleCurve();
      go.SetCommandPrompt("Select rectangles for point display");
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      for (var i = 0; i < go.ObjectCount; i++)
      {
        var rh_object = go.Object(i).Object();
        if (null != rh_object)
        {
          if (rh_object.GripsOn)
            rh_object.GripsOn = false;

          m_grip_enabler.TurnOnGrips(rh_object);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
