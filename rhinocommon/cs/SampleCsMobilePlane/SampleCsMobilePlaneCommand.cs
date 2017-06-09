using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsMobilePlane
{
  [System.Runtime.InteropServices.Guid("ba068579-dcab-476d-b616-a16446ba703e")]
  public class SampleCsMobilePlaneCommand : Command
  {
    /// <summary>
    /// The command name as it appears on the Rhino command line.
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsMobilePlane"; }
    }

    /// <summary>
    /// Called by Rhino when the user runs the command.
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select object");
      go.SubObjectSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var obj_ref = go.Object(0);
      var obj = obj_ref.Object();
      if (null == obj)
        return Result.Failure;

      var pre_selected = go.ObjectsWerePreselected;
      obj.Select(true);

      var gt = new GetOption();
      gt.SetCommandPrompt("Choose mobile plane option");
      gt.AcceptNothing(true);

      var attach_index = gt.AddOption("Attach");
      var detach_index = gt.AddOption("Detach");
      var enable_index = gt.AddOption("Enable");
      var refresh_index = gt.AddOption("Refresh");
      var show_index = gt.AddOption("Show");

      for (;;)
      {
        var res = gt.Get();
        if (res != GetResult.Option)
          break;

        var rc = Result.Cancel;
        var index = gt.OptionIndex();

        if (index == attach_index)
          rc = AttachOption(doc, obj);
        else if (index == detach_index)
          rc = DetachOption(doc, obj);
        else if (index == enable_index)
          rc = EnableOption(doc, obj);
        else if (index == refresh_index)
          rc = RefreshOption(doc, obj);
        else if (index == show_index)
          rc = ShowOption(doc, obj);

        if (rc != Result.Success)
          break;
      }

      if (!pre_selected)
        doc.Objects.UnselectAll();

      doc.Views.Redraw();

      return Result.Success;
    }

    /// <summary>
    /// Attaches a mobile plane to a Rhino object
    /// </summary>
    private Result AttachOption(RhinoDoc doc, RhinoObject obj)
    {
      if (null == doc || null == obj)
        return Result.Failure;

      var viewport_id = doc.Views.ActiveView.ActiveViewportID;
    
      Plane plane;
      var res = RhinoGet.GetPlane(out plane);
      if (res != Result.Success)
        return res;

      var rc = SampleCsMobilePlaneUserData.Attach(obj, plane, viewport_id);
      return rc ? Result.Success : Result.Failure;
    }

    /// <summary>
    /// Detaches a mobile plane from a Rhino object
    /// </summary>
    private Result DetachOption(RhinoDoc doc, RhinoObject obj)
    {
      if (null == doc || null == obj)
        return Result.Failure;

      if (!SampleCsMobilePlaneUserData.IsAttached(obj))
      {
        RhinoApp.WriteLine("No mobile plane attached.");
        return Result.Success;
      }

      var rc = SampleCsMobilePlaneUserData.Detach(obj);
      return rc ? Result.Success : Result.Failure;
    }

    /// <summary>.
    /// Enable mobile plane's dynamic construction plane update.
    /// </summary>
    private Result EnableOption(RhinoDoc doc, RhinoObject obj)
    {
      if (null == doc || null == obj)
        return Result.Failure;

      if (!SampleCsMobilePlaneUserData.IsAttached(obj))
      {
        RhinoApp.WriteLine("No mobile plane attached.");
        return Result.Success;
      }

      var enable = SampleCsMobilePlaneUserData.IsEnabled(obj);
      var res = RhinoGet.GetBool("Enable object mobile plane", true, "Disable", "Enable", ref enable);
      if (res != Result.Success)
        return res;

      var rc = SampleCsMobilePlaneUserData.Enable(obj, enable);
      return rc ? Result.Success : Result.Failure;
    }

    /// <summary>
    /// Refreshes, or updates, the construction plane tracked by the mobile plane.
    /// </summary>
    private Result RefreshOption(RhinoDoc doc, RhinoObject obj)
    {
      if (null == doc || null == obj)
        return Result.Failure;

      if (!SampleCsMobilePlaneUserData.IsAttached(obj))
      {
        RhinoApp.WriteLine("No mobile plane attached.");
        return Result.Success;
      }

      var rc = SampleCsMobilePlaneUserData.Refresh(obj, true);
      return rc ? Result.Success : Result.Failure;
    }

    /// <summary>
    ///  Show the mobile construction plane axes.
    /// </summary>
    private Result ShowOption(RhinoDoc doc, RhinoObject obj)
    {
      if (null == doc || null == obj)
        return Result.Failure;

      var data = SampleCsMobilePlaneUserData.DataFromObject(obj);
      if (null == data)
      {
        RhinoApp.WriteLine("No mobile plane attached.");
        return Result.Success;
      }

      var conduit = new SampleCsMobilePlaneConduit(data.Plane);
      doc.Views.Redraw();

      var gs = new GetString();
      gs.SetCommandPrompt("Press <Enter> when done.");
      gs.Get();

      conduit.Enabled = false;
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
