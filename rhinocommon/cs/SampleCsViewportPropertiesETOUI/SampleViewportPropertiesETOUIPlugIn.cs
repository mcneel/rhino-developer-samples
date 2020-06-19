using Rhino;
using Rhino.Commands;
using Rhino.UI;
using System.Collections.Generic;

namespace SampleViewportPropertiesETOUI
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  public class SampleViewportPropertiesETOUIPlugIn : Rhino.PlugIns.RenderPlugIn

  {
    ViewportPropertiesPage m_page;
    public SampleViewportPropertiesETOUIPlugIn()
    {
      Instance = this;

      // Create our own Viewport properties page
      m_page = new ViewportPropertiesPage();
    }

    ///<summary>Gets the only instance of the SampleViewportPropertiesETOUIPlugIn plug-in.</summary>
    public static SampleViewportPropertiesETOUIPlugIn Instance
    {
      get; private set;
    }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and mantain plug-in wide options in a document.
    protected override void ObjectPropertiesPages(ObjectPropertiesPageCollection collection)
    {
      // Add our own Viewport properties page
      collection.Add(m_page);
    }

    protected override Result Render(RhinoDoc doc, RunMode mode, bool fastPreview)
    {
      return Result.Success;
    }
  }
}