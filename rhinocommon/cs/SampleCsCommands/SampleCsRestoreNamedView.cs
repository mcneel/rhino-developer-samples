using System;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Input;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("df534212-46c4-45f9-a3d0-1c3e3065b58f")]
  public class SampleCsRestoreNamedView : Command
  {
    public SampleCsRestoreNamedView()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsRestoreNamedView"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      string name = null;
      Result rc = RhinoGet.GetString("Named view to restore", false, ref name);
      if (rc != Result.Success || string.IsNullOrEmpty(name))
        return Result.Cancel;

      int index = doc.NamedViews.FindByName(name);
      if (index < 0 || index >= doc.NamedViews.Count)
      {
        RhinoApp.WriteLine("Named view not found");
        return Result.Nothing;
      }

      //doc.NamedViews.Restore(index, view, false);
      doc.NamedViews.RestoreAnimated(index, view, false);
      view.Redraw();

      return Result.Success;
    }
  }
}
