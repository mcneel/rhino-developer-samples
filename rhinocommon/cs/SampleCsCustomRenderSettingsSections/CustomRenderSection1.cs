using System;
using Eto.Forms;
using Rhino.UI;

namespace SampleCustomRenderSettingsSections
{
  ///<summary>
  /// The UI implementation of of Section one
  ///</summary>
  public class CustomRenderSection1 : CustomSection
  {
    private CustomRenderSettingsViewModel m_view_model;
    private Button m_button;
    private CheckBox m_checkbox;
    private Label m_button_lb;
    private Label m_checkbox_lb;
    private Label m_button_clicks_lb;
    private Label m_checkbox_value_lb;
    private int m_clicks;
    private LocalizeStringPair m_caption;

    public CustomRenderSettingsViewModel CustomRenderSettingsViewModel
    {
      get
      {
        return m_view_model;
      }

      set
      {
        m_view_model = value;
      }
    }

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
    public CustomRenderSection1()
    {
      m_caption = new LocalizeStringPair("Custom Render Section1", "Custom Render Section1");
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
        Text = "Custom User Data",
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
                    new TableRow(m_checkbox_value_lb, m_checkbox, m_checkbox_lb)
                }
      };
      Content = layout;
    }

    private void RegisterControlEvents()
    {
      DataChanged += OnDataChanged;

      m_checkbox.CheckedChanged += OnCheckedChanged;
      m_button.Click += OnButtonClick;
    }

    private void UnRegisterControlEvents()
    {
      DataChanged += OnDataChanged;

      m_checkbox.CheckedChanged -= OnCheckedChanged;
      m_button.Click -= OnButtonClick;
    }


    private void OnLoadComplete(object sender, EventArgs e)
    {

    }

    public override void OnViewModelActivated(object sender, EventArgs e)
    {
      base.OnViewModelActivated(sender, e);

      // Setup the viewmodel for the section
      if (CustomRenderSettingsViewModel == null)
        CustomRenderSettingsViewModel = new CustomRenderSettingsViewModel(this);
    }

    void OnDataChanged(object sender, Rhino.UI.Controls.DataSource.EventArgs args)
    {
      m_checkbox.CheckedChanged -= OnCheckedChanged;
      m_checkbox.Checked = CustomRenderSettingsViewModel.CheckBoxValue;
      m_checkbox.CheckedChanged += OnCheckedChanged;
    }

    private void DisplayData()
    {
      m_checkbox.Checked = CustomRenderSettingsViewModel.CheckBoxValue;
    }


    private void OnCheckedChanged(object sender, EventArgs e)
    {
      m_checkbox_lb.Text = m_checkbox.Checked.ToString();
      
      if (m_checkbox.Checked != null)
      {
        bool checked_state = (bool)m_checkbox.Checked;
        CustomRenderSettingsViewModel.CheckBoxValue = checked_state;
      }
    }

    private void OnButtonClick(object sender, EventArgs e)
    {
      m_clicks++;
      m_button_lb.Text = m_clicks.ToString();
    }

  }
}
