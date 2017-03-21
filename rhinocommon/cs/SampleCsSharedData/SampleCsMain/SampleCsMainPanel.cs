using System;
using System.Windows.Forms;
using SampleCsCommon;

namespace SampleCsMain
{
  /// <summary>
  /// This is the user control that is buried in the tabbed, docking panel.
  /// </summary>
  [System.Runtime.InteropServices.Guid("866030B5-359C-46BF-B13C-EC5AB4162D1F")]
  public partial class SampleCsMainPanel : UserControl
  {
    #region Private Members
    private SampleCsStringTableHelpers.OnAddEvent m_on_add_event;
    private SampleCsStringTableHelpers.OnInsertEvent m_insert_event;
    private SampleCsStringTableHelpers.OnRemoveEvent m_on_remove_event;
    private SampleCsStringTableHelpers.OnRemoveAtEvent m_on_remove_at_event;
    private SampleCsStringTableHelpers.OnClearEvent m_on_clear_event;
    private SampleCsStringTableHelpers.OnRefreshEvent m_on_refresh_event;
    #endregion

    /// <summary>
    /// Public Constructor.
    /// </summary>
    public SampleCsMainPanel()
    {
      InitializeComponent();

      // Set the user control property on our plug-in
      SampleCsMainPlugIn.Instance.Panel = this;

      // Initialize event handlers
      m_on_add_event = new SampleCsStringTableHelpers.OnAddEvent(SampleCsStringTableHelpers_OnAdd);
      m_insert_event = new SampleCsStringTableHelpers.OnInsertEvent(SampleCsStringTableHelpers_OnInsert);
      m_on_remove_event = new SampleCsStringTableHelpers.OnRemoveEvent(SampleCsStringTableHelpers_OnRemove);
      m_on_remove_at_event = new SampleCsStringTableHelpers.OnRemoveAtEvent(SampleCsStringTableHelpers_OnRemoveAt);
      m_on_clear_event = new SampleCsStringTableHelpers.OnClearEvent(SampleCsStringTableHelpers_OnClear);
      m_on_refresh_event = new SampleCsStringTableHelpers.OnRefreshEvent(SampleCsStringTableHelpers_OnRefresh);

      // Create a visible changed event handler
      this.VisibleChanged += new EventHandler(SampleCsMainPanel_VisibleChanged);

      // Create a dispose event handler
      this.Disposed += new EventHandler(SampleCsMainPanel_Disposed);
    }

    #region Event Handlers
    /// <summary>
    /// The panel is being disposed.
    /// </summary>
    void SampleCsMainPanel_Disposed(object sender, EventArgs e)
    {
      // Clear the user control property on our plug-in
      SampleCsMainPlugIn.Instance.Panel = null;
    }

    /// <summary>
    /// The visibility of the panel has changed.
    /// </summary>
    void SampleCsMainPanel_VisibleChanged(object sender, EventArgs e)
    {
      if (this.Visible)
      {
        SampleCsStringTableHelpers.OnAdd += m_on_add_event;
        SampleCsStringTableHelpers.OnInsert += m_insert_event;
        SampleCsStringTableHelpers.OnRemove += m_on_remove_event;
        SampleCsStringTableHelpers.OnRemoveAt += m_on_remove_at_event;
        SampleCsStringTableHelpers.OnClear += m_on_clear_event;
        SampleCsStringTableHelpers.OnRefresh += m_on_refresh_event;
        RefreshListBox(true);
      }
      else
      {
        SampleCsStringTableHelpers.OnAdd -= m_on_add_event;
        SampleCsStringTableHelpers.OnInsert -= m_insert_event;
        SampleCsStringTableHelpers.OnRemove -= m_on_remove_event;
        SampleCsStringTableHelpers.OnRemoveAt -= m_on_remove_at_event;
        SampleCsStringTableHelpers.OnClear -= m_on_clear_event;
        SampleCsStringTableHelpers.OnRefresh -= m_on_refresh_event;
        RefreshListBox(false);
      }
    }

    /// <summary>
    /// SampleCsStringTableHelpers_OnRefresh event handler
    /// </summary>
    void SampleCsStringTableHelpers_OnRefresh()
    {
      RefreshListBox(true);
    }

    /// <summary>
    /// SampleCsStringTableHelpers_OnClear event handler
    /// </summary>
    void SampleCsStringTableHelpers_OnClear()
    {
      RefreshListBox(false);
    }

    /// <summary>
    /// SampleCsStringTableHelpers_OnRemoveAt event handler
    /// </summary>
    void SampleCsStringTableHelpers_OnRemoveAt(int index)
    {
      RefreshListBox(true);
    }

    /// <summary>
    /// SampleCsStringTableHelpers_OnRemove event handler
    /// </summary>
    void SampleCsStringTableHelpers_OnRemove(string str)
    {
      RefreshListBox(true);
    }

    /// <summary>
    /// SampleCsStringTableHelpers_OnInsert event handler
    /// </summary>
    void SampleCsStringTableHelpers_OnInsert(int index, string str)
    {
      RefreshListBox(true);
    }

    /// <summary>
    /// SampleCsStringTableHelpers_OnAdd event handler
    /// </summary>
    void SampleCsStringTableHelpers_OnAdd(string str)
    {
      RefreshListBox(true);
    }
    #endregion

    #region Public Methods
    /// <summary>
    /// Clears or fills the list box.
    /// </summary>
    void RefreshListBox(bool bFill)
    {
      if (this.Visible)
      {
        this.listBox.Items.Clear();
        if (bFill)
        {
          for (int i = 0; i < SampleCsStringTableHelpers.Count(); i++)
            this.listBox.Items.Add(SampleCsStringTableHelpers.GetAt(i));
        }
      }
    }

    /// <summary>
    /// Returns the ID of this panel.
    /// </summary>
    public static System.Guid PanelId
    {
      get
      {
        return typeof(SampleCsMainPanel).GUID;
      }
    }
    #endregion
  }
}
