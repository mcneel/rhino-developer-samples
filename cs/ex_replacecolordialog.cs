using Rhino;
using Rhino.Commands;
using Rhino.UI;
using System.Windows.Forms;

namespace examples_cs
{
  public class ReplaceColorDialogCommand : Command
  {
    public override string EnglishName { get { return "csReplaceColorDialog"; } }

    private ColorDialog m_dlg = null;

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Dialogs.SetCustomColorDialog(OnSetCustomColorDialog);
      return Result.Success;
    }

    void OnSetCustomColorDialog(object sender, GetColorEventArgs e)
    {
      m_dlg = new ColorDialog();
      if (m_dlg.ShowDialog(null) == DialogResult.OK)
      {
        var c = m_dlg.Color;
        e.SelectedColor = c;
      }
    }
  }
}