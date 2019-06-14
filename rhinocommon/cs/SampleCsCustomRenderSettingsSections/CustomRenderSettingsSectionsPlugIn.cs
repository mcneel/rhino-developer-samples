using Rhino.UI.Controls;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;

using System;

namespace SampleCustomRenderSettingsSections
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  public class CustomRenderSettingsSectionsPlugIn : Rhino.PlugIns.RenderPlugIn
  {
    // The build in Render Settings Sections in the RDK, These UUIDs start with the prefix
    // uuidRenderSettingsSection and they can be found in RhRdkUuids.h
    private Guid m_uuidRenderSettingsSection_CurrentRenderer = new Guid("dbb37753-0cb3-4ebd-8afc-077ea9edd855");
    private Guid m_uuidRenderSettingsSection_Resolution = new Guid("7d28112a-feee-4cfe-b820-674129dfc047");
    private Guid m_uuidRenderSettingsSection_Background = new Guid("1c2f4520-58f7-4d74-a6b9-7985cecbfa0e");
    private Guid m_uuidRenderSettingsSection_Lighting = new Guid("00cfff90-1e37-c20c-33c7-ffc0f201c0eb");
    private Guid m_uuidRenderSettingsSection_Wireframe = new Guid("e4c4d19c-bd89-42dd-890d-4c44981fe518");
    private Guid m_uuidRenderSettingsSection_ColorAdjustment = new Guid("26126531-70c2-42bc-b50c-3368260dc0b4");
    private Guid m_uuidRenderSettingsSection_FocalBlur = new Guid("8035e79f-0b16-4b2a-ba0f-e277eda7007d");

    public CustomRenderSettingsSectionsPlugIn()
    {
      Instance = this;
    }

    ///<summary>Gets the only instance of the SampleCustomRenderSettingsSectionsPlugIn plug-in.</summary>
    public static CustomRenderSettingsSectionsPlugIn Instance
    {
      get; private set;
    }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and mantain plug-in wide options in a document.
    public override void RenderSettingsCustomSections(List<ICollapsibleSection> sections)
    {
      sections.Add(new CustomRenderSection1());
    }

    /// <summary>
    /// Called to determine which build in rdk render settings sections should be displayed 
    /// for this renderer in additionto the custom sections
    /// </summary>
    protected override List<Guid> RenderSettingsSections()
    {
      List<Guid> types = new List<Guid>();

      // Display Current Renderer section
      types.Add(m_uuidRenderSettingsSection_CurrentRenderer);
      // Display Resolution section
      types.Add(m_uuidRenderSettingsSection_Resolution);

      // Filter away the rest
      //types.Add(m_uuidRenderSettingsSection_Background);
      //types.Add(m_uuidRenderSettingsSection_Lighting);
      //types.Add(m_uuidRenderSettingsSection_Wireframe);
      //types.Add(m_uuidRenderSettingsSection_ColorAdjustment);
      //types.Add(m_uuidRenderSettingsSection_FocalBlur);

      return types;
    }

    protected override Result Render(RhinoDoc doc, RunMode mode, bool fastPreview)
    {
      return Result.Success;
    }
  }
}