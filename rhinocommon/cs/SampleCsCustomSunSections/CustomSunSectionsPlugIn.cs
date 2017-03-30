using Rhino.UI.Controls;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using System;

namespace CustomSunSections
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  ///

  public class CustomSunSectionsPlugIn : Rhino.PlugIns.RenderPlugIn
  {
    private CustomSunSection1 m_sun_section1;
    private CustomSunSection2 m_sun_section2;

    public CustomSunSectionsPlugIn()
    {
      Instance = this;

      // Create two custom sections
      m_sun_section1 = new CustomSunSection1();
      m_sun_section2 = new CustomSunSection2();
    }

    ///<summary>Gets the only instance of the CustomSunSectionsPlugIn plug-in.</summary>
    public static CustomSunSectionsPlugIn Instance
    {
      get; private set;
    }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and mantain plug-in wide options in a document.

    // Return the custom sections that we want display in the sun dialog
    public override void SunCustomSections(List<ICollapsibleSection> sections)
    {
      sections.Add(m_sun_section1);
      sections.Add(m_sun_section2);
    }

    protected override Result Render(RhinoDoc doc, RunMode mode, bool fastPreview)
    {
      return Result.Success;
    }


  }
}