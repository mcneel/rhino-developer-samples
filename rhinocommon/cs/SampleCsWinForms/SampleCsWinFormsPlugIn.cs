using Rhino;
using Rhino.PlugIns;
using Rhino.UI;
using SampleCsWinForms.Forms;
using System.Collections.Generic;

namespace SampleCsWinForms
{
  public class SampleCsWinFormsPlugIn : PlugIn
  {
    public SampleCsWinFormsPlugIn()
    {
      Instance = this;
    }

    /// <summary>
    /// Gets the only instance of the SampleCsWinFormsPlugIn plug-in.
    /// </summary>
    public static SampleCsWinFormsPlugIn Instance { get; private set; }

    /// <summary>
    /// The tabbed dockbar user control
    /// </summary>
    public SampleCsPanelUserControl PanelUserControl { get; set; }

    /// <summary>
    /// Called when the plug-in is being loaded.
    /// </summary>
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      System.Type type = typeof(SampleCsPanelUserControl);
      Panels.RegisterPanel(this, type, "SampleWinForms", SampleCsWinForms.Properties.Resources.Panel, PanelType.System);
      return LoadReturnCode.Success;
    }

    protected override void OptionsDialogPages(List<OptionsDialogPage> pages)
    {
      SampleCsOptionsPage sample_page = new SampleCsOptionsPage();
      pages.Add(sample_page);
    }

    protected override void DocumentPropertiesDialogPages(RhinoDoc doc, List<OptionsDialogPage> pages)
    {
      SampleCsDocPropertiesPage sample_page = new SampleCsDocPropertiesPage(doc);
      pages.Add(sample_page);
    }

    protected override void ObjectPropertiesPages(ObjectPropertiesPageCollection collection)
    {
      SampleCsObjectPropertiesPage sample_page = new SampleCsObjectPropertiesPage();
      collection.Add(sample_page);
    }

  }
}