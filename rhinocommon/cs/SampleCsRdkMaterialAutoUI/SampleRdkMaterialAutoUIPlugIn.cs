using Rhino;
using Rhino.Render;
using Rhino.Commands;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Rhino.PlugIns;

namespace SampleRdkMaterialAutoUI
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  public class SampleRdkMaterialAutoUIPlugIn : Rhino.PlugIns.PlugIn
  {
    private SampleRenderContentSerializer m_rcs;
    private static Guid m_plugin_id = new Guid("3285f192-ecac-40ee-a989-0930756d8f41");

    public SampleRdkMaterialAutoUIPlugIn()
    {
      Instance = this;

      // Create our custom serializer for the material (.sample files)
      m_rcs = new SampleRenderContentSerializer("sample", RenderContentKind.Material, true, true);

      // Register Serializer
      m_rcs.RegisterSerializer(m_plugin_id);
    }

    ///<summary>Gets the only instance of the SampleRdkMaterialAutoUIPlugIn plug-in.</summary>
    public static SampleRdkMaterialAutoUIPlugIn Instance
    {
      get; private set;
    }


    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and mantain plug-in wide options in a document.
    
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      base.OnLoad(ref errorMessage);

      RenderContent.RegisterContent(this);

      return LoadReturnCode.Success;
    }

  }
}