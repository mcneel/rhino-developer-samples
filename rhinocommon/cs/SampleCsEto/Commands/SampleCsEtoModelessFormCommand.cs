using System;
using Rhino;
using Rhino.Commands;
using Rhino.UI;

namespace SampleCsEto.Commands
{
  public class SampleCsEtoModelessFormCommand : Command
  {
    /// <summary>
    /// Form accessor
    /// </summary>
    private Views.SampleCsEtoModelessForm Form
    {
      get;
      set;
    }

    public override string EnglishName
    {
      get { return "SampleCsEtoModelessForm"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      if (null == Form)
      {
        Form = new Views.SampleCsEtoModelessForm { Owner = RhinoEtoApp.MainWindow };
        Form.RestorePosition();
        Form.Closed += OnFormClosed;
        Form.Show();
      }
      return Result.Success;
    }

    /// <summary>
    /// FormClosed EventHandler
    /// </summary>
    private void OnFormClosed(object sender, EventArgs e)
    {
      Form.SavePosition();
      Form.Dispose();
      Form = null;
    }
  }
}
