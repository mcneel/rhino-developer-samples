namespace SampleCsNativeMultiArch
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  public class SampleCsNativeMultiArchPlugin : Rhino.PlugIns.PlugIn
  {
    ///<summary>Gets the only instance of the SampleCsNativeMultiArchPlugin plug-in.</summary>
    public static SampleCsNativeMultiArchPlugin Instance { get; private set; }

    public SampleCsNativeMultiArchPlugin()
    {
      Instance = this;
    }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and mantain plug-in wide options in a document.
  }
}
