using System;
using Rhino.UI.Controls;
using Eto.Forms;
using Rhino.UI;

namespace CustomSunSections
{
  ///<summary>
  /// The UI implementation of of Section one
  ///</summary>
  public class CustomSunSection1 : CustomSection
  {
    private Button m_button;
    private CheckBox m_checkbox;
    private Label m_button_lb;
    private Label m_checkbox_lb;
    private Label m_button_clicks_lb;
    private Label m_checkbox_value_lb;
    private int m_clicks;
    private LocalizeStringPair m_caption;

    ///<summary>
    /// The Heigth of the section
    ///</summary>
    public override int SectionHeight
    {
      get
      {
        return 80;
      }
    }

    public override LocalizeStringPair Caption
    {
      get { return m_caption; }
    }

    ///<summary>
    /// Constructor for SectionOne
    ///</summary>
    public CustomSunSection1()
    {
      m_caption = new LocalizeStringPair("Custom Sun Section1", "Custom Sun Section1");
      InitializeComponents();
      InitializeLayout();
      RegisterControlEvents();
    }

    private void InitializeComponents()
    {
      m_clicks = 0;

      m_button = new Button()
      {
        Text = "Button"
      };

      m_checkbox = new CheckBox();

      m_button_lb = new Label()
      {
        Text = "",
        VerticalAlignment = VerticalAlignment.Center,
        Width = 20
      };

      m_checkbox_lb = new Label()
      {
        Text = "",
        VerticalAlignment = VerticalAlignment.Center,
        Width = 20
      };

      m_button_clicks_lb = new Label()
      {
        Text = "Number of clicks: ",
        VerticalAlignment = VerticalAlignment.Center,
      };

      m_checkbox_value_lb = new Label()
      {
        Text = "Value of checkbox",
        VerticalAlignment = VerticalAlignment.Center,
      };

      m_checkbox_lb.Text = m_checkbox.Checked.ToString();
      m_button_lb.Text = m_clicks.ToString();
    }


    private void InitializeLayout()
    {
      TableLayout layout = new TableLayout()
      {
        // Padding around the table
        Padding = 10,
        // Spacing between table cells
        Spacing = new Eto.Drawing.Size(15, 15),
        Rows =
                {
                    new TableRow(m_button_clicks_lb, m_button_lb, m_button),
                    new TableRow(m_checkbox_value_lb, m_checkbox, m_checkbox_lb),
                }
      };
      Content = layout;
    }

    private void RegisterControlEvents()
    {
      m_checkbox.CheckedChanged += OnCheckedChanged;
      m_button.Click += OnButtonClick;
    }

    private void UnRegisterControlEvents()
    {
      m_checkbox.MouseDoubleClick -= OnCheckedChanged;
      m_button.Click -= OnButtonClick;
    }

    private void OnCheckedChanged(object sender, EventArgs e)
    {
      m_checkbox_lb.Text = m_checkbox.Checked.ToString();
    }

    private void OnButtonClick(object sender, EventArgs e)
    {
      m_clicks++;
      m_button_lb.Text = m_clicks.ToString();
    }

  }
}
