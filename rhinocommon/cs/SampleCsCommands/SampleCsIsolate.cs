using System.Runtime.InteropServices;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [Guid("1dbaa178-2797-47eb-b1ba-30507c320289"), CommandStyle(Style.ScriptRunner)]
  public class SampleCsIsolate : Command
  {
    public SampleCsIsolate()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsIsolate"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select objects to isolate");
      go.GroupSelect = true;
      go.SubObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      for (int i = 0; i < go.ObjectCount; i++)
      {
        var obj = go.Object(i).Object();
        if (null != obj)
          obj.Select(true);
      }

      doc.Views.RedrawEnabled = false;

      RhinoApp.RunScript("_-Invert", false);

      string script = string.Format("_-Hide {0}", SampleCsCommandsPlugIn.Instance.IsolateIndex);
      RhinoApp.RunScript(script, false);

      SampleCsCommandsPlugIn.Instance.IsolateIndex++;

      if (go.ObjectsWerePreselected)
      {
        for (int i = 0; i < go.ObjectCount; i++)
        {
          var obj = go.Object(i).Object();
          if (null != obj)
            obj.Select(true);
        }
      }

      doc.Views.RedrawEnabled = true;

      return Result.Success; 
    }
  }
}
