using System;
using System.Windows.Forms;

namespace SampleCsWinForms.Forms
{
  /// <summary>
  /// This is the user control that is buried in the tabbed, docking panel.
  /// </summary>
  [System.Runtime.InteropServices.Guid("83D6FCC8-4F31-4AE3-BF60-C6528DB232D0")]
  public partial class SampleCsPanelUserControl : UserControl
  {
    /// <summary>
    /// Public constructor
    /// </summary>
    public SampleCsPanelUserControl()
    {
      InitializeComponent();

      // Set the user control property on our plug-in
      SampleCsWinFormsPlugIn.Instance.PanelUserControl = this;

      // Create a visible changed event handler
      VisibleChanged += OnVisibleChanged;

      // Create a dispose event handler
      Disposed += OnUserControlDisposed;
    }

    void OnVisibleChanged(object sender, EventArgs e)
    {
      // TODO...
    }

    /// <summary>
    /// Occurs when the component is disposed by a call to the
    /// System.ComponentModel.Component.Dispose() method.
    /// </summary>
    void OnUserControlDisposed(object sender, EventArgs e)
    {
      // Clear the user control property on our plug-in
      SampleCsWinFormsPlugIn.Instance.PanelUserControl = null;
    }

    /// <summary>
    /// Returns the ID of this panel.
    /// </summary>
    public static Guid PanelId => typeof(SampleCsPanelUserControl).GUID;
  }
}
