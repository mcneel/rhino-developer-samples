using Rhino;
using Rhino.Commands;

namespace SampleCustomLightManager
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  public class SampleCustomLightManagerPlugIn : Rhino.PlugIns.RenderPlugIn

  {
    public SampleCustomLightManagerPlugIn()
    {
      Instance = this;
    }

    ///<summary>Gets the only instance of the SampleCustomLightManagerPlugIn plug-in.</summary>
    public static SampleCustomLightManagerPlugIn Instance
    {
      get; private set;
    }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and mantain plug-in wide options in a document.

    protected override Result Render(RhinoDoc doc, RunMode mode, bool fastPreview)
    {
      return Result.Success;
    }
  }
}