using Eto.Drawing;
using Eto.Forms;

namespace SampleCsEto.Views
{
  /// <summary>
  /// Warning! Modelers forms are not currently supported on Mac, you should use
  /// Panels for cross platform mode-less UI.
  /// </summary>
  class SampleCsEtoModelessForm : Form
  {
    public SampleCsEtoModelessForm()
    {
      Maximizable = false;
      Minimizable = false;
      Padding = new Padding(5);
      Resizable = false;
      ShowInTaskbar = false;
      Title = GetType().Name;
      WindowStyle = WindowStyle.Default;

      var hello_button = new Button { Text = "Hello" };
      hello_button.Click += (sender, e) => OnHelloButton();

      var close_button = new Button { Text = "OK" };
      close_button.Click += (sender, e) => Close();

      var hello_layout = new TableLayout
      {
        Padding = new Padding(5, 10, 5, 5),
        Spacing = new Size(5, 5),
        Rows = { new TableRow(null, hello_button, null) }
      };

      var close_layout = new TableLayout
      {
        Padding = new Padding(5, 10, 5, 5),
        Spacing = new Size(5, 5),
        Rows = { new TableRow(null, close_button, null) }
      };

      Content = new TableLayout
      {
        Padding = new Padding(5),
        Spacing = new Size(5, 5),
        Rows =
        {
          new TableRow(hello_layout),
          new TableRow(close_layout)
        }
      };
    }

    protected void OnHelloButton()
    {
      MessageBox.Show(this, "Hello Rhino!", Title, MessageBoxButtons.OK);
    }
  }
}
