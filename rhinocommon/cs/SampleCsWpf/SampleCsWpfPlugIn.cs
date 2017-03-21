using Rhino.PlugIns;
using Rhino.UI;
using SampleCsWpf.Forms;

namespace SampleCsWpf
{
  ///<summary>
  /// Every RhinoCommon plug-in must have one and only one Rhino.PlugIns.PlugIn
  /// inherited class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.
  ///</summary>
  public class SampleCsWpfPlugIn : Rhino.PlugIns.PlugIn
  {
    public SampleCsWpfPlugIn()
    {
      Instance = this;
    }

    ///<summary>Gets the only instance of the SampleCsWpfPlugIn plug-in.</summary>
    public static SampleCsWpfPlugIn Instance
    {
      get;
      private set;
    }

    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      System.Type panel_type = typeof(SampleCsWpfPanelHost);
      Panels.RegisterPanel(this, panel_type, "SampleWpf", Properties.Resources.SampleCsWpfPanel);
      return LoadReturnCode.Success;
    }
  }

  /// <summary>
  /// Rhino framework requires a System.Windows.Forms.IWin32Window derived object for a panel.
  /// </summary>
  [System.Runtime.InteropServices.Guid("AB80CF1B-B499-42EC-B25D-EF1B41220C7D")]
  public class SampleCsWpfPanelHost : RhinoWindows.Controls.WpfElementHost
  {
    public SampleCsWpfPanelHost()
      : base(new SampleCsWpfPanelUserControl(), null) // No view model (for this example)
    {
    }

    /// <summary>
    /// Returns the ID of this panel.
    /// </summary>
    public static System.Guid PanelId
    {
      get
      {
        return typeof(SampleCsWpfPanelHost).GUID;
      }
    }
  }
}
