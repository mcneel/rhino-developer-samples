using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("0af9dfc5-e0ca-42bb-8fe6-42015bc0b13d")]
  public class SampleCsPrintViewList : Command
  {
    public SampleCsPrintViewList()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsPrintViewList"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.Tables.ViewTable viewTable = doc.Views;
      Rhino.Display.RhinoView[] views = viewTable.GetStandardRhinoViews();
      if (null != views)
      {
        int count = views.Length;
        for (int i = 0; i < count; i++)
        {
          Rhino.Display.RhinoView view = views[i];
          if (null != view)
          {
            string name = view.MainViewport.Name;
            Rhino.RhinoApp.WriteLine(string.Format("View {0} = {1}", i, name));
          }
        }
      }
      return Result.Success;
    }
  }
}
