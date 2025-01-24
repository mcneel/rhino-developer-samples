using Eto.Drawing;
using Eto.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.UI;

namespace SampleCsEto.Commands
{
  public class SampleCsEtoCoffee : Rhino.Commands.Command
  {
    public override string EnglishName => "SampleCsEtoCoffee";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      CoffeeDialog form = new CoffeeDialog();
      Result rc = form.ShowModal(RhinoEtoApp.MainWindow);
      return rc;
    }
  }

  internal class CoffeeDialog : Dialog<Rhino.Commands.Result>
  {
    public CoffeeDialog()
    {
      Title = "Coffee";
      Resizable = true;

      TestSeparator sep0 = new TestSeparator { Text = "Roast" };
      CheckBox chk0 = new CheckBox { Text = "Light roast" };
      CheckBox chk1 = new CheckBox { Text = "Medium roast" };
      CheckBox chk2 = new CheckBox { Text = "Medium-Dark roast" };
      CheckBox chk3 = new CheckBox { Text = "Dark roast" };

      TestSeparator sep1 = new TestSeparator { Text = "Flavor" };
      CheckBox chk4 = new CheckBox { Text = "Mild" };
      CheckBox chk5 = new CheckBox { Text = "Bold" };
      CheckBox chk6 = new CheckBox { Text = "Extra bold" };

      TestSeparator sep2 = new TestSeparator { Text = "Temperature" };
      CheckBox chk7 = new CheckBox { Text = "Cold" };
      CheckBox chk8 = new CheckBox { Text = "Warm" };
      CheckBox chk9 = new CheckBox { Text = "Hot" };

      DefaultButton = new Button { Text = "OK" };
      DefaultButton.Click += (sender, e) => Close(Rhino.Commands.Result.Success);

      AbortButton = new Button { Text = "C&ancel" };
      AbortButton.Click += (sender, e) => Close(Rhino.Commands.Result.Cancel);

      StackLayout buttons = new StackLayout
      {
        Orientation = Orientation.Horizontal,
        Spacing = 5,
        Items = { null, DefaultButton, AbortButton }
      };

      Content = new StackLayout
      {
        Padding = new Padding(10),
        HorizontalContentAlignment = HorizontalAlignment.Left,
        Items =
        {
          new StackLayoutItem(sep0, HorizontalAlignment.Stretch),
          new TableLayout
          {
            Padding = 10,
            Rows = { chk0, chk1, chk2, chk3 }
          },
          new StackLayoutItem(sep1, HorizontalAlignment.Stretch),
          new TableLayout
          {
            Padding = 10,
            Rows = { chk4, chk5, chk6 }
          },
          new StackLayoutItem(sep2, HorizontalAlignment.Stretch),
          new TableLayout
          {
            Padding = 10,
            Rows = { chk7, chk8, chk9 }
          },
          null,
          buttons
        }
      };
    }
  }

  /// <summary>
  /// label with a line separator
  /// </summary>
  internal class TestSeparator : Panel
  {
    readonly Label m_label;
    readonly SampleCsDivider m_divider;

    public string Text
    {
      get { return m_label.Text; }
      set { m_label.Text = value; }
    }

    public Color Color
    {
      get { return m_divider.Color; }
      set { m_divider.Color = value; }
    }

    public TestSeparator()
    {
      m_label = new Label();
      m_divider = new SampleCsDivider { Color = Colors.DarkGray };

      Content = new StackLayout
      {
        Orientation = Orientation.Horizontal,
        VerticalContentAlignment = VerticalAlignment.Stretch,
        Spacing = 2,
        Items =
        {
          m_label,
          new StackLayoutItem(m_divider, true)
        }
      };
    }
  }

  /// <summary>
  /// Line separator
  /// </summary>
  internal class SampleCsDivider : Eto.Forms.Drawable
  {
    private Eto.Drawing.Color m_color;

    public Eto.Drawing.Color Color
    {
      get { return m_color; }
      set
      {
        if (m_color == value)
          return;
        m_color = value;
        Invalidate();
      }
    }

    public Orientation Orientation => Width < Height
      ? Orientation.Vertical
      : Orientation.Horizontal;

    public SampleCsDivider()
    {
      m_color = Colors.DarkGray;
      Size = new Size(3, 3);
    }

    protected override void OnSizeChanged(System.EventArgs e)
    {
      base.OnSizeChanged(e);
      Invalidate();
    }

    protected override void OnLoadComplete(System.EventArgs e)
    {
      base.OnLoadComplete(e);
      Invalidate();
    }

    protected override void OnPaint(PaintEventArgs e)
    {
      base.OnPaint(e);
      PointF middle = new PointF(Size / 2);
      e.Graphics.FillRectangle(
        Color,
        Orientation == Orientation.Horizontal
          ? new RectangleF(0f, middle.Y, ClientSize.Width, 1)
          : new RectangleF(middle.Y, 0f, 1, ClientSize.Height));
    }
  }
}