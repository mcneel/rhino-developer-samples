using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using Rhino;
using Rhino.DocObjects;

namespace SampleCsWpf.Forms
{
  /// <summary>
  /// Interaction logic for SampleCsWpfPanelUserControl.xaml
  /// </summary>
  public partial class SampleCsWpfPanelUserControl : UserControl
  {
    public bool IsEventsEnabled { get; private set; }

    public SampleCsWpfPanelUserControl()
    {
      InitializeComponent();

      IsEventsEnabled = false;
      IsVisibleChanged += OnPanelVisibleChanged;
    }

    /// <summary>
    /// Handles IsVisible change events
    /// </summary>
    private void OnPanelVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
    {
      DebugWriteMethod();
      EnablePanelEvents((bool) e.NewValue);
    }

    /// <summary>
    /// Enables or disables panel event watchers
    /// </summary>
    private void EnablePanelEvents(bool bEnable)
    {
      if (bEnable != IsEnabled)
      {
        if (bEnable)
        {
          RhinoDoc.SelectObjects += OnSelectObjects;
          RhinoDoc.DeselectObjects += OnDeselectObjects;
          RhinoDoc.DeselectAllObjects += OnDeselectAllObjects;
        }
        else
        {
          RhinoDoc.SelectObjects -= OnSelectObjects;
          RhinoDoc.DeselectObjects -= OnDeselectObjects;
          RhinoDoc.DeselectAllObjects -= OnDeselectAllObjects;
        }
      }
      IsEnabled = bEnable;
    }

    #region Object Select Events

    /// <summary>
    /// Called when objects are selected
    /// </summary>
    private void OnSelectObjects(object sender, RhinoObjectSelectionEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when objects are deselected
    /// </summary>
    private void OnDeselectObjects(object sender, RhinoObjectSelectionEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when all objects are deselected
    /// </summary>
    private void OnDeselectAllObjects(object sender, RhinoDeselectAllObjectsEventArgs e)
    {
      DebugWriteMethod();
    }

    private void DebugWriteMethod()
    {
#if DEBUG
      var class_name = GetType().Name;
      var stack_trace = new StackTrace();
      var method_name = stack_trace.GetFrame(1).GetMethod().Name;
      RhinoApp.WriteLine("** {0}.{1} called **", class_name, method_name);
#endif
    }

    #endregion
  }
}
