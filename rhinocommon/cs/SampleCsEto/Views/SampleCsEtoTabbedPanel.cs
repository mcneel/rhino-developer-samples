using Eto.Drawing;
using Eto.Forms;
using Rhino.UI;
using System;

namespace SampleCsEto.Views
{
  /// <summary>
  /// A dockable panel that hosts an Eto TabControl.
  ///
  /// Note, the class registered with Rhino.UI.Panels.RegisterPanel is the
  /// control Rhino sites in the panel host. Derive it from Eto.Forms.Panel and
  /// set its Content to the TabControl, as shown below, rather than registering
  /// a class derived from TabControl itself. Registering the tab control
  /// directly has been reported to stop responding to mouse clicks once focus
  /// moves to a viewport and back.
  /// </summary>
  [System.Runtime.InteropServices.Guid("17F62218-B7F9-4A1E-B096-597FBAE55961")]
  public class SampleCsEtoTabbedPanel : Panel, IPanel
  {
    private readonly uint m_document_sn;
    private readonly Random m_random = new Random();
    private readonly TabControl m_tabs;

    /// <summary>
    /// Provide easy access to the SampleCsEtoTabbedPanel.GUID
    /// </summary>
    public static Guid PanelId => typeof(SampleCsEtoTabbedPanel).GUID;

    /// <summary>
    /// Required public constructor with a single document serial number parameter
    /// </summary>
    public SampleCsEtoTabbedPanel(uint documentSerialNumber)
    {
      m_document_sn = documentSerialNumber;

      Title = GetType().Name;

      m_tabs = CreateTabControl();

      Button add_button = new Button { Text = "Add Tab" };
      add_button.Click += OnAddTabButton;

      Button remove_button = new Button { Text = "Remove Tab" };
      remove_button.Click += OnRemoveTabButton;

      Button select_button = new Button { Text = "Select Tab" };
      select_button.Click += OnSelectTabButton;

      StackLayout button_layout = new StackLayout
      {
        Orientation = Orientation.Horizontal,
        Spacing = 5
      };
      button_layout.Items.Add(new StackLayoutItem(add_button));
      button_layout.Items.Add(new StackLayoutItem(remove_button));
      button_layout.Items.Add(new StackLayoutItem(select_button));
      button_layout.Items.Add(null);

      StackLayout layout = new StackLayout
      {
        Spacing = 5,
        Padding = new Padding(10),
        HorizontalContentAlignment = HorizontalAlignment.Stretch
      };
      layout.Items.Add(new StackLayoutItem(button_layout));
      // The 'true' expands the tab control to fill the remaining panel space
      layout.Items.Add(new StackLayoutItem(m_tabs, true));

      // This panel's content is the layout - the TabControl is a child of it
      Content = layout;
    }

    /// <summary>
    /// The panel title, displayed by the panel host
    /// </summary>
    public string Title { get; }

    /// <summary>
    /// Creates the one and only tab control, with two pages
    /// </summary>
    private TabControl CreateTabControl()
    {
      TabControl control = new TabControl { TabPosition = DockPosition.Top };
      control.SelectedIndexChanged += OnSelectedIndexChanged;

      control.Pages.Add(new TabPage { Text = "Tab 1", Content = CreateLabelPage() });
      control.Pages.Add(new TabPage { Text = "Tab 2", Content = CreateTextAreaPage() });

      return control;
    }

    /// <summary>
    /// Creates a tab page's content - a simple text label
    /// </summary>
    private Control CreateLabelPage()
    {
      Label label = new Label { Text = "Text Label" };

      DynamicLayout layout = new DynamicLayout
      {
        DefaultSpacing = new Size(5, 5),
        Padding = new Padding(10)
      };
      layout.Add(label);
      layout.Add(null);

      return new Panel { Content = layout };
    }

    /// <summary>
    /// Creates a tab page's content - a scrollable text area
    /// </summary>
    private Control CreateTextAreaPage()
    {
      TextArea text = new TextArea { Text = "Every Good Boy Deserves Fudge." };

      TableLayout layout = new TableLayout
      {
        Padding = new Padding(10),
        Spacing = new Size(5, 5)
      };
      layout.Rows.Add(new TableRow(new TableCell(text, true)) { ScaleHeight = true });

      return new Scrollable { Content = layout };
    }

    /// <summary>
    /// Add Tab button click handler
    /// </summary>
    private void OnAddTabButton(object sender, EventArgs e)
    {
      TabPage page = new TabPage { Text = $"Tab {m_tabs.Pages.Count + 1}" };
      page.Content = (m_tabs.Pages.Count % 2 == 0) ? CreateLabelPage() : CreateTextAreaPage();
      m_tabs.Pages.Add(page);
    }

    /// <summary>
    /// Remove Tab button click handler
    /// </summary>
    private void OnRemoveTabButton(object sender, EventArgs e)
    {
      if (m_tabs.SelectedIndex >= 0 && m_tabs.Pages.Count > 0)
        m_tabs.Pages.RemoveAt(m_tabs.SelectedIndex);
    }

    /// <summary>
    /// Select Tab button click handler - selects a random tab
    /// </summary>
    private void OnSelectTabButton(object sender, EventArgs e)
    {
      if (m_tabs.Pages.Count > 0)
        m_tabs.SelectedIndex = m_random.Next(m_tabs.Pages.Count);
    }

    /// <summary>
    /// TabControl selection changed handler
    /// </summary>
    private void OnSelectedIndexChanged(object sender, EventArgs e)
    {
      if (null != m_tabs.SelectedPage)
        Rhino.RhinoApp.WriteLine($"Selected tab: {m_tabs.SelectedPage.Text}");
    }

    #region IPanel methods
    /// <summary>
    /// Called when the panel tab is made visible
    /// </summary>
    public void PanelShown(uint documentSerialNumber, ShowPanelReason reason)
    {
      Rhino.RhinoApp.WriteLine($"Panel shown for document {documentSerialNumber}, this serial number {m_document_sn} should be the same");
    }

    /// <summary>
    /// Called when the panel tab is hidden
    /// </summary>
    public void PanelHidden(uint documentSerialNumber, ShowPanelReason reason)
    {
      Rhino.RhinoApp.WriteLine($"Panel hidden for document {documentSerialNumber}, this serial number {m_document_sn} should be the same");
    }

    /// <summary>
    /// Called when the document or panel container is closed/destroyed
    /// </summary>
    public void PanelClosing(uint documentSerialNumber, bool onCloseDocument)
    {
      Rhino.RhinoApp.WriteLine($"Panel closing for document {documentSerialNumber}, this serial number {m_document_sn} should be the same");
    }
    #endregion IPanel methods
  }
}
