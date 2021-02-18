using Eto.Drawing;
using Eto.Forms;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.UI;
using System;
using System.ComponentModel;

namespace SampleCsEto.Views
{
  class SampleCsEtoSemiModalDialog : Dialog<DialogResult>
  {
    public SampleCsEtoSemiModalDialog()
    {
      Padding = new Padding(5);
      Resizable = false;
      Result = DialogResult.Cancel;
      Title = GetType().Name;
      WindowStyle = WindowStyle.Default;

      var hello_button = new Button { Text = "Pick" };
      hello_button.Click += (sender, e) => this.PushPickButton((s, e2) => PickFunction());

      DefaultButton = new Button { Text = "OK" };
      DefaultButton.Click += (sender, e) => Close(DialogResult.Ok);

      AbortButton = new Button { Text = "Cancel" };
      AbortButton.Click += (sender, e) => Close(DialogResult.Cancel);

      var button_layout = new TableLayout
      {
        Padding = new Padding(5, 10, 5, 5),
        Spacing = new Size(5, 5),
        Rows = { new TableRow(null, hello_button, null) }
      };

      var defaults_layout = new TableLayout
      {
        Padding = new Padding(5, 10, 5, 5),
        Spacing = new Size(5, 5),
        Rows = { new TableRow(null, DefaultButton, AbortButton, null) }
      };

      Content = new TableLayout
      {
        Padding = new Padding(5),
        Spacing = new Size(5, 5),
        Rows =
        {
          new TableRow(button_layout),
          new TableRow(defaults_layout)
        }
      };
    }

    protected override void OnLoadComplete(EventArgs e)
    {
      base.OnLoadComplete(e);
      this.RestorePosition();
    }

    protected override void OnClosing(CancelEventArgs e)
    {
      this.SavePosition();
      base.OnClosing(e);
    }

    protected void PickFunction()
    {
      Point3d point;
      if (Rhino.Commands.Result.Success == RhinoGet.GetPoint("Location for new point", false, out point))
        MessageBox.Show(point.ToString(), Title, MessageBoxButtons.OK);
    }
  }
}
