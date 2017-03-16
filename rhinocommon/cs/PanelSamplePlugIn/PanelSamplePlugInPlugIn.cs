namespace PanelSamplePlugIn
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  public class PanelSamplePlugInPlugIn : Rhino.PlugIns.PlugIn
  {
    public PanelSamplePlugInPlugIn()
    {
      Instance = this;
    }

    ///<summary>Gets the only instance of the PanelSamplePlugInPlugIn plug-in.</summary>
    public static PanelSamplePlugInPlugIn Instance
    {
      get;
      private set;
    }

    /// <summary>
    /// Override OnLoad to register my custom panel type with Rhino when my plug-in
    /// is loaded.
    /// </summary>
    /// <param name="errorMessage"></param>
    /// <returns></returns>
    protected override Rhino.PlugIns.LoadReturnCode OnLoad(ref string errorMessage)
    {
      // Get MyPanel class type
      System.Type panelType = typeof(MyPanel);
      // Register my custom panel class type with Rhino, the custom panel my be display
      // by running the MyOpenPanel command and hidden by running the MyClosePanel command.
      // You can also include the custom panel in any existing panel group by simply right
      // clicking one a panel tab and checking or un-checking the "MyPane" option.
      Rhino.UI.Panels.RegisterPanel(this, panelType, "MyPanel", Properties.Resources.MyPlugIn);
      return Rhino.PlugIns.LoadReturnCode.Success;
    }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and maintain plug-in wide options in a document.
  }

  //TODO: Create your own Guid for this class
  [System.Runtime.InteropServices.Guid("010D635C-47C3-4B46-8585-7D32A786DC9E")]
  public class MyOpenPanelCommand : Rhino.Commands.Command
  {
    public MyOpenPanelCommand()
    {
      // Rhino only creates one instance of each command class defined in a
      // plug-in, so it is safe to store a reference in a static property.
      Instance = this;
    }

    ///<summary>The only instance of this command.</summary>
    public static MyOpenPanelCommand Instance
    {
      get;
      private set;
    }

    ///<returns>The command name as it appears on the Rhino command line.</returns>
    public override string EnglishName
    {
      get { return "MyOpenPanel"; }
    }

    protected override Rhino.Commands.Result RunCommand(Rhino.RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      var type = typeof(MyPanel);
      Rhino.UI.Panels.OpenPanel(type.GUID);
      return Rhino.Commands.Result.Success;
    }
  }

  //TODO: Create your own Guid for this class
  [System.Runtime.InteropServices.Guid("B47E23E3-022B-4F78-97D7-70C7ED2CEC1B")]
  public class MyClosePanelCommand : Rhino.Commands.Command
  {
    public MyClosePanelCommand()
    {
      // Rhino only creates one instance of each command class defined in a
      // plug-in, so it is safe to store a reference in a static property.
      Instance = this;
    }

    ///<summary>The only instance of this command.</summary>
    public static MyClosePanelCommand Instance
    {
      get;
      private set;
    }

    ///<returns>The command name as it appears on the Rhino command line.</returns>
    public override string EnglishName
    {
      get { return "MyClosePanel"; }
    }

    protected override Rhino.Commands.Result RunCommand(Rhino.RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      // Hide my custom panel
      var type = typeof(MyPanel);
      Rhino.UI.Panels.ClosePanel(type.GUID);
      return Rhino.Commands.Result.Success;
    }
  }

}