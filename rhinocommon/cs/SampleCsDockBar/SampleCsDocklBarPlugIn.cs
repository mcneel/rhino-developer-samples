using System;
using Rhino.PlugIns;
using RhinoWindows.Controls;

namespace SampleCsDockBar
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  public class SampleCsDocklBarPlugIn : Rhino.PlugIns.PlugIn
  {
    private WinFormsDockBar m_winforms_bar;
    private WpfDockBar m_wpf_bar;

    public SampleCsDocklBarPlugIn()
    {
      Instance = this;
    }

    ///<summary>Gets the only instance of the WpfInControlBarPlugIn plug-in.</summary>
    public static SampleCsDocklBarPlugIn Instance
    {
      get; private set;
    }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and maintain plug-in wide options in a document.

    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      CreateMyDockBar();
      return base.OnLoad(ref errorMessage);
    }
    
    private void CreateMyDockBar()
    {
      var create_options = new DockBarCreateOptions
      {
        DockLocation = DockBarDockLocation.Right,
        Visible = false,
        DockStyle = DockBarDockStyle.Any,
        FloatPoint = new System.Drawing.Point(100, 100)
      };

      m_winforms_bar = new WinFormsDockBar();
      m_winforms_bar.Create(create_options);

      m_wpf_bar = new WpfDockBar();
      m_wpf_bar.Create(create_options);
    }

  }

  /// <summary>
  /// WinFormsDockBar dockbar class
  /// </summary>
  internal class WinFormsDockBar : DockBar
  {
    public static Guid BarId => new Guid("{b924b685-160e-461d-adf9-c2b457965ac6}");
    public WinFormsDockBar() : base(SampleCsDocklBarPlugIn.Instance, BarId, "WinForms")
    {
      SetContentControl(new SampleCsWinFormPanel());
    }
  }

  /// <summary>
  /// WpfDockBar dockbar class
  /// </summary>
  internal class WpfDockBar : DockBar
  {
    public static Guid BarId => new Guid("{c520731e-376a-4d82-975a-403664fca2fc}");
    public WpfDockBar() : base(SampleCsDocklBarPlugIn.Instance, BarId, "WPF")
    {
      SetContentControl(new WpfHost(new DockBarPanel(), null));
    }
  }
}